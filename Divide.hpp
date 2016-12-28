/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Division.hpp
 * Author: matthewsupernaw
 *
 * Created on November 3, 2016, 10:43 AM
 */

#ifndef DIVISION_HPP
#define DIVISION_HPP
#include "Expression.hpp"
#include <cmath>
namespace atl {

    template <class REAL_T, class LHS, class RHS>
    struct Divide : public ExpressionBase<REAL_T, Divide<REAL_T, LHS, RHS> > {
        typedef REAL_T BASE_TYPE;

        Divide(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {

        }

        Divide(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        Divide(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;
        }

        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() /rhs_m.GetValue();
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    return lhs_m.GetValue() / rhs_m.GetValue();
                } else {//scalar/matrix multiply
                    return lhs_m.GetValue() / rhs_m.GetValue(i, j);
                }
            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
                return lhs_m.GetValue(i, j) / rhs_m.GetValue();
            } else { //matrix multiply

                throw std::invalid_argument("matrix-matrix division not available.");

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
            return (rhs_m.GetValue() * lhs_m.EvaluateDerivative(id) -
                    rhs_m.EvaluateDerivative(id) * lhs_m.GetValue()) / (rhs_m.GetValue() * rhs_m.GetValue());
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            REAL_T lhs_value_m = lhs_m.GetValue();
            REAL_T rhs_value_m = rhs_m.GetValue();
            REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(a) * rhs_m.EvaluateDerivative(b)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
            REAL_T bb = ((lhs_m.EvaluateDerivative(a) * rhs_m.EvaluateDerivative(b)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
            REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(a, b)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
            REAL_T dd = ((lhs_m.EvaluateDerivative(b) * rhs_m.EvaluateDerivative(a)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
            REAL_T ee = lhs_m.EvaluateDerivative(a, b) / rhs_value_m;
            return aa - bb - cc - dd + ee;
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            REAL_T lhs_value_m = lhs_m.GetValue();
            REAL_T rhs_value_m = rhs_m.GetValue();
            return -1.0 * (static_cast<REAL_T> (6.0) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, 4.0)+(2 * (lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+
                    (static_cast<REAL_T> (2) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+(static_cast<REAL_T> (2.0) * (lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
                    ((lhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+(static_cast<REAL_T> (2.0) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-((lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+
                    (static_cast<REAL_T> (2.0) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+(static_cast<REAL_T> (2.0) * (lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
                    ((lhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-((lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-(lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
                    ((lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x, y))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-((lhs_m.EvaluateDerivative(y, z))*(rhs_m.EvaluateDerivative(x))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0)) + lhs_m.EvaluateDerivative(x, y, z) / rhs_value_m;
        }

        inline REAL_T EvaluateDerivative(uint32_t a, size_t i, size_t j = 0) const {
#warning  Multiply.hpp : Line 70 needs review
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    return (rhs_m.GetValue() * lhs_m.EvaluateDerivative(a) -
                            rhs_m.EvaluateDerivative(a) * lhs_m.GetValue()) / (rhs_m.GetValue() * rhs_m.GetValue());
                } else {//scalar/matrix multiply
                    return (rhs_m.GetValue(i, j) * lhs_m.EvaluateDerivative(a) -
                            rhs_m.EvaluateDerivative(a, i, j) * lhs_m.GetValue()) / (rhs_m.GetValue(i, j) * rhs_m.GetValue(i, j));
                }
            } else if (rhs_m.IsScalar()) {//scalar/matrix divide
                return (rhs_m.GetValue() * lhs_m.EvaluateDerivative(a, i, j) -
                        rhs_m.EvaluateDerivative(a) * lhs_m.GetValue(i, j)) / (rhs_m.GetValue() * rhs_m.GetValue());
            } else { //matrix multiply

                throw std::invalid_argument("matrix-matrix division not available.");
            }
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    REAL_T lhs_value_m = lhs_m.GetValue();
                    REAL_T rhs_value_m = rhs_m.GetValue();
                    REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(a) * rhs_m.EvaluateDerivative(b)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
                    REAL_T bb = ((lhs_m.EvaluateDerivative(a) * rhs_m.EvaluateDerivative(b)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
                    REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(a, b)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
                    REAL_T dd = ((lhs_m.EvaluateDerivative(b) * rhs_m.EvaluateDerivative(a)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
                    REAL_T ee = lhs_m.EvaluateDerivative(a, b) / rhs_value_m;
                    return aa - bb - cc - dd + ee;
                } else {//scalar/matrix divide
                    REAL_T lhs_value_m = lhs_m.GetValue();
                    REAL_T rhs_value_m = rhs_m.GetValue(i, j);
                    REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(a, i, j) * rhs_m.EvaluateDerivative(b, i, j)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
                    REAL_T bb = ((lhs_m.EvaluateDerivative(a) * rhs_m.EvaluateDerivative(b, i, j)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
                    REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(a, b, i, j)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
                    REAL_T dd = ((lhs_m.EvaluateDerivative(b) * rhs_m.EvaluateDerivative(a, i, j)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
                    REAL_T ee = lhs_m.EvaluateDerivative(a, b) / rhs_value_m;
                    return aa - bb - cc - dd + ee;
                }
            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
                REAL_T lhs_value_m = lhs_m.GetValue(i, j);
                REAL_T rhs_value_m = rhs_m.GetValue();
                REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(a) * rhs_m.EvaluateDerivative(b)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
                REAL_T bb = ((lhs_m.EvaluateDerivative(a, i, j) * rhs_m.EvaluateDerivative(b)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
                REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(a, b)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
                REAL_T dd = ((lhs_m.EvaluateDerivative(b, i, j) * rhs_m.EvaluateDerivative(a)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
                REAL_T ee = lhs_m.EvaluateDerivative(a, b, i, j) / rhs_value_m;
                return aa - bb - cc - dd + ee;
            } else { //matrix multiply

                throw std::invalid_argument("matrix-matrix division not available.");
            }
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            //            if (lhs_m.IsScalar()) {//scalar divide
            //                if (rhs_m.IsScalar()) {//scalar divide
            //                    REAL_T lhs_value_m = lhs_m.GetValue();
            //                    REAL_T rhs_value_m = rhs_m.GetValue();
            //                    return -1.0 * (static_cast<REAL_T> (6.0) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, 4.0)+(2 * (lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+
            //                            (static_cast<REAL_T> (2) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+(static_cast<REAL_T> (2.0) * (lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
            //                            ((lhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+(static_cast<REAL_T> (2.0) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-((lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+
            //                            (static_cast<REAL_T> (2.0) * lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+(static_cast<REAL_T> (2.0) * (lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
            //                            ((lhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-((lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-(lhs_m.GetValue()*(rhs_m.EvaluateDerivative(x, y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
            //                            ((lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x, y))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-((lhs_m.EvaluateDerivative(y, z))*(rhs_m.EvaluateDerivative(x))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0)) + lhs_m.EvaluateDerivative(x, y, z) / rhs_value_m;
            //
            //                } else {//scalar/matrix divide
            //                    REAL_T lhs_value_m = lhs_m.GetValue();
            //                    REAL_T rhs_value_m = rhs_m.GetValue(i,j);
            //                    return -1.0 * (static_cast<REAL_T> (6.0) * 
            //                            lhs_m.GetValue()*
            //                            (rhs_m.EvaluateDerivative(x,i,j))*
            //                            (rhs_m.EvaluateDerivative(y,i,j))*
            //                            (rhs_m.EvaluateDerivative(z,i,j))) /
            //                            std::pow(rhs_value_m, 4.0)+
            //                            (2 * (lhs_m.EvaluateDerivative(x))*
            //                            (rhs_m.EvaluateDerivative(y,i,j))*
            //                            (rhs_m.EvaluateDerivative(z,i,j))) /
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+          
            //                            (static_cast<REAL_T> (2) * 
            //                            lhs_m.GetValue()*
            //                            (rhs_m.EvaluateDerivative(x, y,i,j))*
            //                            (rhs_m.EvaluateDerivative(z,i,j))) / 
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (3.0))
            //                            +(static_cast<REAL_T> (2.0) * 
            //                            (lhs_m.EvaluateDerivative(y))*
            //                            (rhs_m.EvaluateDerivative(x,i,j))*
            //                            (rhs_m.EvaluateDerivative(z,i,j))) / 
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
            //                            ((lhs_m.EvaluateDerivative(x, y))*
            //                            (rhs_m.EvaluateDerivative(z,i,j))) / 
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+
            //                            (static_cast<REAL_T> (2.0) * lhs_m.GetValue()*
            //                            (rhs_m.EvaluateDerivative(x,i,j))*
            //                            (rhs_m.EvaluateDerivative(y, z,i,j))) /
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
            //                            ((lhs_m.EvaluateDerivative(x))*
            //                            (rhs_m.EvaluateDerivative(y, z,i,j))) /
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+
            //                            (static_cast<REAL_T> (2.0) * 
            //                            lhs_m.GetValue()*
            //                            (rhs_m.EvaluateDerivative(x, z,i,j))*
            //                            (rhs_m.EvaluateDerivative(y,i,j))) / 
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+
            //                            (static_cast<REAL_T> (2.0) * 
            //                            (lhs_m.EvaluateDerivative(z))*
            //                            (rhs_m.EvaluateDerivative(x,i,j))*
            //                            (rhs_m.EvaluateDerivative(y,i,j))) /
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
            //                            ((lhs_m.EvaluateDerivative(x, z))*
            //                            (rhs_m.EvaluateDerivative(y,i,j))) / 
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
            //                            ((lhs_m.EvaluateDerivative(y))*
            //                            (rhs_m.EvaluateDerivative(x, z,i,j))) / 
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
            //                            (lhs_m.GetValue()*
            //                            (rhs_m.EvaluateDerivative(x, y, z,i,j))) /
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
            //                            ((lhs_m.EvaluateDerivative(z))*
            //                            (rhs_m.EvaluateDerivative(x, y,i,j))) / 
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
            //                            ((lhs_m.EvaluateDerivative(y, z))*
            //                            (rhs_m.EvaluateDerivative(x,i,j))) /
            //                            std::pow(rhs_value_m, static_cast<REAL_T> (2.0)) + 
            //                            lhs_m.EvaluateDerivative(x, y, z) / rhs_value_m;
            //
            //                }
            //            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
            //                return (lhs_m.EvaluateDerivative(x, y, i, j))*(rhs_m.EvaluateDerivative(z))+
            //                        (lhs_m.EvaluateDerivative(x, i, j))*(rhs_m.EvaluateDerivative(y, z))
            //                        +(lhs_m.EvaluateDerivative(x, z, i, j))*(rhs_m.EvaluateDerivative(y))
            //                        +(lhs_m.EvaluateDerivative(y, i, j))*(rhs_m.EvaluateDerivative(x, z))
            //                        + lhs_m.GetValue(i, j)*(rhs_m.EvaluateDerivative(x, y, z))+
            //                        (lhs_m.EvaluateDerivative(z, i, j))*(rhs_m.EvaluateDerivative(x, y))+
            //                        (lhs_m.EvaluateDerivative(y, z, i, j))*(rhs_m.EvaluateDerivative(x))
            //                        + rhs_m.GetValue()*(lhs_m.EvaluateDerivative(x, y, z, i, j));
            //            } else { //matrix multiply

            if (lhs_m.IsScalar() || rhs_m.IsScalar()) {
                REAL_T lhs_value_m = lhs_m.GetValue(i, j);
                REAL_T rhs_value_m = rhs_m.GetValue(i, j);
                return -1.0 * (static_cast<REAL_T> (6.0) *
                        lhs_value_m *
                        (rhs_m.EvaluateDerivative(x, i, j))*
                        (rhs_m.EvaluateDerivative(y, i, j))*
                        (rhs_m.EvaluateDerivative(z, i, j))) /
                        std::pow(rhs_value_m, 4.0)+
                        (2 * (lhs_m.EvaluateDerivative(x, i, j))*
                        (rhs_m.EvaluateDerivative(y, i, j))*
                        (rhs_m.EvaluateDerivative(z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+
                        (static_cast<REAL_T> (2) *
                        lhs_value_m *
                        (rhs_m.EvaluateDerivative(x, y, i, j))*
                        (rhs_m.EvaluateDerivative(z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (3.0))
                        +(static_cast<REAL_T> (2.0) *
                        (lhs_m.EvaluateDerivative(y, i, j))*
                        (rhs_m.EvaluateDerivative(x, i, j))*
                        (rhs_m.EvaluateDerivative(z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
                        ((lhs_m.EvaluateDerivative(x, y, i, j))*
                        (rhs_m.EvaluateDerivative(z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+
                        (static_cast<REAL_T> (2.0) * lhs_m.GetValue()*
                        (rhs_m.EvaluateDerivative(x, i, j))*
                        (rhs_m.EvaluateDerivative(y, z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
                        ((lhs_m.EvaluateDerivative(x, i, j))*
                        (rhs_m.EvaluateDerivative(y, z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+
                        (static_cast<REAL_T> (2.0) *
                        lhs_m.GetValue()*
                        (rhs_m.EvaluateDerivative(x, z, i, j))*
                        (rhs_m.EvaluateDerivative(y, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+
                        (static_cast<REAL_T> (2.0) *
                        (lhs_m.EvaluateDerivative(z, i, j))*
                        (rhs_m.EvaluateDerivative(x, i, j))*
                        (rhs_m.EvaluateDerivative(y, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
                        ((lhs_m.EvaluateDerivative(x, z, i, j))*
                        (rhs_m.EvaluateDerivative(y, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
                        ((lhs_m.EvaluateDerivative(y, i, j))*
                        (rhs_m.EvaluateDerivative(x, z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
                        (lhs_value_m *
                        (rhs_m.EvaluateDerivative(x, y, z, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
                        ((lhs_m.EvaluateDerivative(z, i, j))*
                        (rhs_m.EvaluateDerivative(x, y, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
                        ((lhs_m.EvaluateDerivative(y, z, i, j))*
                        (rhs_m.EvaluateDerivative(x, i, j))) /
                        std::pow(rhs_value_m, static_cast<REAL_T> (2.0)) +
                        lhs_m.EvaluateDerivative(x, y, z, i, j) / rhs_value_m;

            } else {

                throw std::invalid_argument("matrix-matrix division not available.");
            }
        }

        size_t GetColumns() const {
            if (!lhs_m.IsScalar() && !rhs_m.IsScalar()) {
                size_t lc = lhs_m.GetColumns();
                size_t rc = rhs_m.GetColumns();
                assert(lc == rc);
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
                assert(lc == rc);
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



        atl::Real<REAL_T> real_m;//used for operations involving real numbers
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
    inline const Divide<REAL_T, LHS, RHS> operator/(const ExpressionBase<REAL_T, LHS>& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Divide<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    template <class REAL_T, class LHS>
    inline const Divide<REAL_T, LHS, Real<REAL_T> > operator/(const ExpressionBase<REAL_T, LHS>& a,
            REAL_T b) {

        return Divide<REAL_T, LHS, Real<REAL_T> > (a.Cast(), b);
    }

    template <class REAL_T, class LHS, class RHS>
    inline const Divide<REAL_T, Real<REAL_T>, RHS> operator/(const REAL_T& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Divide<REAL_T, Real<REAL_T>, RHS > (a, b.Cast());
    }





}

#endif /* DIVISION_HPP */

