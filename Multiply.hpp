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




    //Function Pointers

    template <class REAL_T, class LHS, class RHS>
    inline REAL_T EvaluateMultiplyValueCase1(int i, int j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs) {
        size_t kmax = lhs.GetColumns();
        REAL_T ret = static_cast<REAL_T> (0.0);
        for (size_t k = 0; k < kmax; k++) {
            ret += lhs.GetValue(i, k) * rhs.GetValue(k, j);

        }
        return ret;
    }

    template <class REAL_T, class LHS, class RHS>
    REAL_T EvaluateMultiplyValueCase2(int i, int j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs) {

    }

    template <class REAL_T, class LHS, class RHS>
    REAL_T EvaluateMultiplyValueCase3(int i, int j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs) {

    }

    template <class REAL_T, class LHS, class RHS>
    REAL_T EvaluateMultiplyValueCase4(int i, int j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs) {

        return 1;
    }

    template <class REAL_T, class LHS, class RHS>
    inline REAL_T EvaluateMultiplyFirstDerivativeCase1(uint32_t a, size_t i, size_t j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs) {

        size_t kmax = lhs.GetColumns();
        REAL_T ret = static_cast<REAL_T> (0.0);

        for (size_t k = 0; k < kmax; k++) {
            ret += (lhs.GetValue(i, k) *
                    rhs.EvaluateDerivative(a, k, j) +
                    lhs.EvaluateDerivative(a, i, k) *
                    rhs.GetValue(k, j));

        }
        return ret;

    }

    template <class REAL_T, class LHS, class RHS>
    struct Multiply : public ExpressionBase<REAL_T, Multiply<REAL_T, LHS, RHS> > {
        //        typedef REAL_T(*GetValue_ptr)(int i, int j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs);
        //        typedef REAL_T(*FirstOrderDerivative_ptr)(uint32_t a, size_t i, size_t j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs);
        //        typedef REAL_T(*SecondOrderDerivative_ptr)(uint32_t a, uint32_t b, int i, int j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs);
        //        typedef REAL_T(*ThirdOrderDerivative_ptr)(uint32_t a, uint32_t b, uint32_t c, int i, int j, const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs);


        //        GetValue_ptr value_func;
        //        FirstOrderDerivative_ptr first_order_func;
        //        SecondOrderDerivative_ptr second_order_func;
        //        ThirdOrderDerivative_ptr third_order_func;

        bool mm_multiply = false;

        Multiply(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {
            if (!lhs_m.IsScalar() && !rhs_m.IsScalar()) {
                assert(lhs_m.GetRows() == rhs_m.GetColumns());
                mm_multiply = true;
            }

            //             value_func = &EvaluateMultiplyValueCase1<REAL_T, LHS, RHS>;
            //             first_order_func = &EvaluateMultiplyFirstDerivativeCase1<REAL_T, LHS, RHS>;
        }

        Multiply(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        Multiply(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;
        }

        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() * rhs_m.GetValue();
        }

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

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            lhs_m.PushIds(ids, i, j);
            rhs_m.PushIds(ids, i, j);
        }

        inline const REAL_T EvaluateDerivative(uint32_t id) const {
            return (lhs_m.GetValue() * rhs_m.EvaluateDerivative(id) +
                    lhs_m.EvaluateDerivative(id) * rhs_m.GetValue());
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return lhs_m.EvaluateDerivative(a) * rhs_m.EvaluateDerivative(b) +
                    lhs_m.GetValue() * rhs_m.EvaluateDerivative(a, b) +
                    lhs_m.EvaluateDerivative(b) * rhs_m.EvaluateDerivative(a) +
                    rhs_m.GetValue() * lhs_m.EvaluateDerivative(a, b);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return (lhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))+
                    (lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))
                    +(lhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))
                    +(lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x, z))
                    + lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, y, z))+
                    (lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x, y))+
                    (lhs_m.EvaluateDerivative(y, z))*(rhs_m.EvaluateDerivative(x))
                    + rhs_m.GetValue()*(lhs_m.EvaluateDerivative(x, y, z));
        }

        inline REAL_T EvaluateDerivative(uint32_t a, size_t i, size_t j = 0) const {

            ////            return first_order_func(a,i,j,lhs_m.Cast(), rhs_m.Cast());
            //            
            //#warning  Multiply.hpp : Line 70 needs review
            //            if (lhs_m.IsScalar()) {//scalar multiply
            //                if (rhs_m.IsScalar()) {//scalar multiply
            //                    return (lhs_m.GetValue() *
            //                            rhs_m.EvaluateDerivative(a) +
            //                            lhs_m.EvaluateDerivative(a) *
            //                            rhs_m.GetValue());
            //                } else {//scalar/matrix multiply
            //                    return (lhs_m.GetValue() * rhs_m.EvaluateDerivative(a, i, j) +
            //                            lhs_m.EvaluateDerivative(a) * rhs_m.GetValue(i, j));
            //                }
            //            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
            //                return (lhs_m.GetValue(i, j) *
            //                        rhs_m.EvaluateDerivative(a) +
            //                        lhs_m.EvaluateDerivative(a, i, j) *
            //                        rhs_m.GetValue());
            if (!mm_multiply) {
                return (lhs_m.GetValue(i, j) *
                        rhs_m.EvaluateDerivative(a,i, j) +
                        lhs_m.EvaluateDerivative(a, i, j) *
                        rhs_m.GetValue(i, j));
            } else { //matrix multiply

                //                assert(lhs_m.GetRows() == rhs_m.GetColumns());

                size_t kmax = lhs_m.GetColumns();
                REAL_T ret = static_cast<REAL_T> (0.0);
                for (size_t k = 0; k < kmax; k++) {
                    ret += (lhs_m.GetValue(i, k) *
                            rhs_m.EvaluateDerivative(a, k, j) +
                            lhs_m.EvaluateDerivative(a, i, k) *
                            rhs_m.GetValue(k, j));

                }
                return ret;
            }
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    return lhs_m.EvaluateDerivative(a) *
                            rhs_m.EvaluateDerivative(b) +
                            lhs_m.GetValue() *
                            rhs_m.EvaluateDerivative(a, b) +
                            lhs_m.EvaluateDerivative(b) *
                            rhs_m.EvaluateDerivative(a) +
                            rhs_m.GetValue() *
                            lhs_m.EvaluateDerivative(a, b);
                } else {//scalar/matrix multiply
                    return lhs_m.EvaluateDerivative(a) *
                            rhs_m.EvaluateDerivative(a, i, j) +
                            lhs_m.GetValue() *
                            rhs_m.EvaluateDerivative(a, b, i, j) +
                            lhs_m.EvaluateDerivative(b) *
                            rhs_m.EvaluateDerivative(a, i, j) +
                            rhs_m.GetValue(i, j) *
                            lhs_m.EvaluateDerivative(a, b);
                }
            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
                return lhs_m.EvaluateDerivative(a, i, j) *
                        rhs_m.EvaluateDerivative(b) + lhs_m.GetValue(i, j) *
                        rhs_m.EvaluateDerivative(a, b) +
                        lhs_m.EvaluateDerivative(b, i, j) *
                        rhs_m.EvaluateDerivative(a) + rhs_m.GetValue() *
                        lhs_m.EvaluateDerivative(a, b, i, j);
            } else { //matrix multiply

                assert(lhs_m.GetRows() == rhs_m.GetColumns());

                size_t kmax = lhs_m.GetColumns();
                REAL_T ret = static_cast<REAL_T> (0.0);
                for (size_t k = 0; k < kmax; k++) {
                    ret += lhs_m.EvaluateDerivative(a, i, k) *
                            rhs_m.EvaluateDerivative(b, k, j) +
                            lhs_m.GetValue(i, k) *
                            rhs_m.EvaluateDerivative(a, b, k, j) +
                            lhs_m.EvaluateDerivative(b, i, k) *
                            rhs_m.EvaluateDerivative(a, k, j) +
                            rhs_m.GetValue(k, j) *
                            lhs_m.EvaluateDerivative(a, b, i, k);

                }
                return ret;
            }
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    return (lhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))+
                            (lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))
                            +(lhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))
                            +(lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x, z))
                            + lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, y, z))+
                            (lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x, y))+
                            (lhs_m.EvaluateDerivative(y, z))*(rhs_m.EvaluateDerivative(x))
                            + rhs_m.GetValue()*(lhs_m.EvaluateDerivative(x, y, z));
                } else {//scalar - matrix multiply
                    return (lhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z, i, j))+
                            (lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z, i, j))
                            +(lhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y, i, j))
                            +(lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x, z, i, j))
                            + lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, y, z, i, j))+
                            (lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x, y, i, j))+
                            (lhs_m.EvaluateDerivative(y, z))*(rhs_m.EvaluateDerivative(x, i, j))
                            + rhs_m.GetValue(i, j)*(lhs_m.EvaluateDerivative(x, y, z));
                }
            } else if (rhs_m.IsScalar()) {//scalar - matrix multiply
                return (lhs_m.EvaluateDerivative(x, y, i, j))*(rhs_m.EvaluateDerivative(z))+
                        (lhs_m.EvaluateDerivative(x, i, j))*(rhs_m.EvaluateDerivative(y, z))
                        +(lhs_m.EvaluateDerivative(x, z, i, j))*(rhs_m.EvaluateDerivative(y))
                        +(lhs_m.EvaluateDerivative(y, i, j))*(rhs_m.EvaluateDerivative(x, z))
                        + lhs_m.GetValue(i, j)*(rhs_m.EvaluateDerivative(x, y, z))+
                        (lhs_m.EvaluateDerivative(z, i, j))*(rhs_m.EvaluateDerivative(x, y))+
                        (lhs_m.EvaluateDerivative(y, z, i, j))*(rhs_m.EvaluateDerivative(x))
                        + rhs_m.GetValue()*(lhs_m.EvaluateDerivative(x, y, z, i, j));
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

        bool IsScalar() const {
            if (lhs_m.IsScalar() && rhs_m.IsScalar()) {
                return true;
            }
            return false;
        }



        atl::Real<REAL_T> real_m; //used for operations involving real numbers
        const LHS& lhs_m;
        const RHS& rhs_m;
        REAL_T value_m;
    };

    
    
    /**
     * Operator for addition of two expression templates.
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS, class RHS>
    inline const Multiply<REAL_T, LHS, RHS> operator*(const ExpressionBase<REAL_T, LHS>& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Multiply<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    template <class REAL_T, class LHS>
    inline const Multiply<REAL_T, LHS, Real<REAL_T> > operator*(const ExpressionBase<REAL_T, LHS>& a,
            REAL_T b) {

        return Multiply<REAL_T, LHS, Real<REAL_T> > (a.Cast(), b);
    }

    template <class REAL_T, class LHS, class RHS>
    inline const Multiply<REAL_T, Real<REAL_T>, RHS> operator*(const REAL_T& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Multiply<REAL_T, Real<REAL_T>, RHS > (a, b.Cast());
    }





}


#endif /* MULTIPLY_HPP */

