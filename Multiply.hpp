/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Multiply.hpp
 * Author: matthewsupernaw
 *
 * Created on November 3, 2016, 9:13 AM
 */

#ifndef MULTIPLY_HPP
#define MULTIPLY_HPP

#include "Expression.hpp"

#include <vector>
#include <set>
#include "Real.hpp"


namespace atl {

    /**
     * Expression template to handle multiplication.
     * 
     * \f$ f(x) * g(x) \f$
     * 
     * or
     * 
     * \f$ f_{i,j}(x) * g_{i,j}(x) \f$
     * 
     */
    template <class REAL_T, class LHS, class RHS>
    struct Multiply : public ExpressionBase<REAL_T, Multiply<REAL_T, LHS, RHS> > {
        bool mm_multiply = false;

        Multiply(const Multiply<REAL_T, LHS, RHS>& other) :
        mm_multiply(other.mm_multiply), real_m(other.real_m), lhs_m(other.lhs_m), rhs_m(other.rhs_m), value_m(other.value_m) {
        }

        /**
         * Constructor for Variable types.
         * 
         * @param lhs
         * @param rhs
         */
        Multiply(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs, bool element_product = false)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {


            if (!lhs_m.IsScalar() && !rhs_m.IsScalar()) {

                if (element_product == true) {
                    this->mm_multiply = false;
                } else {

                    if (lhs_m.GetRows() != rhs_m.GetColumns()) {//check for vector element multiply
                        assert(lhs_m.GetRows() == 1);
                        assert(rhs_m.GetRows() == 1);
                        mm_multiply = false;
                    } else {
                        assert(lhs_m.GetRows() == rhs_m.GetColumns());
                        mm_multiply = true;
                    }

                }
            }
        }

