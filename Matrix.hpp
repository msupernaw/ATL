/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Matrix.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 10:07 AM
 */

#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include "Expression.hpp"
#include "Tape.hpp"
#include "Variable.hpp"
#include "ATL.hpp"
#include "ThreadPool.hpp"
#include "CLFAllocator.hpp"
namespace atl {

    template<typename T>
    struct VariableVector;

    template<typename T>
    struct VariableMatrix;

    template<typename T>
    struct RealMatrix;

    template< typename A, typename T>
    struct MatrixAssign {
        static void Assign(VariableMatrix<T>& m, atl::ExpressionBase<T, A>& exp);
    };

    /**
     * VariableMatrix assign functor used by 
     * \ref VariableMatrix::AssignConcurrent
     * 
     * Passed to the \ref ThreadPool.
     * 
     */
    template<typename T, class A>
    struct VariableMatrixAssign {
        atl::VariableMatrix<T>& m;
        std::vector<T>& temp;

        size_t index_start;
        size_t row_start;
        size_t row_end;
        const atl::ExpressionBase<T, A>& exp;

        VariableMatrixAssign(
                atl::VariableMatrix<T>& m,
                std::vector<T>& temp,
                size_t index_start,
                size_t row_start,
                size_t row_end,
                const atl::ExpressionBase<T, A>& exp) :
        m(m), temp(temp), index_start(index_start), row_start(row_start), row_end(row_end), exp(exp) {
        }

        inline void operator()() {
            //            typename atl::StackEntry<T>::vi_iterator it;
            size_t k = 0;
            size_t index;
#pragma unroll
            for (size_t i = row_start; i < row_end; i++) {
                index = i * m.columns;
                for (size_t j = 0; j < m.columns; j++) {
                    atl::StackEntry<T>& entry = atl::Variable<T>::tape.stack[index_start++];
                    exp.PushIds(entry.ids, i, j);

                    entry.w = m.data_m[index + j].info;
                    entry.first.resize(entry.ids.size());
                    temp[index + j] = exp.GetValue(i, j);

                    k = 0;
                    for (auto it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                        entry.first[k] = exp.EvaluateDerivative((*it)->id, i, j);
                        k++;
                    }
                }
            }

        }
    };

    /**
     * Matrix of \ref Variable types.
     * 
     */
    template<typename T>
    struct VariableMatrix : atl::ExpressionBase<T, VariableMatrix<T> > {
        size_t rows;
        size_t columns;

        std::vector<atl::Variable<T>, atl::clfallocator<atl::Variable<T> > > data_m;

        /**
         * Constructor.
         * 
         * @param rows
         * @param columns
         * @param initial_value
         */
        VariableMatrix(size_t rows = 0, size_t columns = 1, T initial_value = static_cast<T> (0.0)) :
        rows(rows), columns(columns) {
            data_m.resize(rows * columns);
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = atl::Variable<T>(initial_value);
            }
        }

