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

    /**
     * Expression template to handle 
     * 
     * \f$ f(x) / g(x) \f$
     * 
     * or
     * 
     * \f$ f_{i,j}(x) / g_{i,j}(x) \f$
     */
    template <class REAL_T, class LHS, class RHS>
    struct Divide : public ExpressionBase<REAL_T, Divide<REAL_T, LHS, RHS> > {
        typedef REAL_T BASE_TYPE;

        Divide(const Divide<REAL_T, LHS, RHS>& other) :
        real_m(other.real_m), lhs_m(other.lhs_m.Cast()), rhs_m(other.rhs_m.Cast()), value_m(other.value_m), val_(other.val_) {
        }

        /**
         * Constructor for two variable types.
         * 
         * @param lhs
         * @param rhs
         */
        Divide(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {

        }

        /**
         * Constructor for a real divided by a variable type.
         * 
         * @param lhs
         * @param rhs
         */
        Divide(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        /**
         * Constructor for a variable divided by a real type.
         * 
         * @param lhs
         * @param rhs
         */
        Divide(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;

        }

        /**
         * Compute the division of lhs by rhs.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() / rhs_m.GetValue();
        }

        /**
         * Compute the division of the lhs and rhs expressions at index {i,j}.
         * 
         * @return 
         */
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
         *  Push variable info into a set. 
         * 
         * @param ids
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        /**
         * Push variable info into a set at index {i,j}.
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
            return lhs_m.ComplexEvaluate(x, h) / rhs_m.ComplexEvaluate(x, h);
        }

        inline const REAL_T Taylor(uint32_t degree) const {

            if (degree == 0) {
                val_.reserve(5);
                val_.resize(1);

                val_[0] = lhs_m.Taylor(degree) / rhs_m.Taylor(degree);

                return val_[0];
            }

            size_t l = val_.size();
            val_.resize(degree + 1);



            for (int i = l; i <= degree; i++) {
                val_[i] = lhs_m.Taylor(i);
                for (int k = 1; k <= degree; k++) {
                    val_[i] -= val_[degree - k] * rhs_m.Taylor(k);
                }
                val_[i] /= rhs_m.Taylor(0);
            }

            return val_[degree];
        }

        /**
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f(x)}\over{g(x)}}-{{f(x)\,
         * \left({{d}\over{d\,x}}\,g(x)\right)}\over{g(x)^2}} \f$
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {
            return (rhs_m.GetValue() * lhs_m.EvaluateDerivative(x) -
                    rhs_m.EvaluateDerivative(x) * lhs_m.GetValue()) / (rhs_m.GetValue() * rhs_m.GetValue());
        }

        /**
         * Evaluates the second-order derivative with respect to x and y.
         * 
         * 
         * \f$ {{2\,f(x,y)\,\left({{d}\over{d\,x}}\,g(x,y)\right)\,
         *  \left({{d}\over{d\,y}}\,g(x,y)\right)}\over{g(x,y)^3}}-
         *  {{{{d}\over{d\,x}}\,f(x,y)\,\left({{d}\over{d\,y}}\,g(x,
         *  y)\right)}\over{g(x,y)^2}}- \\ {{f(x,y)\,\left({{d^2}\over{d
         *  \,x\,d\,y}}\,g(x,y)\right)}\over{g(x,y)^2}}-{{{{d}\over{
         *  d\,y}}\,f(x,y)\,\left({{d}\over{d\,x}}\,g(x,y)\right)
         *  }\over{g(x,y)^2}}+{{{{d^2}\over{d\,x\,d\,y}}\,f(x,y)
         *  }\over{g(x,y)}} \f$
         * 
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T lhs_value_m = lhs_m.GetValue();
            REAL_T rhs_value_m = rhs_m.GetValue();
            REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(x) * rhs_m.EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
            REAL_T bb = ((lhs_m.EvaluateDerivative(x) * rhs_m.EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
            REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(x, y)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
            REAL_T dd = ((lhs_m.EvaluateDerivative(y) * rhs_m.EvaluateDerivative(x)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
            REAL_T ee = lhs_m.EvaluateDerivative(x, y) / rhs_value_m;
            return aa - bb - cc - dd + ee;
        }

        /**
         * 
         * Evaluates the third-order derivative with respect to x, y, and z.
         * 
         * \f$ -{{6\,f\left(x , y , z\right)\,\left({{d}\over{d\,x}}\,g\left(x , y
         * , z\right)\right)\,\left({{d}\over{d\,y}}\,g\left(x , y , z\right)
         * \right)\,\left({{d}\over{d\,z}}\,g\left(x , y , z\right)\right)
         * }\over{g^4\left(x , y , z\right)}}+{{2\,\left({{d}\over{d\,x}}\,f
         * \left(x , y , z\right)\right)\,\left({{d}\over{d\,y}}\,g\left(x , y
         * , z\right)\right)\,\left({{d}\over{d\,z}}\,g\left(x , y , z\right)
         * \right)}\over{g^3\left(x , y , z\right)}}+{{2\,f\left(x , y , z
         * \right)\,\left({{d^2}\over{d\,x\,d\,y}}\,g\left(x , y , z\right)
         * \right)\,\left({{d}\over{d\,z}}\,g\left(x , y , z\right)\right)
         * }\over{g^3\left(x , y , z\right)}}+ \\ {{2\,\left({{d}\over{d\,y}}\,f
         * \left(x , y , z\right)\right)\,\left({{d}\over{d\,x}}\,g\left(x , y
         * , z\right)\right)\,\left({{d}\over{d\,z}}\,g\left(x , y , z\right)
         * \right)}\over{g^3\left(x , y , z\right)}}-{{{{d^2}\over{d\,x\,d\,y}}
         * \,f\left(x , y , z\right)\,\left({{d}\over{d\,z}}\,g\left(x , y , z
         * \right)\right)}\over{g^2\left(x , y , z\right)}}+{{2\,f\left(x , y
         * , z\right)\,\left({{d}\over{d\,x}}\,g\left(x , y , z\right)\right)
         * \,\left({{d^2}\over{d\,y\,d\,z}}\,g\left(x , y , z\right)\right)
         * }\over{g^3\left(x , y , z\right)}}-{{{{d}\over{d\,x}}\,f\left(x , y
         * , z\right)\,\left({{d^2}\over{d\,y\,d\,z}}\,g\left(x , y , z\right)
         * \right)}\over{g^2\left(x , y , z\right)}}+{{2\,f\left(x , y , z
         * \right)\,\left({{d^2}\over{d\,x\,d\,z}}\,g\left(x , y , z\right)
         * \right)\,\left({{d}\over{d\,y}}\,g\left(x , y , z\right)\right)
         * }\over{g^3\left(x , y , z\right)}}+ \\ {{2\,\left({{d}\over{d\,z}}\,f
         * \left(x , y , z\right)\right)\,\left({{d}\over{d\,x}}\,g\left(x , y
         * , z\right)\right)\,\left({{d}\over{d\,y}}\,g\left(x , y , z\right)
         * \right)}\over{g^3\left(x , y , z\right)}}-{{{{d^2}\over{d\,x\,d\,z}}
         * \,f\left(x , y , z\right)\,\left({{d}\over{d\,y}}\,g\left(x , y , z
         * \right)\right)}\over{g^2\left(x , y , z\right)}}-{{{{d}\over{d\,y}}
         * \,f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d\,z}}\,g\left(x
         * , y , z\right)\right)}\over{g^2\left(x , y , z\right)}}-{{f\left(x
         * , y , z\right)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,g\left(x , y
         * , z\right)\right)}\over{g^2\left(x , y , z\right)}}-{{{{d}\over{d\,
         * z}}\,f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d\,y}}\,g
         * \left(x , y , z\right)\right)}\over{g^2\left(x , y , z\right)}}-{{{{
         * d^2}\over{d\,y\,d\,z}}\,f\left(x , y , z\right)\,\left({{d}\over{d\,
         * x}}\,g\left(x , y , z\right)\right)}\over{g^2\left(x , y , z\right)
         * }}+{{{{d^3}\over{d\,x\,d\,y\,d\,z}}\,f\left(x , y , z\right)}\over{g
         * \left(x , y , z\right)}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            REAL_T lhs_value_m = lhs_m.GetValue();
            REAL_T rhs_value_m = rhs_m.GetValue();
            return -1.0 * (static_cast<REAL_T> (6.0) * lhs_value_m * (rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, 4.0)+(2 * (lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+
                    (static_cast<REAL_T> (2) * lhs_value_m * (rhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+(static_cast<REAL_T> (2.0) * (lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
                    ((lhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+(static_cast<REAL_T> (2.0) * lhs_value_m * (rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-((lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))+
                    (static_cast<REAL_T> (2.0) * lhs_value_m * (rhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))+(static_cast<REAL_T> (2.0) * (lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (3.0))-
                    ((lhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-((lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-(lhs_value_m * (rhs_m.EvaluateDerivative(x, y, z))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-
                    ((lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x, y))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0))-((lhs_m.EvaluateDerivative(y, z))*(rhs_m.EvaluateDerivative(x))) / std::pow(rhs_value_m, static_cast<REAL_T> (2.0)) + lhs_m.EvaluateDerivative(x, y, z) / rhs_value_m;
        }

        /**
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f_{i,j}(x)}\over{g_{i,j}(x)}}-{{f_{i,j}(x)\,
         * \left({{d}\over{d\,x}}\,g_{i,j}(x)\right)}\over{g_{i,j}(x)^2}} \f$
         * 
         * @param x
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
#warning  Multiply.hpp : Line 70 needs review
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    return (rhs_m.GetValue() * lhs_m.EvaluateDerivative(x) -
                            rhs_m.EvaluateDerivative(x) * lhs_m.GetValue()) / (rhs_m.GetValue() * rhs_m.GetValue());
                } else {//scalar/matrix multiply
                    return (rhs_m.GetValue(i, j) * lhs_m.EvaluateDerivative(x) -
                            rhs_m.EvaluateDerivative(x, i, j) * lhs_m.GetValue()) / (rhs_m.GetValue(i, j) * rhs_m.GetValue(i, j));
                }
            } else if (rhs_m.IsScalar()) {//scalar/matrix divide
                return (rhs_m.GetValue() * lhs_m.EvaluateDerivative(x, i, j) -
                        rhs_m.EvaluateDerivative(x) * lhs_m.GetValue(i, j)) / (rhs_m.GetValue() * rhs_m.GetValue());
            } else { //matrix multiply

                throw std::invalid_argument("matrix-matrix division not available.");
            }
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return (lhs_m.ToDynamic() / rhs_m.ToDynamic());
        }

        /**
         * Evaluates the second-order derivative with respect to x and y at 
         * index {i,j}.
         * 
         * \f$ {{2\,f_{i,j}(x,y)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y)\right)\,
         *  \left({{d}\over{d\,y}}\,g_{i,j}(x,y)\right)}\over{g_{i,j}(x,y)^3}}-
         *  {{{{d}\over{d\,x}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,
         *  y)\right)}\over{g_{i,j}(x,y)^2}}- \\ {{f_{i,j}(x,y)\,\left({{d^2}\over{d
         *  \,x\,d\,y}}\,g_{i,j}(x,y)\right)}\over{g_{i,j}(x,y)^2}}-{{{{d}\over{
         *  d\,y}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y)\right)
         *  }\over{g_{i,j}(x,y)^2}}+{{{{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y)
         *  }\over{g_{i,j}(x,y)}} \f$
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            if (lhs_m.IsScalar()) {//scalar multiply
                if (rhs_m.IsScalar()) {//scalar multiply
                    REAL_T lhs_value_m = lhs_m.GetValue();
                    REAL_T rhs_value_m = rhs_m.GetValue();
                    REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(x) * rhs_m.EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
                    REAL_T bb = ((lhs_m.EvaluateDerivative(x) * rhs_m.EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
                    REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(x, y)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
                    REAL_T dd = ((lhs_m.EvaluateDerivative(y) * rhs_m.EvaluateDerivative(x)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
                    REAL_T ee = lhs_m.EvaluateDerivative(x, y) / rhs_value_m;
                    return aa - bb - cc - dd + ee;
                } else {//scalar/matrix divide
                    REAL_T lhs_value_m = lhs_m.GetValue();
                    REAL_T rhs_value_m = rhs_m.GetValue(i, j);
                    REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(x, i, j) * rhs_m.EvaluateDerivative(y, i, j)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
                    REAL_T bb = ((lhs_m.EvaluateDerivative(x) * rhs_m.EvaluateDerivative(y, i, j)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
                    REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(x, y, i, j)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
                    REAL_T dd = ((lhs_m.EvaluateDerivative(y) * rhs_m.EvaluateDerivative(x, i, j)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
                    REAL_T ee = lhs_m.EvaluateDerivative(x, y) / rhs_value_m;
                    return aa - bb - cc - dd + ee;
                }
            } else if (rhs_m.IsScalar()) {//scalar/matrix multiply
                REAL_T lhs_value_m = lhs_m.GetValue(i, j);
                REAL_T rhs_value_m = rhs_m.GetValue();
                REAL_T aa = ((2.0 * lhs_value_m * rhs_m.EvaluateDerivative(x) * rhs_m.EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
                REAL_T bb = ((lhs_m.EvaluateDerivative(x, i, j) * rhs_m.EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
                REAL_T cc = ((lhs_value_m * rhs_m.EvaluateDerivative(x, y)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
                REAL_T dd = ((lhs_m.EvaluateDerivative(y, i, j) * rhs_m.EvaluateDerivative(x)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
                REAL_T ee = lhs_m.EvaluateDerivative(x, y, i, j) / rhs_value_m;
                return aa - bb - cc - dd + ee;
            } else { //matrix multiply

                throw std::invalid_argument("matrix-matrix division not available.");
            }
        }

        /**
         *  Evaluates the third-order derivative with respect to x, y, and z 
         *  at index {i,j}.
         * 
         * \f$ -{{6\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y,z)\right)
         *  \,\left({{d}\over{d\,y}}\,g_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,
         *  z}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)^4}}+{{2\,\left({{d
         *  }\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,y}}\,g_{i,j
         *  }(x,y,z)\right)\,\left({{d}\over{d\,z}}\,g_{i,j}(x,y,z)\right)
         *  }\over{g_{i,j}(x,y,z)^3}}+{{2\,f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,
         *  x\,d\,y}}\,g_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,g_{i,j}(x,
         *  y,z)\right)}\over{g_{i,j}(x,y,z)^3}}+ \\ {{2\,\left({{d}\over{d\,y}}\,f
         *  _{i,j}(x,y,z)\right)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y,z)\right)
         *  \,\left({{d}\over{d\,z}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)
         *  ^3}}-{{{{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,
         *  z}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)^2}}+{{2\,f_{i,j}(x,y
         *  ,z)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y,z)\right)\,\left({{d^2
         *  }\over{d\,y\,d\,z}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)^3}}-
         *  {{{{d}\over{d\,x}}\,f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,
         *  g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)^2}}+ \\ {{2\,f_{i,j}(x,y,z)\,
         *  \left({{d^2}\over{d\,x\,d\,z}}\,g_{i,j}(x,y,z)\right)\,\left({{d
         *  }\over{d\,y}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)^3}}+{{2\,
         *  \left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,x
         *  }}\,g_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y,z)
         *  \right)}\over{g_{i,j}(x,y,z)^3}}-{{{{d^2}\over{d\,x\,d\,z}}\,f_{i,j}
         * (x,y,z)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}
         * (x,y,z)^2}}-{{{{d}\over{d\,y}}\,f_{i,j}(x,y,z)\,\left({{d^2}\over{d
         * \,x\,d\,z}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)^2}}-{{f_{i,j
         * }(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,g_{i,j}(x,y,z)
         * \right)}\over{g_{i,j}(x,y,z)^2}}- \\ {{{{d}\over{d\,z}}\,f_{i,j}(x,y,z)
         * \,\left({{d^2}\over{d\,x\,d\,y}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j
         * }(x,y,z)^2}}-{{{{d^2}\over{d\,y\,d\,z}}\,f_{i,j}(x,y,z)\,\left({{d
         * }\over{d\,x}}\,g_{i,j}(x,y,z)\right)}\over{g_{i,j}(x,y,z)^2}}+{{{{d^
         * 3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)}\over{g_{i,j}(x,y,z)}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
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

        /**
         * Return the number of columns.
         *
         * @return
         */
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

        /**
         * Return the number of rows.
         *
         * @return
         */
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

        /**
         * True if the expression is a scalar.
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
         * 
         * @return 
         */
        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::Divide<T," << lhs_m.ToExpressionTemplateString() << ", " << rhs_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }


        atl::Real<REAL_T> real_m; //used for operations involving real numbers
        const LHS& lhs_m;
        const RHS& rhs_m;
        REAL_T value_m;
        mutable std::vector<REAL_T> val_;
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

    template <class REAL_T, class RHS>
    inline const Divide<REAL_T, Real<REAL_T>, RHS> operator/(const REAL_T& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Divide<REAL_T, Real<REAL_T>, RHS > (a, b.Cast());
    }



}

#endif /* DIVISION_HPP */

