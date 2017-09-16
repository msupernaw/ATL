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

#include "Expression.hpp"
#include "Variable.hpp"
#include "Matrix.hpp"
#include <cassert>

namespace atl {

    /**
     * VariableRowVector is a 1 x M VariableMatrix.
     *
     * This object references values from a row in a VariableMatrix. Therfore,
     * if a value is modified, it is also modified in the VariableMatrix of
     * its origin.
     *
     */
    template<typename T>
    struct VariableRowVector : atl::ExpressionBase<T, VariableRowVector<T > > {
        size_t rows_m = 0;
        size_t columns_m = 0;
        std::vector<atl::Variable<T>, atl::clfallocator<atl::Variable<T> > >& data_m;

        VariableRowVector(size_t rows_m, size_t columns_m, std::vector<atl::Variable<T>, atl::clfallocator<atl::Variable<T> > >& data_m) :
        rows_m(rows_m), columns_m(columns_m), data_m(data_m) {
        }

        VariableRowVector(const VariableRowVector<T>& other) :
        rows_m(other.rows_m), columns_m(other.columns_m), data_m(other.data_m) {
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

        atl::Variable<T>& operator()(size_t j) const {
            return this->data_m[rows_m * this->columns_m + j];
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() const {
            return this->columns_m;
        }

        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on vector template.");
        }

        inline const T GetValue(size_t j) const {
            return this->data_m[rows_m * this->columns_m + j].GetValue();
        }

        inline const T GetValue(size_t i, size_t j) const {
            return this->data_m[rows_m * this->columns_m + j].GetValue();
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids) const {
            throw std::invalid_argument("PushIds(typename atl::StackEntry<T>::vi_storage& ids) called on vector template.");
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j) const {
            data_m[rows_m * this->columns_m + j].PushIds(ids);
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

        inline T EvaluateDerivative(uint32_t x, size_t i, size_t j) const {
            return this->data_m[rows_m * this->columns_m + j].EvaluateDerivative(x);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j) const {
            return this->data_m[rows_m * this->columns_m + j].EvaluateDerivative(x, y);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j) const {
            return this->data_m[rows_m * this->columns_m + j].EvaluateDerivative(x, y, z);
        }

        size_t GetColumns() const {
            return this->columns_m;
        }

        size_t GetRows() const {
            return 1;
        }

        bool IsScalar() const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::VariableRowVector<T>";
            return ss.str();
        }


    };

    /**
     * RealRowVector is a 1 x M VariableMatrix.
     *
     * This object references values from a row in a RealMatrix. Therfore,
     * if a value is modified, it is also modified in the RealMatrix of
     * its origin.
     *
     */
    template<typename T>
    struct RealRowVector : atl::ExpressionBase<T, RealRowVector<T > > {
        size_t rows_m = 0;
        size_t columns_m = 0;
        std::vector<T, atl::clfallocator<T > >& data_m;

        RealRowVector(size_t rows_m, size_t columns_m, std::vector<T, atl::clfallocator<T> >& data_m) :
        rows_m(rows_m), columns_m(columns_m), data_m(data_m) {
        }

        RealRowVector(const RealRowVector<T>& other) :
        rows_m(other.rows_m), columns_m(other.columns_m), data_m(other.data_m) {
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

        T& operator()(size_t j) const {
            return this->data_m[rows_m * this->columns_m + j];
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() const {
            return this->columns_m;
        }

        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on vector template.");
        }

        inline const T GetValue(size_t j) const {
            return this->data_m[rows_m * this->columns_m + j];
        }

        inline const T GetValue(size_t i, size_t j) const {
            return this->data_m[rows_m * this->columns_m + j];
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids) const {
            throw std::invalid_argument("PushIds(typename atl::StackEntry<T>::vi_storage& ids) called on vector template.");
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j) const {

        }

        std::shared_ptr<DynamicExpressionBase<T> > ToDynamic() const {
            throw std::invalid_argument("Not yet implemented. ToDynamic() called on vector template.");
        }

        inline T EvaluateDerivative(uint32_t x) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, size_t i, size_t j) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j) const {
            return static_cast<T> (0.0);
        }

