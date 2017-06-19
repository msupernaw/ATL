/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vector.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 10:07 AM
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Matrix.hpp"


namespace atl {

    /**
     * VariableVector is a 1 x M VariableMatrix.
     */
    template<typename T>
    struct VariableVector : atl::ExpressionBase<T, VariableVector<T > > {
        //        using VariableMatrix<T>::operator=;

        size_t columns_m;
        std::vector<atl::Variable<T> > data_m;

        /**
         * Constructor. 
         * @param columns
         */
        VariableVector(size_t columns = 0) :
        columns_m(columns) {
            this->data_m.resize(columns);
        }

        /**
         * Assignment operator for real scalar types.
         * 
         * @param exp
         * @return 
         */
        VariableVector& operator=(const T& v) {
            for (int i = 0; i < this->columns_m; i++) {
                this->data_m[i].SetValue(v);
            }
            return *this;
        }

        /**
         * Assignment operator for variable scalar types.
         * 
         * @param exp
         * @return 
         */
        VariableVector& operator=(const atl::Variable<T>& v) {
            for (int i = 0; i < this->columns_m; i++) {
                this->data_m[i].SetValue(v.GetValue());
            }
            return *this;
        }

     

        template<class A>
        VariableVector& operator=(const atl::ExpressionBase<T, A>& exp) {

            if (exp.IsScalar()) {
                if (this->data_m.size() == 0) {
                    this->data_m.resize(1);
                }
                for (int i = 0; i < this->columns_m; i++) {
                    this->data_m[i] = exp;
                }
            } else {
                if (exp.GetRows() > 1) {
                    throw std::invalid_argument("VariableVector& operator=(const atl::ExpressionBase<T,A>& exp) with rows > 0 called on vector template.");
                } else {
                    for (int i = 0; i < this->columns_m; i++) {
                        this->data_m[i] = exp;
                    }

                }
            }


            return *this;
        }

        /**
         * Sets the size of this vector.
         * 
         * @param size
         */
        void SetSize(size_t size) {
            this->columns_m = size;
            this->data_m.resize(size);
        }

        atl::Variable<T>& operator()(size_t j) {
            return this->data_m[j];
        }

        /**
         * Returns the size of this vector.
         * @return 
         */
        size_t GetSize() {
            return this->columns_m;
        }

        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on vector template.");
        }

        inline const T GetValue(size_t i, size_t j = 0) const {
            return this->data_m[j].GetValue();
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids)const {
            throw std::invalid_argument("PushIds(typename atl::StackEntry<T>::vi_storage& ids) called on vector template.");
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j = 0)const {
            data_m[j].PushIds(ids, i, j);
        }

        std::shared_ptr<DynamicExpressionBase<T> > ToDynamic() const {
            throw std::invalid_argument("Not yet implemented. ToDynamic() called on vector template.");
        }

        inline T EvaluateDerivative(uint32_t x) const {
            throw std::invalid_argument("EvaluateDerivative(uint32_t x) called on vector template.");
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y) const {
            throw std::invalid_argument("EvaluateDerivative(uint32_t x,uint32_t y) called on vector template.");
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            throw std::invalid_argument("EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) called on vector template.");
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return this->data_m[j].EvaluateDerivative(x);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return this->data_m[j].EvaluateDerivative(x, y);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return this->data_m[j].EvaluateDerivative(x, y, z);
        }

        size_t GetColumns() const {
            return this->columns_m;
        }

        size_t GetRows() const {
            return 1;
        }

        bool IsScalar()const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::VariableVector<T>";
            return ss.str();
        }


    };

    /**
     * RealVector is a 1 x M RealMatrix.
     */
    template<typename T>
    struct RealVector : RealMatrix<T> {
        using RealMatrix<T>::operator=;

        /**
         * Constructor. 
         * @param columns
         */
        RealVector(size_t columns = 0) :
        RealMatrix<T>(1, columns) {
        }

        /**
         * Sets the size of this vector.
         * 
         * @param size
         */
        void SetSize(size_t size) {
            this->columns = size;
            this->data_m.resize(size);
        }

        /**
         * Returns the size of this vector.
         * @return 
         */
        size_t GetSize() {
            return this->columns;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::RealVector<T>";
            return ss.str();
        }


    };

}


#endif /* VECTOR_HPP */

