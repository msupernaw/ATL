/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pow.hpp
 * Author: matthewsupernaw
 *
 * Created on December 28, 2016, 11:59 AM
 */

#ifndef POW_HPP
#define POW_HPP

#include "Expression.hpp"


namespace atl {

    /**
     * Expression template to handle pow for variable or 
     * container expressions. 
     * 
     * \f$  f(x)^{g(x)} \f$
     * 
     * or 
     * 
     * \f$ f_{i,j}(x)^{g_{i,j}(x)}  \f$
     * 
     */
    template <class REAL_T, class LHS, class RHS>
    struct Pow : public ExpressionBase<REAL_T, Pow<REAL_T, LHS, RHS> > {
        typedef REAL_T BASE_TYPE;



        BinaryMethod pow_method;

        /**
         * Constructor for two expression template types.
         * 
         * @param lhs
         * @param rhs
         */
        Pow(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {

        }

        /**
         * Constructor for a real and expression template type.
         * 
         * @param lhs
         * @param rhs
         */
        Pow(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        /**
         * Constructor for a expression template type and a real type.
         * 
         * @param lhs
         * @param rhs
         */
        Pow(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;
        }

        /**
         * Computes the the evaluation of the lhs raised by the 
         * evaluation of the rhs.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::pow(lhs_m.GetValue(), rhs_m.GetValue());
        }

        /**
         * Computes the the evaluation of the lhs raised by the 
         * evaluation of the rhs at index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::pow(lhs_m.GetValue(i, j), rhs_m.GetValue(i, j));
        }

        /**
         * Returns true.
         * 
         * @return 
         */
        inline bool IsNonlinear() const {
            return true;
        }

        /**
         * Push ids into a set.
         * 
         * @param ids
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        /**
         * Push ids into a set at index {i,j}.
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
            lhs_m.PushNLIds(ids, true);
            rhs_m.PushNLIds(ids, true);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return std::pow(lhs_m.ComplexEvaluate(x, h), rhs_m.ComplexEvaluate(x, h));
        }

        /**
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ f\left(x\right)^{g\left(x\right)}\,\left(\log f\left(x\right)\,
         *  \left({{d}\over{d\,x}}\,g\left(x\right)\right)+{{g\left(x\right)\,
         *  \left({{d}\over{d\,x}}\,f\left(x\right)\right)}\over{f\left(x\right) 
         *  }}\right) \f$
         * 
         * @param x
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x) const {
            REAL_T g = rhs_m.GetValue();
            REAL_T f = lhs_m.GetValue();
            REAL_T fx = lhs_m.EvaluateDerivative(x);
            REAL_T gx = rhs_m.EvaluateDerivative(x);
            return std::pow(f, g)*(std::log(f) * gx + g * fx / f);
        }

        /**
         * Evaluates the second-order derivative with respect to x and y.
         * 
         * \f$ f(x,y)^{g(x,y)}\,\left({{{{d}\over{d\,x}}\,f(x,y)
         *  \,\left({{d}\over{d\,y}}\,g(x,y)\right)}\over{f(x,y)}}+
         *  \log f(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,g(x,y)
         *  \right)+{{{{d}\over{d\,y}}\,f(x,y)\,\left({{d}\over{d\,x}}\,g
         *  (x,y)\right)}\over{f(x,y)}}-{{g(x,y)\,\left({{d
         *  }\over{d\,x}}\,f(x,y)\right)\,\left({{d}\over{d\,y}}\,f(
         *  x,y)\right)}\over{f(x,y)^2}}+ \\ {{g(x,y)\,\left({{d^2
         *  }\over{d\,x\,d\,y}}\,f(x,y)\right)}\over{f(x,y)}}\right)
         *  +f(x,y)^{g(x,y)}\,\left(\log f(x,y)\,\left({{d
         *  }\over{d\,x}}\,g(x,y)\right)+{{g(x,y)\,\left({{d}\over{d
         *  \,x}}\,f(x,y)\right)}\over{f(x,y)}}\right)\,\left(\log f
         *  (x,y)\,\left({{d}\over{d\,y}}\,g(x,y)\right)+{{g(x
         *  ,y)\,\left({{d}\over{d\,y}}\,f(x,y)\right)}\over{f(x,y)
         *  }}\right) \f$
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fxy = lhs_m.EvaluateDerivative(x, y);
            REAL_T g = rhs_m.GetValue();
            REAL_T f = lhs_m.GetValue();
            REAL_T fx = lhs_m.EvaluateDerivative(x);
            REAL_T gy = rhs_m.EvaluateDerivative(y);
            REAL_T fy = lhs_m.EvaluateDerivative(y);
            REAL_T gx = rhs_m.EvaluateDerivative(x);
            REAL_T gxy = rhs_m.EvaluateDerivative(x, y);
            return std::pow(f, g)*(((fx * gy) / f) + std::log(f) * gxy + (fy * gx / f) -
                    (g * fx * fy) / (f * f) + g * fxy / f) + std::pow(f, g)*(std::log(f) * gx +
                    g * fx / f)*(std::log(f) * gy + g * fy / f);
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z.
         * 
         * \f$f(x,y,z)^{g(x,y,z)}\,\left(-{{{{d}\over{d\,x}}\,f
         * (x,y,z)\,\left({{d}\over{d\,y}}\,f(x,y,z)\right)\,\left({{d
         * }\over{d\,z}}\,g(x,y,z)\right)}\over{f(x,y,z)^2}}+{{{{d^
         * 2}\over{d\,x\,d\,y}}\,f(x,y,z)\,\left({{d}\over{d\,z}}\,g_{i,j
         * }(x,y,z)\right)}\over{f(x,y,z)}}+{{{{d}\over{d\,x}}\,f(x
         * ,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,g(x,y,z)\right)}\over{f
         * (x,y,z)}}-{{{{d}\over{d\,x}}\,f(x,y,z)\,\left({{d}\over{
         * d\,z}}\,f(x,y,z)\right)\,\left({{d}\over{d\,y}}\,g(x,y,z
         * )\right)}\over{f(x,y,z)^2}}+ \\ {{{{d^2}\over{d\,x\,d\,z}}\,f_{i,j
         * }(x,y,z)\,\left({{d}\over{d\,y}}\,g(x,y,z)\right)}\over{f_{i,j
         * }(x,y,z)}}+{{{{d}\over{d\,y}}\,f(x,y,z)\,\left({{d^2}\over{d\,
         * x\,d\,z}}\,g(x,y,z)\right)}\over{f(x,y,z)}}+\log f
         * (x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,g(x,y,z)\right)
         * +{{{{d}\over{d\,z}}\,f(x,y,z)\,\left({{d^2}\over{d\,x\,d\,y}}
         * \,g(x,y,z)\right)}\over{f(x,y,z)}}-{{{{d}\over{d\,y}}\,f
         * (x,y,z)\,\left({{d}\over{d\,z}}\,f(x,y,z)\right)\,\left(
         * {{d}\over{d\,x}}\,g(x,y,z)\right)}\over{f(x,y,z)^2}}+{{
         * {{d^2}\over{d\,y\,d\,z}}\,f(x,y,z)\,\left({{d}\over{d\,x}}\,g
         * (x,y,z)\right)}\over{f(x,y,z)}}+ \\ {{2\,g(x,y,z)\,
         * \left({{d}\over{d\,x}}\,f(x,y,z)\right)\,\left({{d}\over{d\,y
         * }}\,f(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f(x,y,z)
         * \right)}\over{f(x,y,z)^3}}-{{g(x,y,z)\,\left({{d^2
         * }\over{d\,x\,d\,y}}\,f(x,y,z)\right)\,\left({{d}\over{d\,z}}\,
         * f(x,y,z)\right)}\over{f(x,y,z)^2}}-{{g(x,y,z)\,
         * \left({{d}\over{d\,x}}\,f(x,y,z)\right)\,\left({{d^2}\over{d\,
         * y\,d\,z}}\,f(x,y,z)\right)}\over{f(x,y,z)^2}}-{{g(
         * x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,f(x,y,z)\right)\,
         * \left({{d}\over{d\,y}}\,f(x,y,z)\right)}\over{f(x,y,z)^2
         * }}+{{g(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f(x
         * ,y,z)\right)}\over{f(x,y,z)}}\right)+ \\ f(x,y,z)^{g(x
         * ,y,z)}\,\left(\log f(x,y,z)\,\left({{d}\over{d\,x}}\,g(x
         * ,y,z)\right)+{{g(x,y,z)\,\left({{d}\over{d\,x}}\,f(x,y,z
         * )\right)}\over{f(x,y,z)}}\right)\,\left({{{{d}\over{d\,y}}\,f
         * (x,y,z)\,\left({{d}\over{d\,z}}\,g(x,y,z)\right)}\over{f
         * (x,y,z)}}+\log f(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}
         * \,g(x,y,z)\right)+{{{{d}\over{d\,z}}\,f(x,y,z)\,\left({{
         * d}\over{d\,y}}\,g(x,y,z)\right)}\over{f(x,y,z)}}-{{g_{i,
         * j}(x,y,z)\,\left({{d}\over{d\,y}}\,f(x,y,z)\right)\,\left({{d
         * }\over{d\,z}}\,f(x,y,z)\right)}\over{f(x,y,z)^2}}+{{g_{i
         * ,j}(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,f(x,y,z)\right)
         * }\over{f(x,y,z)}}\right)+ \\ f(x,y,z)^{g(x,y,z)}\,
         * \left(\log f(x,y,z)\,\left({{d}\over{d\,y}}\,g(x,y,z)
         * \right)+{{g(x,y,z)\,\left({{d}\over{d\,y}}\,f(x,y,z)
         * \right)}\over{f(x,y,z)}}\right)\,\left({{{{d}\over{d\,x}}\,f_{
         * i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,g(x,y,z)\right)}\over{f_{
         * i,j}(x,y,z)}}+\log f(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,g
         * (x,y,z)\right)+{{{{d}\over{d\,z}}\,f(x,y,z)\,\left({{d
         * }\over{d\,x}}\,g(x,y,z)\right)}\over{f(x,y,z)}}-{{g_{i,j
         * }(x,y,z)\,\left({{d}\over{d\,x}}\,f(x,y,z)\right)\,\left({{d
         * }\over{d\,z}}\,f(x,y,z)\right)}\over{f(x,y,z)^2}}+{{g_{i
         * ,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,f(x,y,z)\right)
         * }\over{f(x,y,z)}}\right)+ \\ f(x,y,z)^{g(x,y,z)}\,
         * \left({{{{d}\over{d\,x}}\,f(x,y,z)\,\left({{d}\over{d\,y}}\,g
         * (x,y,z)\right)}\over{f(x,y,z)}}+\log f(x,y,z)\,
         * \left({{d^2}\over{d\,x\,d\,y}}\,g(x,y,z)\right)+{{{{d}\over{d
         * \,y}}\,f(x,y,z)\,\left({{d}\over{d\,x}}\,g(x,y,z)\right)
         * }\over{f(x,y,z)}}-{{g(x,y,z)\,\left({{d}\over{d\,x}}\,f
         * (x,y,z)\right)\,\left({{d}\over{d\,y}}\,f(x,y,z)\right)
         * }\over{f(x,y,z)^2}}+{{g(x,y,z)\,\left({{d^2}\over{d\,x\,
         * d\,y}}\,f(x,y,z)\right)}\over{f(x,y,z)}}\right)\,\left(
         * \log f(x,y,z)\,\left({{d}\over{d\,z}}\,g(x,y,z)\right)+
         * {{g(x,y,z)\,\left({{d}\over{d\,z}}\,f(x,y,z)\right)
         * }\over{f(x,y,z)}}\right)+ \\ f(x,y,z)^{g(x,y,z)}\,
         * \left(\log f(x,y,z)\,\left({{d}\over{d\,x}}\,g(x,y,z)
         * \right)+{{g(x,y,z)\,\left({{d}\over{d\,x}}\,f(x,y,z)
         * \right)}\over{f(x,y,z)}}\right)\,\left(\log f(x,y,z)\,
         * \left({{d}\over{d\,y}}\,g(x,y,z)\right)+{{g(x,y,z)\,
         * \left({{d}\over{d\,y}}\,f(x,y,z)\right)}\over{f(x,y,z)}}
         * \right)\,\left(\log f(x,y,z)\,\left({{d}\over{d\,z}}\,g(
         * x,y,z)\right)+{{g(x,y,z)\,\left({{d}\over{d\,z}}\,f(x,y,
         * z)\right)}\over{f(x,y,z)}}\right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {

            return std::pow(lhs_m.GetValue(), rhs_m.GetValue())*(-1.0 * ((lhs_m.EvaluateDerivative(x))*(lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / std::pow(lhs_m.GetValue(), 2.0)+((lhs_m.EvaluateDerivative(x, y))*(rhs_m.EvaluateDerivative(z))) / lhs_m.GetValue() + ((lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y, z))) / lhs_m.GetValue() - ((lhs_m.EvaluateDerivative(x))*(lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(y))) / std::pow(lhs_m.GetValue(), 2.0)+((lhs_m.EvaluateDerivative(x, z))*(rhs_m.EvaluateDerivative(y))) / lhs_m.GetValue() +
                    ((lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x, z))) / lhs_m.GetValue() + std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(x, y, z))+((lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x, y))) / lhs_m.GetValue() - ((lhs_m.EvaluateDerivative(y))*(lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x))) / std::pow(lhs_m.GetValue(), 2.0)+((lhs_m.EvaluateDerivative(y, z))*(rhs_m.EvaluateDerivative(x))) / lhs_m.GetValue() +
                    (2.0 * rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x))*(lhs_m.EvaluateDerivative(y))*(lhs_m.EvaluateDerivative(z))) / std::pow(lhs_m.GetValue(), 3.0) - (rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x, y))*(lhs_m.EvaluateDerivative(z))) / std::pow(lhs_m.GetValue(), 2.0)-(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x))*(lhs_m.EvaluateDerivative(y, z))) / std::pow(lhs_m.GetValue(), 2.0)-(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x, z))*(lhs_m.EvaluateDerivative(y))) / std::pow(lhs_m.GetValue(), 2.0)+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x, y, z))) / lhs_m.GetValue()) +
                    std::pow(lhs_m.GetValue(), rhs_m.GetValue())*(std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(x))+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x))) / lhs_m.GetValue())*
                    (((lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(z))) / lhs_m.GetValue() + std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(y, z))+((lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(y))) / lhs_m.GetValue() - (rhs_m.GetValue() * (lhs_m.EvaluateDerivative(y))*(lhs_m.EvaluateDerivative(z))) / std::pow(lhs_m.GetValue(), 2.0)+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(y, z))) / lhs_m.GetValue()) + std::pow(lhs_m.GetValue(), rhs_m.GetValue())*
                    (std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(y))+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(y))) / lhs_m.GetValue())*
                    (((lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(z))) / lhs_m.GetValue() + std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(x, z))+((lhs_m.EvaluateDerivative(z))*(rhs_m.EvaluateDerivative(x))) / lhs_m.GetValue() - (rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x))*(lhs_m.EvaluateDerivative(z))) / std::pow(lhs_m.GetValue(), 2.0)+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x, z))) / lhs_m.GetValue()) + std::pow(lhs_m.GetValue(), rhs_m.GetValue())*
                    (((lhs_m.EvaluateDerivative(x))*(rhs_m.EvaluateDerivative(y))) / lhs_m.GetValue() + std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(x, y))+((lhs_m.EvaluateDerivative(y))*(rhs_m.EvaluateDerivative(x))) / lhs_m.GetValue() - (rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x))*(lhs_m.EvaluateDerivative(y))) / std::pow(lhs_m.GetValue(), 2.0)+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x, y))) / lhs_m.GetValue())*
                    (std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(z))+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(z))) / lhs_m.GetValue()) + std::pow(lhs_m.GetValue(), rhs_m.GetValue())*(std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(x))+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(x))) / lhs_m.GetValue())*(std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(y))+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(y))) / lhs_m.GetValue())*
                    (std::log(lhs_m.GetValue())*(rhs_m.EvaluateDerivative(z))+(rhs_m.GetValue() * (lhs_m.EvaluateDerivative(z))) / lhs_m.GetValue());
        }

        /**
         * Evaluates the first-order derivative with respect to x at index {i,j}.
         * 
         * \f$ f_{i,j}(x)^{g_{i,j}(x)}\,\left(\log f_{i,j}(x)\,\left({{d}\over{d\,
         *  x}}\,g_{i,j}(x)\right)+{{g_{i,j}(x)\,\left({{d}\over{d\,x}}\,f_{i,j}
         *  (x)\right)}\over{f_{i,j}(x)}}\right) \f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            REAL_T g = rhs_m.GetValue(i, j);
            REAL_T f = lhs_m.GetValue(i, j);
            REAL_T fx = lhs_m.EvaluateDerivative(x, i, j);
            REAL_T gx = rhs_m.EvaluateDerivative(x, i, j);
            return std::pow(f, g)*(std::log(f) * gx + g * fx / f);
        }

        /**
         * Evaluates the second-order derivative with respect to x and y at index
         *  {i,j}.
         * 
         * \f$ f_{i,j}(x,y)^{g_{i,j}(x,y)}\,\left({{{{d}\over{d\,x}}\,f_{i,j}(x,y)
         *  \,\left({{d}\over{d\,y}}\,g_{i,j}(x,y)\right)}\over{f_{i,j}(x,y)}}+
         *  \log f_{i,j}(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,g_{i,j}(x,y)
         *  \right)+{{{{d}\over{d\,y}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,x}}\,g
         *  _{i,j}(x,y)\right)}\over{f_{i,j}(x,y)}}-{{g_{i,j}(x,y)\,\left({{d
         *  }\over{d\,x}}\,f_{i,j}(x,y)\right)\,\left({{d}\over{d\,y}}\,f_{i,j}(
         *  x,y)\right)}\over{f_{i,j}(x,y)^2}}+ \\ {{g_{i,j}(x,y)\,\left({{d^2
         *  }\over{d\,x\,d\,y}}\,f_{i,j}(x,y)\right)}\over{f_{i,j}(x,y)}}\right)
         *  +f_{i,j}(x,y)^{g_{i,j}(x,y)}\,\left(\log f_{i,j}(x,y)\,\left({{d
         *  }\over{d\,x}}\,g_{i,j}(x,y)\right)+{{g_{i,j}(x,y)\,\left({{d}\over{d
         *  \,x}}\,f_{i,j}(x,y)\right)}\over{f_{i,j}(x,y)}}\right)\,\left(\log f
         *  _{i,j}(x,y)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y)\right)+{{g_{i,j}(x
         *  ,y)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y)\right)}\over{f_{i,j}(x,y)
         *  }}\right) \f$
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {

            REAL_T fxy = lhs_m.EvaluateDerivative(x, y, i, j);
            REAL_T g = rhs_m.GetValue(i, j);
            REAL_T f = lhs_m.GetValue(i, j);
            REAL_T fx = lhs_m.EvaluateDerivative(x, i, j);
            REAL_T gy = rhs_m.EvaluateDerivative(y, i, j);
            REAL_T fy = lhs_m.EvaluateDerivative(y, i, j);
            REAL_T gx = rhs_m.EvaluateDerivative(x, i, j);
            REAL_T gxy = rhs_m.EvaluateDerivative(x, y, i, j);
            return std::pow(f, g)*(((fx * gy) / f) + std::log(f) * gxy + (fy * gx / f) -
                    (g * fx * fy) / (f * f) + g * fxy / f) + std::pow(f, g)*(std::log(f) * gx +
                    g * fx / f)*(std::log(f) * gy + g * fy / f);
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z
         * at index {i,j}.
         * 
         * \f$f_{i,j}(x,y,z)^{g_{i,j}(x,y,z)}\,\left(-{{{{d}\over{d\,x}}\,f_{i,j}
         * (x,y,z)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         * }\over{d\,z}}\,g_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2}}+{{{{d^
         * 2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,g_{i,j
         * }(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}+{{{{d}\over{d\,x}}\,f_{i,j}(x
         * ,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,g_{i,j}(x,y,z)\right)}\over{f
         * _{i,j}(x,y,z)}}-{{{{d}\over{d\,x}}\,f_{i,j}(x,y,z)\,\left({{d}\over{
         * d\,z}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y,z
         * )\right)}\over{f_{i,j}(x,y,z)^2}}+ \\ {{{{d^2}\over{d\,x\,d\,z}}\,f_{i,j
         * }(x,y,z)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y,z)\right)}\over{f_{i,j
         * }(x,y,z)}}+{{{{d}\over{d\,y}}\,f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,
         * x\,d\,z}}\,g_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}+\log f_{i,j}
         * (x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,g_{i,j}(x,y,z)\right)
         * +{{{{d}\over{d\,z}}\,f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,y}}
         * \,g_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}-{{{{d}\over{d\,y}}\,f
         * _{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)\,\left(
         * {{d}\over{d\,x}}\,g_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2}}+{{
         * {{d^2}\over{d\,y\,d\,z}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,g
         * _{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}+ \\ {{2\,g_{i,j}(x,y,z)\,
         * \left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,y
         * }}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)
         * \right)}\over{f_{i,j}(x,y,z)^3}}-{{g_{i,j}(x,y,z)\,\left({{d^2
         * }\over{d\,x\,d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,
         * f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2}}-{{g_{i,j}(x,y,z)\,
         * \left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d^2}\over{d\,
         * y\,d\,z}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2}}-{{g_{i,j}(
         * x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,f_{i,j}(x,y,z)\right)\,
         * \left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2
         * }}+{{g_{i,j}(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x
         * ,y,z)\right)}\over{f_{i,j}(x,y,z)}}\right)+ \\ f_{i,j}(x,y,z)^{g_{i,j}(x
         * ,y,z)}\,\left(\log f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,g_{i,j}(x
         * ,y,z)\right)+{{g_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z
         * )\right)}\over{f_{i,j}(x,y,z)}}\right)\,\left({{{{d}\over{d\,y}}\,f
         * _{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,g_{i,j}(x,y,z)\right)}\over{f
         * _{i,j}(x,y,z)}}+\log f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}
         * \,g_{i,j}(x,y,z)\right)+{{{{d}\over{d\,z}}\,f_{i,j}(x,y,z)\,\left({{
         * d}\over{d\,y}}\,g_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}-{{g_{i,
         * j}(x,y,z)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         * }\over{d\,z}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2}}+{{g_{i
         * ,j}(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,f_{i,j}(x,y,z)\right)
         * }\over{f_{i,j}(x,y,z)}}\right)+ \\ f_{i,j}(x,y,z)^{g_{i,j}(x,y,z)}\,
         * \left(\log f_{i,j}(x,y,z)\,\left({{d}\over{d\,y}}\,g_{i,j}(x,y,z)
         * \right)+{{g_{i,j}(x,y,z)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)
         * \right)}\over{f_{i,j}(x,y,z)}}\right)\,\left({{{{d}\over{d\,x}}\,f_{
         * i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,g_{i,j}(x,y,z)\right)}\over{f_{
         * i,j}(x,y,z)}}+\log f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,g
         * _{i,j}(x,y,z)\right)+{{{{d}\over{d\,z}}\,f_{i,j}(x,y,z)\,\left({{d
         * }\over{d\,x}}\,g_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}-{{g_{i,j
         * }(x,y,z)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         * }\over{d\,z}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2}}+{{g_{i
         * ,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,f_{i,j}(x,y,z)\right)
         * }\over{f_{i,j}(x,y,z)}}\right)+ \\ f_{i,j}(x,y,z)^{g_{i,j}(x,y,z)}\,
         * \left({{{{d}\over{d\,x}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,y}}\,g
         * _{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}+\log f_{i,j}(x,y,z)\,
         * \left({{d^2}\over{d\,x\,d\,y}}\,g_{i,j}(x,y,z)\right)+{{{{d}\over{d
         * \,y}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y,z)\right)
         * }\over{f_{i,j}(x,y,z)}}-{{g_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,f
         * _{i,j}(x,y,z)\right)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)
         * }\over{f_{i,j}(x,y,z)^2}}+{{g_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,
         * d\,y}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}\right)\,\left(
         * \log f_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,g_{i,j}(x,y,z)\right)+
         * {{g_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)
         * }\over{f_{i,j}(x,y,z)}}\right)+ \\ f_{i,j}(x,y,z)^{g_{i,j}(x,y,z)}\,
         * \left(\log f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,g_{i,j}(x,y,z)
         * \right)+{{g_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)
         * \right)}\over{f_{i,j}(x,y,z)}}\right)\,\left(\log f_{i,j}(x,y,z)\,
         * \left({{d}\over{d\,y}}\,g_{i,j}(x,y,z)\right)+{{g_{i,j}(x,y,z)\,
         * \left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)}}
         * \right)\,\left(\log f_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,g_{i,j}(
         * x,y,z)\right)+{{g_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,
         * z)\right)}\over{f_{i,j}(x,y,z)}}\right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return std::pow(lhs_m.GetValue(i, j), rhs_m.GetValue(i, j))*(-1.0 * ((lhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, i, j))*(rhs_m.EvaluateDerivative(z, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)+((lhs_m.EvaluateDerivative(x, y, i, j))*(rhs_m.EvaluateDerivative(z, i, j))) / lhs_m.GetValue(i, j) + ((lhs_m.EvaluateDerivative(x, i, j))*(rhs_m.EvaluateDerivative(y, z, i, j))) / lhs_m.GetValue(i, j) - ((lhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(z, i, j))*(rhs_m.EvaluateDerivative(y, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)+((lhs_m.EvaluateDerivative(x, z, i, j))*(rhs_m.EvaluateDerivative(y, i, j))) / lhs_m.GetValue(i, j) +
                    ((lhs_m.EvaluateDerivative(y, i, j))*(rhs_m.EvaluateDerivative(x, z, i, j))) / lhs_m.GetValue(i, j) + std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(x, y, z, i, j))+((lhs_m.EvaluateDerivative(z, i, j))*(rhs_m.EvaluateDerivative(x, y, i, j))) / lhs_m.GetValue(i, j) - ((lhs_m.EvaluateDerivative(y, i, j))*(lhs_m.EvaluateDerivative(z, i, j))*(rhs_m.EvaluateDerivative(x, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)+((lhs_m.EvaluateDerivative(y, z, i, j))*(rhs_m.EvaluateDerivative(x, i, j))) / lhs_m.GetValue(i, j) +
                    (2.0 * rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, i, j))*(lhs_m.EvaluateDerivative(z, i, j))) / std::pow(lhs_m.GetValue(i, j), 3.0) - (rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, y, i, j))*(lhs_m.EvaluateDerivative(z, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)-(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, z, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)-(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, z, i, j))*(lhs_m.EvaluateDerivative(y, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, y, z, i, j))) / lhs_m.GetValue(i, j)) +
                    std::pow(lhs_m.GetValue(i, j), rhs_m.GetValue(i, j))*(std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(x, i, j))+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, i, j))) / lhs_m.GetValue(i, j))*
                    (((lhs_m.EvaluateDerivative(y, i, j))*(rhs_m.EvaluateDerivative(z, i, j))) / lhs_m.GetValue(i, j) + std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(y, z, i, j))+((lhs_m.EvaluateDerivative(z, i, j))*(rhs_m.EvaluateDerivative(y, i, j))) / lhs_m.GetValue(i, j) - (rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(y, i, j))*(lhs_m.EvaluateDerivative(z, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(y, z, i, j))) / lhs_m.GetValue(i, j)) + std::pow(lhs_m.GetValue(i, j), rhs_m.GetValue(i, j))*
                    (std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(y, i, j))+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(y, i, j))) / lhs_m.GetValue(i, j))*
                    (((lhs_m.EvaluateDerivative(x, i, j))*(rhs_m.EvaluateDerivative(z, i, j))) / lhs_m.GetValue(i, j) + std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(x, z, i, j))+((lhs_m.EvaluateDerivative(z, i, j))*(rhs_m.EvaluateDerivative(x, i, j))) / lhs_m.GetValue(i, j) - (rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(z, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, z, i, j))) / lhs_m.GetValue(i, j)) + std::pow(lhs_m.GetValue(i, j), rhs_m.GetValue(i, j))*
                    (((lhs_m.EvaluateDerivative(x, i, j))*(rhs_m.EvaluateDerivative(y, i, j))) / lhs_m.GetValue(i, j) + std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(x, y, i, j))+((lhs_m.EvaluateDerivative(y, i, j))*(rhs_m.EvaluateDerivative(x, i, j))) / lhs_m.GetValue(i, j) - (rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, i, j))) / std::pow(lhs_m.GetValue(i, j), 2.0)+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, y, i, j))) / lhs_m.GetValue(i, j))*
                    (std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(z, i, j))+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(z, i, j))) / lhs_m.GetValue(i, j)) + std::pow(lhs_m.GetValue(i, j), rhs_m.GetValue(i, j))*(std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(x, i, j))+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(x, i, j))) / lhs_m.GetValue(i, j))*(std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(y, i, j))+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(y, i, j))) / lhs_m.GetValue(i, j))*
                    (std::log(lhs_m.GetValue(i, j))*(rhs_m.EvaluateDerivative(z, i, j))+(rhs_m.GetValue(i, j) * (lhs_m.EvaluateDerivative(z, i, j))) / lhs_m.GetValue(i, j));
        }

        /**
         * Returns the number of columns. 
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
         * Returns the number of rows.
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
         * True if scalar.
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
            ss << "atl::Pow<T," << lhs_m.ToExpressionTemplateString() << ", " << rhs_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }

        atl::Real<REAL_T> real_m; //used for operations involving real numbers
        const LHS& lhs_m;
        const RHS& rhs_m;
        REAL_T value_m;
    };

    /**
     * Returns a expression template representing pow.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS, class RHS>
    inline const Pow<REAL_T, LHS, RHS> pow(const ExpressionBase<REAL_T, LHS>& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Pow<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    /**
     * Returns a expression template representing pow.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS>
    inline const Pow<REAL_T, LHS, Real<REAL_T> > pow(const ExpressionBase<REAL_T, LHS>& a,
            REAL_T b) {

        return Pow<REAL_T, LHS, Real<REAL_T> > (a.Cast(), b);
    }

    /**
     * Returns a expression template representing pow.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class RHS>
    inline const Pow<REAL_T, Real<REAL_T>, RHS> pow(const REAL_T& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Pow<REAL_T, Real<REAL_T>, RHS > (a, b.Cast());
    }
}


#endif /* POW_HPP */