        size_t GetColumns() const {
            return this->columns_m;
        }

        size_t GetRows() const {
            return 1;
        }

        bool IsScalar() const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "RealRowVector<T>";
            return ss.str();
        }


    };

    /**
     * VariableColumnVector is a N x 1 VariableMatrix.
     *
     * This object references values from a column in a VariableMatrix. Therfore,
     * if a value is modified, it is also modified in the VariableMatrix of
     * its origin.
     *
     */
    template<typename T>
    struct VariableColumnVector : atl::ExpressionBase<T, VariableColumnVector<T > > {
        size_t rows_m = 0;
        size_t columns_m = 0;
        size_t col = 0;
        std::vector<atl::Variable<T>, atl::clfallocator<atl::Variable<T> > >& data_m;

        VariableColumnVector(size_t rows_m, size_t columns_m, size_t col, std::vector<atl::Variable<T>, atl::clfallocator<atl::Variable<T> > >& data_m) :
        rows_m(rows_m), columns_m(columns_m), col(col), data_m(data_m) {
        }

        VariableColumnVector(const VariableColumnVector<T>& other) :
        rows_m(other.rows_m), columns_m(other.columns_m), col(other.col), data_m(other.data_m) {
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

        atl::Variable<T>& operator()(size_t i) {
            return this->data_m[i * this->columns_m + col];
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() {
            return this->rows_m;
        }

        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on vector template.");
        }

        inline const T GetValue(size_t i, size_t j = 0) const {
            return this->data_m[i * this->columns_m + col].GetValue();
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids) const {
            throw std::invalid_argument("PushIds(typename atl::StackEntry<T>::vi_storage& ids) called on vector template.");
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j = 0) const {
            data_m[i * this->columns_m + col].PushIds(ids);
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
            return this->data_m[i * this->columns_m + col].EvaluateDerivative(x);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return this->data_m[i * this->columns_m + col].EvaluateDerivative(x, y);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return this->data_m[i * this->columns_m + col].EvaluateDerivative(x, y, z);
        }

        size_t GetColumns() const {
            return 1;
        }

        size_t GetRows() const {
            return this->rows_m;
        }

        bool IsScalar() const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::VariableColumnVector<T>";
            return ss.str();
        }


    };

    /**
     * RealColumnVector is a N x 1 VariableMatrix.
     *
     * This object references values from a column in a VariableMatrix. Therfore,
     * if a value is modified, it is also modified in the VariableMatrix of
     * its origin.
     *
     */
    template<typename T>
    struct RealColumnVector : atl::ExpressionBase<T, RealColumnVector<T > > {
        size_t rows_m = 0;
        size_t columns_m = 0;
        size_t col = 0;
        std::vector<T, atl::clfallocator<T > >& data_m;

        RealColumnVector(size_t rows_m, size_t columns_m, size_t col, std::vector<T, atl::clfallocator<T> >& data_m) :
        rows_m(rows_m), columns_m(columns_m), col(col), data_m(data_m) {
        }

        RealColumnVector(const RealColumnVector<T>& other) :
        rows_m(other.rows_m), columns_m(other.columns_m), col(other.col), data_m(other.data_m) {
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

        T& operator()(size_t i) {
            return this->data_m[i * this->columns_m + col];
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() {
            return this->rows_m;
        }

        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on vector template.");
        }

        inline const T GetValue(size_t i, size_t j = 0) const {
            return this->data_m[i * this->columns_m + col];
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids) const {
            throw std::invalid_argument("PushIds(typename atl::StackEntry<T>::vi_storage& ids) called on vector template.");
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j = 0) const {

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
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return static_cast<T> (0.0);
        }

        size_t GetColumns() const {
            return 1;
        }

        size_t GetRows() const {
            return this->rows_m;
        }

        bool IsScalar() const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::RealColumnVector<T>";
            return ss.str();
        }


    };

    /**
     * VariableVector is a 1 x M VariableMatrix.
     */
    template<typename T>
    struct VariableVector : atl::ExpressionBase<T, VariableVector<T > > {
        size_t columns_m;
        std::vector<atl::Variable<T> > data_m;

        VariableVector(const VariableVector<T>& other) :
        columns_m(other.columns_m), data_m(other.data_m) {
        }

        /**
         * Constructor.
         * @param columns
         */
        VariableVector(size_t columns = 0) :
        columns_m(columns) {
            this->data_m.resize(columns);
        }

        template<class A>
        VariableVector(const atl::ExpressionBase<T, A>& exp) {
            this->Assign(exp, atl::Variable<T>::tape);
        }

        VariableVector(const std::initializer_list<T>& l) {
            this->columns_m = l.size();

            typename std::initializer_list<T>::iterator jt;
            data_m.resize(this->columns_m);
            int index = 0;

            for (jt = l.begin(); jt != l.end(); ++jt) {
                T v = (*jt);
                data_m[index++] = v;
            }

        }

        /**
         * Assignment operator. Sets all entries to
         * value.
         * @param value
         * @return
         */
        inline VariableVector& operator=(const T& value) {
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i].info->value = value;
            }
            return *this;
        }

        inline VariableVector& operator=(const std::initializer_list<T>& l) const {
            this->columns_m = l.size();

            typename std::initializer_list<T>::iterator jt;
            data_m.resize(this->columns_m);
            int index = 0;

            for (jt = l.begin(); jt != l.end(); ++jt) {
                T v = (*jt);
                data_m[index++] = v;
            }

            return *this;
        }

        //        /**
        //         * Assignment operator for variable scalar types.
        //         *
        //         * @param exp
        //         * @return
        //         */
        //        VariableVector& operator=(const atl::Variable<T>& v) const {
        //            for (int i = 0; i < this->columns_m; i++) {
        //                this->data_m[i].SetValue(v.GetValue());
        //            }
        //            return *this;
        //        }

        template<class A>
        inline VariableVector<T>& operator=(const ExpressionBase<T, A>& exp) {

            this->Assign(exp, atl::Variable<T>::tape);

            return *this;
        }

        template<class A>
        inline void Assign(const ExpressionBase<T, A>& exp, atl::Tape<T>& tape) {
            this->columns_m = exp.GetColumns();

            //            assert(exp.GetRows() <= 1);
            if (exp.GetRows() > 1) {
                throw std::invalid_argument("VariableVector& operator=(const atl::ExpressionBase<T,A>& exp) with rows > 1 called on vector template.");
            }

            std::vector<T > temp(this->columns_m);
            this->data_m.resize(temp.size());
            size_t index = tape.GetBlock(temp.size());

            size_t i = 0;
            for (size_t j = 0; j < this->columns_m; j++) {

                temp[j] = exp.GetValue(i, j);
                typename atl::StackEntry<T>& entry = tape.stack[index++];

                exp.PushIds(entry.ids, i, j);

                entry.w = this->data_m[j].info;
                entry.w->count++;
                entry.w->is_nl = true;
                entry.first.resize(entry.ids.size(), static_cast<T> (0.0));
                typename atl::StackEntry<T>::vi_iterator it;
                typename atl::StackEntry<T>::vi_iterator jt;
                typename atl::StackEntry<T>::vi_iterator kt;
                size_t ii = 0;
                size_t jj = 0;
                size_t kk = 0;
                entry.wv = exp.GetValue(i, j);
                switch (tape.derivative_trace_level) {

                    case FIRST_ORDER_REVERSE:
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            entry.first[ii] = exp.EvaluateDerivative((*it)->id, i, j);
                            ii++;
                        }
                        break;

                    case SECOND_ORDER_REVERSE:
                        entry.w->is_nl = exp.IsNonlinear();
                        entry.is_nl = exp.IsNonlinear();

                        entry.second.resize(entry.ids.size() * entry.ids.size(), static_cast<T> (0.0));

                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {

                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            entry.first[ii] = exp.EvaluateDerivative((*it)->id);
                            jj = 0;
                            for (jt = entry.ids.begin(); jt != entry.ids.end(); ++jt) {
                                entry.second[ii * entry.ids.size() + jj] = exp.EvaluateDerivative((*it)->id, (*jt)->id, i, j);
                                jj++;
                            }
                            i++;
                        }
                        break;

                    case THIRD_ORDER_REVERSE:
                        entry.w->is_nl = exp.IsNonlinear();
                        entry.is_nl = exp.IsNonlinear();

                        entry.second.resize(entry.ids.size() * entry.ids.size(), static_cast<T> (0.0));
                        entry.third.resize(entry.ids.size() * entry.ids.size() * entry.ids.size(), static_cast<T> (0.0));
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            (*it)->live++;
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            entry.first[i] = exp.EvaluateDerivative((*it)->id, i, j);
                            jj = 0;
                            for (jt = entry.ids.begin(); jt != entry.ids.end(); ++jt) {
                                entry.second[ii * entry.ids.size() + jj] = exp.EvaluateDerivative((*it)->id, (*jt)->id);
                                kk = 0;
                                for (kt = entry.ids.begin(); kt != entry.ids.end(); ++kt) {

                                    entry.third[ii * entry.ids.size() * entry.ids.size() + jj * entry.ids.size() + kk] =
                                            exp.EvaluateDerivative((*it)->id, (*jt)->id, (*kt)->id, i, j);
                                    kk++;
                                }
                                jj++;
                            }
                            ii++;
                        }
                        break;
                    case atl::UTPM_REVERSE:
                        throw std::invalid_argument("UTPM not yet implemented for VariableVector.");

                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            (*it)->tayor_coefficients.resize(tape.taylor_order + 1);
                            (*it)->tayor_coefficients[0] = (*it)->value;
                        }
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            (*it)->tayor_coefficients[1] = static_cast<T> (1.0);
                            for (ii = 0; i <= tape.taylor_order; i++) {
                                entry.taylor_coeff[(*it)->id].push_back(exp.Taylor(ii));
                            }
                            (*it)->tayor_coefficients[1] = static_cast<T> (0.0);
                        }

                        break;

                    case DYNAMIC_RECORD:

                        throw std::invalid_argument("DYNAMIC_RECORD not yet implemented for VariableVector.");


                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                        }
                        entry.exp = exp.ToDynamic();

                        break;
                    default:
                        std::cout << "Unknown Derivative Trace Level.\n";
                        exit(0);
                }

            }

            //in case there is aliasing
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = temp[i];
            }


        }

        inline VariableVector& operator+=(const T& val) {
            *this = *this+val;
            return *this;
        }

        template<class A>
        inline VariableVector& operator+=(const ExpressionBase<T, A>& exp) {
            *this = *this+exp;
            return *this;
        }

        inline VariableVector operator-=(const T& val) {
            *this = *this-val;
            return *this;
        }

        inline VariableVector<T> operator-() const {
            return static_cast<T> (-1.0) * (*this);
        }

        template<class A>
        inline VariableVector operator-=(const ExpressionBase<T, A>& exp) {
            *this = *this-exp;
            return *this;
        }

        inline VariableVector& operator*=(const T& val) {
            *this = *this*val;
            return *this;
        }

        template<class A>
        inline VariableVector& operator*=(const ExpressionBase<T, A>& exp) {
            *this = *this*exp;
            return *this;
        }

        inline VariableVector& operator/=(const T& val) {
            *this = *this / val;
            return *this;
        }

        template<class A>
        inline VariableVector& operator/=(const ExpressionBase<T, A>& exp) {
            *this = *this / exp;
            return *this;
        }

        void SetBounds(const T& min, const T& max) {
            for (int i = 0; i<this->data_m.size(); i++) {
                this->data_m[i].SetBounds(min, max);
            }
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

        /**
         * Sets the size of this vector.
         *
         * @param size
         */
        void Resize(size_t size) {
            this->columns_m = size;
            this->data_m.resize(size);
        }

        atl::Variable<T>& operator()(size_t j) {
            return this->data_m[j];
        }

        const atl::Variable<T>& operator()(size_t j) const {
            return this->data_m[j];
        }

        void SetName(const std::string& name) {
            std::stringstream ss;
            for (int i = 0; i < this->columns_m; i++) {
                ss.str("");
                ss << name << "(" << i << ")";
                this->data_m[i].SetName(ss.str());
            }
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() {
            return this->columns_m;
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() const {
            return this->columns_m;
        }

        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on vector template.");
        }

        inline const T GetValue(size_t i, size_t j) const {
            return this->data_m[j].GetValue();
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids) const {
            throw std::invalid_argument("PushIds(typename atl::StackEntry<T>::vi_storage& ids) called on vector template.");
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j) const {
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

        inline T EvaluateDerivative(uint32_t x, size_t i, size_t j) const {
            return this->data_m[j].EvaluateDerivative(x);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j) const {
            return this->data_m[j].EvaluateDerivative(x, y);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j) const {
            return this->data_m[j].EvaluateDerivative(x, y, z);
        }

        size_t GetColumns() const {
            return this->columns_m;
        }

        size_t GetRows() const {
            return 1;
        }

        bool IsScalar() const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::VariableVector<T>";
            return ss.str();
        }

    };

    /**
     * RealVector is a 1 x M VariableMatrix.
     */
    template<typename T>
    struct RealVector : atl::ExpressionBase<T, RealVector<T > > {
        size_t columns_m;
        std::vector<T> data_m;

        RealVector(const RealVector<T>& other) :
        columns_m(other.columns_m), data_m(other.data_m) {
        }

        /**
         * Constructor.
         * @param columns
         */
        RealVector(size_t columns = 0) :
        columns_m(columns) {
            this->data_m.resize(columns);
        }

        template<class A>
        RealVector(const atl::ExpressionBase<T, A>& exp) {
            this->Assign(exp, atl::Variable<T>::tape);
        }

        RealVector(const std::initializer_list<T>& l) {
            this->columns_m = l.size();

            typename std::initializer_list<T>::iterator jt;
            data_m.resize(this->columns_m);
            int index = 0;

            for (jt = l.begin(); jt != l.end(); ++jt) {
                T v = (*jt);
                data_m[index++] = v;
            }

        }

        /**
         * Assignment operator. Sets all entries to
         * value.
         * @param value
         * @return
         */
        inline RealVector& operator=(const T& value) {
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i].info->value = value;
            }
            return *this;
        }

        inline RealVector& operator=(const std::initializer_list<T>& l) const {
            this->columns_m = l.size();

            typename std::initializer_list<T>::iterator jt;
            data_m.resize(this->columns_m);
            int index = 0;

            for (jt = l.begin(); jt != l.end(); ++jt) {
                T v = (*jt);
                data_m[index++] = v;
            }

            return *this;
        }

        /**
         * Assignment operator for variable scalar types.
         *
         * @param exp
         * @return
         */
        RealVector& operator=(const T& v) const {
            for (int i = 0; i < this->columns_m; i++) {
                this->data_m[i] = v;
            }
            return *this;
        }

        template<class A>
        inline RealVector& operator=(const ExpressionBase<T, A>& exp) {

            this->Assign(exp, atl::Variable<T>::tape);

            return *this;
        }

        template<class A>
        inline void Assign(const ExpressionBase<T, A>& exp, atl::Tape<T>& tape) {
            this->columns_m = exp.GetColumns();

            //            assert(exp.GetRows() <= 1);
            if (exp.GetRows() > 1) {
                throw std::invalid_argument("VariableVector& operator=(const atl::ExpressionBase<T,A>& exp) with rows > 1 called on vector template.");
            }

            std::vector<T > temp(this->columns_m);
            this->data_m.resize(temp.size());
            size_t index = tape.GetBlock(temp.size());

            size_t i = 0;
            for (size_t j = 0; j < this->columns_m; j++) {
                temp[j] = exp.GetValue(i, j);
            }

            //in case there is aliasing
            for (int i = 0; i < data_m.size(); i++) {
                data_m[i] = temp[i];
            }


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

        /**
         * Sets the size of this vector.
         *
         * @param size
         */
        void Resize(size_t size) {
            this->columns_m = size;
            this->data_m.resize(size);
        }

        T& operator()(size_t j) {
            return this->data_m[j];
        }

        const T& operator()(size_t j) const {
            return this->data_m[j];
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() {
            return this->columns_m;
        }

        /**
         * Returns the size of this vector.
         * @return
         */
        size_t GetSize() const {
            return this->columns_m;
        }

        inline const T GetValue() const {
            throw std::invalid_argument("GetValue() called on vector template.");
        }

        inline const T GetValue(size_t i, size_t j) const {
            return this->data_m[j].GetValue();
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids) const {
            throw std::invalid_argument("PushIds(typename atl::StackEntry<T>::vi_storage& ids) called on vector template.");
        }

        inline void PushIds(typename atl::StackEntry<T>::vi_storage& ids, size_t i, size_t j) const {
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

        inline T EvaluateDerivative(uint32_t x, size_t i, size_t j) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j) const {
            return static_cast<T> (0.0);
        }

        inline T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j) const {
            return static_cast<T> (0.0);
        }

        size_t GetColumns() const {
            return this->columns_m;
        }

        size_t GetRows() const {
            return 1;
        }

        bool IsScalar() const {
            return false;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::RealVector<T>";
            return ss.str();
        }


    };

    template<class T>
    inline const atl::VariableVector<T> FirstDifference(const atl::VariableVector<T> &expr) {
        atl::VariableVector<T> ret;
        size_t n = expr.GetSize() - 1;
        ret.Resize(n);
        for (size_t i = 0; i < n; i++) {
            ret(i) = expr(i + 1) - expr(i);
        }
        return ret;
    }

    template<class T>
    inline const atl::Variable<T> Norm2(const atl::VariableVector<T> &expr) {
        atl::Variable<T> ret = T(0.0);
        size_t n = expr.GetSize();
        for (size_t i = 0; i < n; i++) {
            ret += expr(i) * expr(i);
        }
        return ret;
    }

    template<class T>
    inline const atl::Variable<T> Sum(const atl::VariableVector<T> &expr) {
        atl::Variable<T> ret = T(0.0);
        size_t n = expr.GetSize();
        for (size_t i = 0; i < n; i++) {
            ret += expr(i);
        }
        return ret;
    }

    template<class T>
    inline const atl::Variable<T> Product(const atl::VariableVector<T> &expr) {
        atl::Variable<T> ret = T(1.0);
        size_t n = expr.GetSize();
        for (size_t i = 0; i < n; i++) {
            ret *= expr(i);
        }
        return ret;
    }

    template<class T>
    inline const atl::Variable<T> Average(const atl::VariableVector<T> &expr) {
        atl::Variable<T> ret = atl::Sum(expr);
        size_t n = expr.GetSize();
        if (n > 1) {
            ret /= T(n);
        }
        return ret;
    }

    template<typename REAL_T>
    std::ostream& operator<<(std::ostream& out, const VariableVector<REAL_T>& m) {
        for (int i = 0; i < m.GetRows(); i++) {
            for (int j = 0; j < m.GetColumns(); j++) {
                out << m.data_m[i * m.GetColumns() + j].GetValue() << " ";
            }
            std::cout << std::endl;
        }

        return out;
    }

    //    /**
    //     * RealVector is a 1 x M RealMatrix.
    //     */
    //    template<typename T>
    //    struct RealVector : RealMatrix<T> {
    //        using RealMatrix<T>::operator=;
    //
    //        /**
    //         * Constructor.
    //         * @param columns
    //         */
    //        RealVector(size_t columns = 0) :
    //        RealMatrix<T>(1, columns) {
    //        }
    //
    //        /**
    //         * Sets the size of this vector.
    //         *
    //         * @param size
    //         */
    //        void SetSize(size_t size) {
    //            this->columns = size;
    //            this->data_m.resize(size);
    //        }
    //
    //        /**
    //         * Returns the size of this vector.
    //         * @return
    //         */
    //        size_t GetSize() {
    //            return this->columns;
    //        }
    //
    //        const std::string ToExpressionTemplateString() const {
    //            std::stringstream ss;
    //            ss << "atl::RealVector<T>";
    //            return ss.str();
    //        }
    //
    //
    //    };

}


#endif /* VECTOR_HPP */