        /**
         * Constructor for a real and Variable type.
         * 
         * @param lhs
         * @param rhs
         */
        Multiply(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        /**
         * Constructor for Variable and real type.
         * 
         * @param lhs
         * @param rhs
         */
        Multiply(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T & rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;
        }

        /**
         * Compute the value of this expression. 
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() * rhs_m.GetValue();
        }

        /**
         * Compute the value of this expression at index {i,j}.
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            if (!mm_multiply) {
                return lhs_m.GetValue(i, j) * rhs_m.GetValue(i, j);
            } else { //matrix multiply


                size_t kmax = lhs_m.GetColumns();
                REAL_T ret = static_cast<REAL_T> (0.0);
                for (size_t k = 0; k < kmax; k++) {
                    ret += lhs_m.GetValue(i, k) * rhs_m.GetValue(k, j);

                }
                return ret;

            }
        }

        /**
         * Returns true if the left or right side is nonlinear, else
         * false.
         * @return 
         */
        bool IsNonlinear()const {
            if (lhs_m.IsNonlinear() || rhs_m.IsNonlinear()) {
                return true;
            } else {
                return false;
            }
        }

        /**
         * Push variable info into a set. 
         * 
         * @param ids
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage & ids)const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        /**
         *  Push variable info into a set at index {i,j}. 
         * 
         * @param ids
         * @param i
         * @param j
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            lhs_m.PushIds(ids, i, j);
            rhs_m.PushIds(ids, i, j);
        }

        inline void PushNLIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, bool nl = false)const {
            lhs_m.PushNLIds(ids, nl);
            rhs_m.PushNLIds(ids, nl);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return lhs_m.ComplexEvaluate(x, h) * rhs_m.ComplexEvaluate(x, h);
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            REAL_T ret = static_cast<REAL_T> (0.0);
            if (degree == 0) {
                return lhs_m.Taylor(0) * rhs_m.Taylor(0);
            }

            for (int i = 0; i <= degree; i++) {
                ret += lhs_m.Taylor(degree - i) * rhs_m.Taylor(i);
            }

            return ret;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return (lhs_m.ToDynamic() * rhs_m.ToDynamic());
        }

        /**
         * Evaluates the derivative of this expression with respect to x.
         * 
         * \f$f\left(x\right)\,\left({{d}\over{d\,x}}\,g\left(x\right)\right)+g\left(x\right)\,\left({{d}\over{d\,x}}\,f\left(x\right)\right)\f$
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateFirstDerivative(uint32_t x) const {
            return (lhs_m.GetValue() * rhs_m.EvaluateFirstDerivative(x) +
                    lhs_m.EvaluateFirstDerivative(x) * rhs_m.GetValue());
        }

        /**
         * 
         * Evaluates the second-order mixed partial derivative of this expression 
         * with respect to x and y.
         * 
         * \f${{d}\over{d\,x}}\,f\left(x , y , z\right)\,\left({{d}\over{d\,y}}\,
         * g\left(x , y , z\right)\right)+f\left(x , y , z\right)\,\left({{d^2
         * }\over{d\,x\,d\,y}}\,g\left(x , y , z\right)\right)+{{d}\over{d\,y}}
         * \,f\left(x , y , z\right)\,\left({{d}\over{d\,x}}\,g\left(x , y , z
         * \right)\right)+g\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d\,y
         * }}\,f\left(x , y , z\right)\right)\f$
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateSecondDerivative(uint32_t x, uint32_t y) const {
            return lhs_m.EvaluateFirstDerivative(x) * rhs_m.EvaluateFirstDerivative(y) +
                    lhs_m.GetValue() * rhs_m.EvaluateSecondDerivative(x, y) +
                    lhs_m.EvaluateFirstDerivative(y) * rhs_m.EvaluateFirstDerivative(x) +
                    rhs_m.GetValue() * lhs_m.EvaluateSecondDerivative(x, y);
        }

        /**
         * Evaluates the third-order mixed partial derivative of this expression 
         * with respect to x, y, and z.
         * 
         * \f$ {{d^2}\over{d\,x\,d\,y}}\,f\left(x , y , z\right)\,\left({{d}\over{
         * d\,z}}\,g\left(x , y , z\right)\right)+{{d}\over{d\,x}}\,f\left(x , 
         * y , z\right)\,\left({{d^2}\over{d\,y\,d\,z}}\,g\left(x , y , z
         * \right)\right)+{{d^2}\over{d\,x\,d\,z}}\,f\left(x , y , z\right)\,
         * \left({{d}\over{d\,y}}\,g\left(x , y , z\right)\right)+{{d}\over{d\,
         * y}}\,f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d\,z}}\,g
         * \left(x , y , z\right)\right)+ \\ f\left(x , y , z\right)\,\left({{d^3
         * }\over{d\,x\,d\,y\,d\,z}}\,g\left(x , y , z\right)\right)+{{d}\over{
         * d\,z}}\,f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d\,y}}\,g
         * \left(x , y , z\right)\right)+{{d^2}\over{d\,y\,d\,z}}\,f\left(x , y
         * , z\right)\,\left({{d}\over{d\,x}}\,g\left(x , y , z\right)\right)+
         * g\left(x , y , z\right)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f
         * \left(x , y , z\right)\right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateThirdDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return (lhs_m.EvaluateSecondDerivative(x, y))*(rhs_m.EvaluateFirstDerivative(z))+
                    (lhs_m.EvaluateFirstDerivative(x))*(rhs_m.EvaluateSecondDerivative(y, z))
                    +(lhs_m.EvaluateSecondDerivative(x, z))*(rhs_m.EvaluateFirstDerivative(y))
                    +(lhs_m.EvaluateFirstDerivative(y))*(rhs_m.EvaluateSecondDerivative(x, z))
                    + lhs_m.GetValue()*(rhs_m.EvaluateThirdDerivative(x, y, z))+
                    (lhs_m.EvaluateFirstDerivative(z))*(rhs_m.EvaluateSecondDerivative(x, y))+
                    (lhs_m.EvaluateSecondDerivative(y, z))*(rhs_m.EvaluateFirstDerivative(x))
                    + rhs_m.GetValue()*(lhs_m.EvaluateThirdDerivative(x, y, z));
        }

        /**
         * Evaluates the derivative of this expression
         * with respect to x at index {i,j}. 
         * 
         * \f$f_{i,j}(x)\,\left({{d}\over{d\,x}}\,g_{i,j}(x)\right)+g_{i,j}(x)\,
         * \left({{d}\over{d\,x}}\,f_{i,j}(x)\right)\f$
         * 
         * @param x
         * @return 
         */
        inline REAL_T EvaluateFirstDerivativeAt(uint32_t x, size_t i, size_t j = 0) const {

            ////            return first_order_func(a,i,j,lhs_m.Cast(), rhs_m.Cast());
            //            
            //#warning  Multiply.hpp : Line 70 needs review
            //            if (lhs_m.IsScalar()) {//scalar multiply
            //                if (rhs_m.IsScalar()) {//scalar multiply
            //                    return (lhs_m.GetValue() *
            //                            rhs_m.EvaluateFirstDerivative(a) +
            //                            lhs_m.EvaluateFirstDerivative(a) *
            //                            rhs_m.GetValue());
            //                } else {//scalar/matrix multiply
            //                    return (lhs_m.GetValue() * rhs_m.EvaluateFirstDerivativeAt(a, i, j) +
            //                            lhs_m.EvaluateFirstDerivative(a) * rhs_m.GetValue(i, j));
            //                }
            //            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
            //                return (lhs_m.GetValue(i, j) *
            //                        rhs_m.EvaluateFirstDerivative(a) +
            //                        lhs_m.EvaluateFirstDerivativeAt(a, i, j) *
            //                        rhs_m.GetValue());
            if (!mm_multiply) {
                return (lhs_m.GetValue(i, j) *
                        rhs_m.EvaluateFirstDerivativeAt(x, i, j) +
                        lhs_m.EvaluateFirstDerivativeAt(x, i, j) *
                        rhs_m.GetValue(i, j));
            } else { //matrix multiply

                //                assert(lhs_m.GetRows() == rhs_m.GetColumns());

                size_t kmax = lhs_m.GetColumns();
                REAL_T ret = static_cast<REAL_T> (0.0);
                for (size_t k = 0; k < kmax; k++) {
                    ret += (lhs_m.GetValue(i, k) *
                            rhs_m.EvaluateDerivative(x, k, j) +
                            lhs_m.EvaluateDerivative(x, i, k) *
                            rhs_m.GetValue(k, j));

                }
                return ret;
            }
        }

        /**
         * Evaluates the second-order mixed partial derivative of this expression with respect to x and y at 
         * index {i,j}.
         * 
         * \f$ {{d}\over{d\,x}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y
         * )\right)+f_{i,j}(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,g_{i,j}(x,y)
         * \right)+{{d}\over{d\,y}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,x}}\,g_{i
         * ,j}(x,y)\right)+g_{i,j}(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,f_{i,j
         * }(x,y)\right) \f$
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateSecondDerivativeAt(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    return lhs_m.EvaluateFirstDerivative(x) *
                            rhs_m.EvaluateFirstDerivative(y) +
                            lhs_m.GetValue() *
                            rhs_m.EvaluateSecondDerivative(x, y) +
                            lhs_m.EvaluateFirstDerivative(y) *
                            rhs_m.EvaluateFirstDerivative(x) +
                            rhs_m.GetValue() *
                            lhs_m.EvaluateSecondDerivative(x, y);
                } else {//scalar/matrix multiply
                    return lhs_m.EvaluateFirstDerivative(x) *
                            rhs_m.EvaluateFirstDerivativeAt(x, i, j) +
                            lhs_m.GetValue() *
                            rhs_m.EvaluateSecondDerivativeAt(x, y, i, j) +
                            lhs_m.EvaluateFirstDerivative(y) *
                            rhs_m.EvaluateFirstDerivativeAt(x, i, j) +
                            rhs_m.GetValue(i, j) *
                            lhs_m.EvaluateSecondDerivative(x, y);
                }
            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
                return lhs_m.EvaluateFirstDerivativeAt(x, i, j) *
                        rhs_m.EvaluateFirstDerivative(y) + lhs_m.GetValue(i, j) *
                        rhs_m.EvaluateSecondDerivative(x, y) +
                        lhs_m.EvaluateFirstDerivativeAt(y, i, j) *
                        rhs_m.EvaluateFirstDerivative(x) + rhs_m.GetValue() *
                        lhs_m.EvaluateSecondDerivativeAt(x, y, i, j);
            } else { //matrix multiply

                assert(lhs_m.GetRows() == rhs_m.GetColumns());

                size_t kmax = lhs_m.GetColumns();
                REAL_T ret = static_cast<REAL_T> (0.0);
                for (size_t k = 0; k < kmax; k++) {
                    ret += lhs_m.EvaluateDerivative(x, i, k) *
                            rhs_m.EvaluateDerivative(y, k, j) +
                            lhs_m.GetValue(i, k) *
                            rhs_m.EvaluateDerivative(x, y, k, j) +
                            lhs_m.EvaluateDerivative(y, i, k) *
                            rhs_m.EvaluateDerivative(x, k, j) +
                            rhs_m.GetValue(k, j) *
                            lhs_m.EvaluateDerivative(x, y, i, k);

                }
                return ret;
            }
        }

        /**
         * Evaluates the third-order mixed partial derivative of this expression 
         * with respect to x, y, and z at index {i,j}.
         * 
         * \f$ {{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,g
         *  _{i,j}(x,y,z)\right)+{{d}\over{d\,x}}\,f_{i,j}(x,y,z)\,\left({{d^2
         *  }\over{d\,y\,d\,z}}\,g_{i,j}(x,y,z)\right)+{{d^2}\over{d\,x\,d\,z}}
         *  \,f_{i,j}(x,y,z)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y,z)\right)+{{d
         *  }\over{d\,y}}\,f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,g_{i,
         *  j}(x,y,z)\right)+ \\ f_{i,j}(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z
         *  }}\,g_{i,j}(x,y,z)\right)+{{d}\over{d\,z}}\,f_{i,j}(x,y,z)\,\left({{
         *  d^2}\over{d\,x\,d\,y}}\,g_{i,j}(x,y,z)\right)+{{d^2}\over{d\,y\,d\,z
         *  }}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y,z)\right)+g
         *  _{i,j}(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)
         *  \right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateThirdDerivativeAt(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    return (lhs_m.EvaluateSecondDerivative(x, y))*(rhs_m.EvaluateFirstDerivative(z))+
                            (lhs_m.EvaluateFirstDerivative(x))*(rhs_m.EvaluateSecondDerivative(y, z))
                            +(lhs_m.EvaluateSecondDerivative(x, z))*(rhs_m.EvaluateFirstDerivative(y))
                            +(lhs_m.EvaluateFirstDerivative(y))*(rhs_m.EvaluateSecondDerivative(x, z))
                            + lhs_m.GetValue()*(rhs_m.EvaluateThirdDerivative(x, y, z))+
                            (lhs_m.EvaluateFirstDerivative(z))*(rhs_m.EvaluateSecondDerivative(x, y))+
                            (lhs_m.EvaluateSecondDerivative(y, z))*(rhs_m.EvaluateFirstDerivative(x))
                            + rhs_m.GetValue()*(lhs_m.EvaluateThirdDerivative(x, y, z));
                } else {//scalar - matrix multiply
                    return (lhs_m.EvaluateSecondDerivative(x, y))*(rhs_m.EvaluateFirstDerivativeAt(z, i, j))+
                            (lhs_m.EvaluateFirstDerivative(x))*(rhs_m.EvaluateDerivative(y, z, i, j))
                            +(lhs_m.EvaluateSecondDerivative(x, z))*(rhs_m.EvaluateFirstDerivativeAt(y, i, j))
                            +(lhs_m.EvaluateFirstDerivative(y))*(rhs_m.EvaluateSecondDerivativeAt(x, z, i, j))
                            + lhs_m.GetValue()*(rhs_m.EvaluateThirdDerivativeAt(x, y, z, i, j))+
                            (lhs_m.EvaluateFirstDerivative(z))*(rhs_m.EvaluateSecondDerivativeAt(x, y, i, j))+
                            (lhs_m.EvaluateSecondDerivative(y, z))*(rhs_m.EvaluateFirstDerivativeAt(x, i, j))
                            + rhs_m.GetValue(i, j)*(lhs_m.EvaluateThirdDerivative(x, y, z));
                }
            } else if (rhs_m.IsScalar()) {//scalar - matrix multiply
                return (lhs_m.EvaluateSecondDerivativeAt(x, y, i, j))*(rhs_m.EvaluateFirstDerivative(z))+
                        (lhs_m.EvaluateFirstDerivativeAt(x, i, j))*(rhs_m.EvaluateSecondDerivative(y, z))
                        +(lhs_m.EvaluateSecondDerivativeAt(x, z, i, j))*(rhs_m.EvaluateFirstDerivative(y))
                        +(lhs_m.EvaluateFirstDerivativeAt(y, i, j))*(rhs_m.EvaluateSecondDerivative(x, z))
                        + lhs_m.GetValue(i, j)*(rhs_m.EvaluateThirdDerivative(x, y, z))+
                        (lhs_m.EvaluateFirstDerivativeAt(z, i, j))*(rhs_m.EvaluateSecondDerivative(x, y))+
                        (lhs_m.EvaluateDerivative(y, z, i, j))*(rhs_m.EvaluateFirstDerivative(x))
                        + rhs_m.GetValue()*(lhs_m.EvaluateThirdDerivativeAt(x, y, z, i, j));
            } else { //matrix multiply

                assert(lhs_m.GetRows() == rhs_m.GetColumns());

                size_t kmax = lhs_m.GetColumns();
                REAL_T ret = static_cast<REAL_T> (0.0);
#pragma unroll
                for (size_t k = 0; k < kmax; k++) {
                    ret += (lhs_m.EvaluateDerivative(x, y, i, k))*(rhs_m.EvaluateDerivative(z, k, j))+
                            (lhs_m.EvaluateDerivative(x, i, k))*(rhs_m.EvaluateDerivative(y, z, k, j))
                            +(lhs_m.EvaluateDerivative(x, z, i, k))*(rhs_m.EvaluateDerivative(y, k, j))
                            +(lhs_m.EvaluateDerivative(y, i, k))*(rhs_m.EvaluateDerivative(x, z, k, j))
                            + lhs_m.GetValue(i, k)*(rhs_m.EvaluateDerivative(x, y, z, k, j))+
                            (lhs_m.EvaluateDerivative(z, i, k))*(rhs_m.EvaluateDerivative(x, y, k, j))+
                            (lhs_m.EvaluateDerivative(y, z, i, k))*(rhs_m.EvaluateDerivative(x, k, j))
                            + rhs_m.GetValue(k, j)*(lhs_m.EvaluateDerivative(x, y, z, i, k));

                }
                return ret;
            }
        }

        /**
         * Return the number of columns.
         * 
         * @return 
         */
        size_t GetColumns() const {
            if (!lhs_m.IsScalar() && !rhs_m.IsScalar()) {
                size_t lc = lhs_m.GetColumns();
                size_t rc = rhs_m.GetColumns();
                return lc < rc ? lc : rc;
            } else if (!lhs_m.IsScalar()) {
                return lhs_m.GetColumns();
            } else {
                return rhs_m.GetColumns();
            }
        }

        /**
         * Return the number of rows. 
         * 
         * @return 
         */
        size_t GetRows() const {
            if (!lhs_m.IsScalar() && !rhs_m.IsScalar()) {
                size_t lc = lhs_m.GetRows();
                size_t rc = rhs_m.GetRows();
                return lc < rc ? lc : rc;
            } else if (!lhs_m.IsScalar()) {
                return lhs_m.GetRows();
            } else {
                return rhs_m.GetRows();
            }
        }

        /**
         * True if this expression is a scalar.
         * 
         * @return 
         */
        bool IsScalar() const {
            if (lhs_m.IsScalar() && rhs_m.IsScalar()) {
                return true;
            }
            return false;
        }

        /**
         * Create a string representation of this expression template. 
         * @return 
         */
        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::Multiply<T," << lhs_m.ToExpressionTemplateString() << ", " << rhs_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }

        atl::Real<REAL_T> real_m; //used for operations involving real numbers
        const LHS& lhs_m;
        const RHS& rhs_m;
        REAL_T value_m;
    };

    /**
     * Operator for multiplication of two expression templates.
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS, class RHS>
    inline const Multiply<REAL_T, LHS, RHS> operator*(const ExpressionBase<REAL_T, LHS>& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Multiply<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    /**
     * element product.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS, class RHS>
    inline const Multiply<REAL_T, LHS, RHS> ElementProduct(const ExpressionBase<REAL_T, LHS>& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Multiply<REAL_T, LHS, RHS > (a.Cast(), b.Cast(), true);
    }

    /**
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS>
    inline const Multiply<REAL_T, LHS, Real<REAL_T> > operator*(const ExpressionBase<REAL_T, LHS>& a,
            REAL_T b) {

        return Multiply<REAL_T, LHS, Real<REAL_T> > (a.Cast(), b);
    }

    /**
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class RHS>
    inline const Multiply<REAL_T, Real<REAL_T>, RHS> operator*(const REAL_T& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Multiply<REAL_T, Real<REAL_T>, RHS > (a, b.Cast());
    }


}


#endif /* MULTIPLY_HPP */