        /**
         * Assignment operator. Sets all entries to 
         * value. 
         * @param value
         * @return 
         */
        inline VariableMatrix& operator=(const T& value) {
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = value;
            }
            return *this;
        }

        /**
         * Assignment operator for expression template types.
         * 
         * @param exp
         * @return 
         */
        template<class A>
        inline VariableMatrix& operator=(const ExpressionBase<T, A>& exp) {

            this->columns = exp.GetColumns();
            this->rows = exp.GetRows();
            std::vector<T > temp(this->rows * this->columns);
            data_m.resize(columns * rows);
            size_t index = atl::Variable<T>::tape.GetBlock(temp.size());

            size_t k = 0;
            for (size_t i = 0; i < this->rows; i++) {
                for (size_t j = 0; j < this->columns; j++) {
                    atl::StackEntry<T>& entry = atl::Variable<T>::tape.stack[index++];
                    exp.PushIds(entry.ids, i, j);
                    entry.w = this->data_m[i * columns + j].info;
                    entry.first.resize(entry.ids.size());
                    temp[i * columns + j] = exp.GetValue(i, j);
                    typename atl::StackEntry<T>::vi_iterator it;
                    k = 0;
                    for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                        entry.first[k] = exp.EvaluateDerivative((*it)->id, i, j);
                        k++;
                    }
                }
            }

            //in case there is aliasing
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = temp[i];
            }

            return *this;
        }

        /**
         * Assignment function for concurrent assignment. This function 
         * guarantees proper tape recording by reserving a block of entries from
         * the \ref Tape and allocating among threads accordingly. Each thread 
         * receives a reference to exp and its range of \ref Tape entries.
         * 
         * @param exp
         * @return 
         */
        template<class A>
        inline VariableMatrix& AssignConcurrent(const ExpressionBase<T, A>& exp) {
            this->columns = exp.GetColumns();
            this->rows = exp.GetRows();
            std::vector<T > temp(this->rows * this->columns);
            data_m.resize(columns * rows);
            size_t index = atl::Variable<T>::tape.GetBlock(temp.size());


            size_t nthreads = atl::thread_pool_g.Size();
            size_t range = this->rows / nthreads;
            size_t chunk = range * this->columns;
            atl::WaitVariable wv;

            for (size_t t = 0; t < nthreads; t++) {
                size_t start = (t * range);
                size_t end;
                t < (nthreads - 1) ? end = (t + 1) * range : end = rows;
                size_t index_start = t*chunk;
                //                (atl::VariableMatrix<T>& m, std::vector<T>& temp,
                //            size_t index_start, size_t row_start, size_t row_end, size_t chunk,
                //            atl::ExpressionBase<T, A>& exp)
                atl::thread_pool_g.DoJob(std::bind(atl::VariableMatrixAssign<T, A>(*this,
                        temp, index_start, start, end, exp)), wv);

            }
            atl::thread_pool_g.Wait(wv);

            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = temp[i];
            }

            return *this;
        }

        /**
         * Push variable info into a set for index {0,0}..
         *  
         * @param ids
         */
        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids)const {
            this->data_m[0].PushIds(ids);
        }

        /**
         * Push variable info into a set at index {i,j}.
         *  
         * @param ids
         */
        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j = 0)const {
            this->data_m[i * columns + j].PushIds(ids);
        }

        /**
         * 
         * @param i
         * @param j
         * @return 
         */
        atl::Variable<T>& operator()(size_t i, size_t j = 0) {
            return this->data_m[i * columns + j];
        }

        /**
         * 
         * @param i
         * @param j
         * @return 
         */
        const atl::Variable<T>& operator()(size_t i, size_t j = 0) const {
            return this->data_m[i * columns + j];
        }

        /**
         * throws std::invalid_argument
         * @return 
         */
        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on matrix template.");
            return this->data_m[0].GetValue();
        }

        /**
         * Returns the real value at index {i,j}. 
         * @param i
         * @param j
         * @return 
         */
        inline const T GetValue(size_t i, size_t j = 0) const {
            return this->data_m[i * columns + j].GetValue();
        }

        /**
         * Returns false.
         * 
         * @return 
         */
        inline bool IsNonlinear() const {
            return false;
        }

        /**
         * throws std::invalid_argument
         * @return 
         */
        inline T EvaluateDerivative(uint32_t x) const {
            throw std::invalid_argument("EvaluateDerivative(uint32_t x) called on matrix template.");
            return this->data_m[0].EvaluateDerivative(x);
        }

        /**
         * throws std::invalid_argument
         * @return 
         */
        inline T EvaluateDerivative(uint32_t x, uint32_t y) const {
            throw std::invalid_argument("EvaluateDerivative(uint32_t x,uint32_t y) called on matrix template.");
            return static_cast<T> (0.0);
        }

        /**
         * throws std::invalid_argument
         * @return 
         */
        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            throw std::invalid_argument("EvaluateDerivative(uint32_t x, uint32_t y,uint32_t z) called on matrix template.");
            return static_cast<T> (0.0);
        }

        /**
         * Evaluates the first-order derivative at index {i,j}.
         * 
         * Returns 1 if index {i,j} is equal to x, else it returns 0.
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return this->data_m[i * columns + j].EvaluateDerivative(x);
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return static_cast<T> (0.0);
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return static_cast<T> (0.0);
        }

        /**
         * Returns the number of columns.
         * 
         * @return 
         */
        size_t GetColumns() const {
            return columns;
        }

        /**
         * Returns the number of rows.
         * 
         * @return 
         */
        size_t GetRows() const {
            return rows;
        }

        /**
         * Returns false.
         */
        bool IsScalar()const {
            return false;
        }



    };

    template<typename T, size_t ROWS, size_t COLUMNS>
    struct FastVariableMatrix : atl::ExpressionBase<T, FastVariableMatrix<T, ROWS, COLUMNS> > {
        size_t rows = ROWS;
        size_t columns = COLUMNS;

        atl::Variable<T> data_m[ROWS*COLUMNS];
        //        std::vector<atl::Variable<T> , atl::clfallocator<atl::Variable<T> > > data_m;

        FastVariableMatrix() :
        rows(ROWS), columns(COLUMNS) {
            ////            data_m.resize(rows * columns);
            //            for (int i = 0; i <(rows * columns); i++) {
            //                data_m[i] = atl::Variable<T>();
            //            }
        }

        template<class A>
        inline FastVariableMatrix& operator=(const ExpressionBase<T, A>& exp) {

            this->columns = exp.GetColumns();
            this->rows = exp.GetRows();
            std::vector<T > temp(this->rows * this->columns);
            data_m.resize(columns * rows);
            size_t index = atl::Variable<T>::tape.GetBlock(temp.size());


            for (size_t i = 0; i < this->rows; i++) {
                for (size_t j = 0; j < this->columns; j++) {
                    atl::StackEntry<T>& entry = atl::Variable<T>::tape.stack[index++];
                    exp.PushIds(entry.ids, i, j);
                    entry.w = this->data_m[i * columns + j].info;
                    entry.first.resize(entry.ids.size());
                    temp[i * columns + j] = exp.GetValue(i, j);
                    typename atl::StackEntry<T>::vi_iterator it;
                    size_t k = 0;
                    for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                        entry.first[k] = exp.EvaluateDerivative((*it)->id, i, j);
                        k++;
                    }
                }
            }

            //in case there is aliasing
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = temp[i];
            }

            return *this;
        }

        template<class A>
        inline void AssignConcurrent(const ExpressionBase<T, A>& exp) {
            this->columns = exp.GetColumns();
            this->rows = exp.GetRows();
            std::vector<T > temp(this->rows * this->columns);
            data_m.resize(columns * rows);
            size_t index = atl::Variable<T>::tape.GetBlock(temp.size());


            size_t nthreads = atl::thread_pool_g.Size();
            size_t range = this->rows / nthreads;
            size_t chunk = range * this->columns;
            atl::WaitVariable wv;

            for (size_t t = 0; t < nthreads; t++) {
                size_t start = (t * range);
                size_t end;
                t < (nthreads - 1) ? end = (t + 1) * range : end = rows;
                size_t index_start = t*chunk;
                //                (atl::VariableMatrix<T>& m, std::vector<T>& temp,
                //            size_t index_start, size_t row_start, size_t row_end, size_t chunk,
                //            atl::ExpressionBase<T, A>& exp)
                atl::thread_pool_g.DoJob(std::bind(atl::VariableMatrixAssign<T, A>(*this,
                        temp, index_start, start, end, exp)), wv);

            }
            atl::thread_pool_g.Wait(wv);

            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = temp[i];
            }


        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids)const {
            this->data_m[0].PushIds(ids);
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j = 0)const {
            this->data_m[i * columns + j].PushIds(ids);
        }

        atl::Variable<T>& operator()(size_t i, size_t j = 0) {
            return this->data_m[i * columns + j];
        }

        const atl::Variable<T>& operator()(size_t i, size_t j = 0) const {
            return this->data_m[i * columns + j];
        }

        inline const T GetValue() const {
            return this->data_m[0].GetValue();
        }

        inline const T GetValue(size_t i, size_t j = 0) const {
            return this->data_m[i * columns + j].GetValue();
        }

        inline T EvaluateDerivative(uint32_t a) const {
            return this->data_m[0].EvaluateDerivative(a);
        }

        inline T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t a, size_t i, size_t j = 0) const {
            return this->data_m[i * columns + j].EvaluateDerivative(a);
        }

        inline T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return static_cast<T> (0.0);
        }

        size_t GetColumns() const {
            return columns;
        }

        size_t GetRows() const {
            return rows;
        }

        bool IsScalar()const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::Matrix<T>";
            return ss.str();
        }


    };

    template<typename REAL_T>
    std::ostream& operator<<(std::ostream& out, const VariableMatrix<REAL_T>& m) {
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.columns; j++) {
                out << m.data_m[i * m.columns + j].GetValue() << " ";
            }
            std::cout << std::endl;
        }

        return out;
    }



}



#endif /* MATRIX_HPP */

