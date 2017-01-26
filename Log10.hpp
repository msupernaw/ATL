/* 
 * File:   Log10.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:35 PM
 */


/**
 *
 * @author  Matthew R. Supernaw
 *
 * Public Domain Notice
 * National Oceanic And Atmospheric Administration
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance warrant the performance or results
 * that may be obtained by using this  software or data. The National Oceanic
 * And Atmospheric Administration and the U.S. Government disclaim all
 * warranties, express or implied, including warranties of performance,
 * merchantability or fitness for any particular purpose.
 *
 * Please cite the author(s) in any work or product based on this material.
 *
 */



#ifndef LOG10_HPP
#define LOG10_HPP

#include "Expression.hpp"



#define AD_LOG10 2.30258509299404590109361379290930926799774169921875

namespace atl {

    /**
     * Expression template to handle log10 for variable or 
     * container expressions. 
     * 
     * \f$ {\it log_{10}}\left(f(x)\right) \f$
     * 
     * or 
     * 
     * \f$ {\it log_{10}}\left(f_{i,j}(x)\right) \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct Log10 : public ExpressionBase<REAL_T, Log10<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        /**
         * Constructor
         * 
         * @param a
         */
        Log10(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Computes the log10 of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::log10(expr_m.GetValue());
        }

        /**
         * Computes the log10 of the evaluated expression at index {i,j}.
         * 
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::log10(expr_m.GetValue(i, j));
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
         * Push variable info into a set.
         *  
         * @param ids
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            expr_m.PushIds(ids);
        }

        /**
         * Push variable info into a set at index {i,j}.
         * 
         * @param ids
         * @param i
         * @param j
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            expr_m.PushIds(ids, i, j);
        }

        inline void PushNLIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, bool nl = false)const {
            expr_m.PushNLIds(ids, true);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return std::log10(expr_m.ComplexEvaluate(x, h));
        }

        /**
         * Evaluates the first-order derivative with respect to x at 
         * index {i,j}.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f(x)}\over{\log 10\,f(x)}} \f$
         *  
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {
            return (expr_m.EvaluateDerivative(x) / (AD_LOG10 * expr_m.GetValue()));
        }

        /**
         * Evaluates the second-order derivative with respect to x and y. 
         * 
         * \f${{{{d^2}\over{d\,x\,d\,y}}\,f(x,y)}\over{\log 10\,f(x,y
         * )}}-{{{{d}\over{d\,x}}\,f(x,y)\,\left({{d}\over{d\,y}}\,
         * f(x,y)\right)}\over{\log 10\,f(x,y)^2}}\f$
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fx = expr_m.GetValue();
            return (expr_m.EvaluateDerivative(x, y) / (AD_LOG10 * fx)) -
                    ((expr_m.EvaluateDerivative(x) * expr_m.EvaluateDerivative(y)) / (AD_LOG10 * (fx * fx)));
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z.
         * 
         * \f$ {{2\,\left({{d}\over{d\,x}}\,f\left(x , y , z\right)\right)\,\left(
         * {{d}\over{d\,y}}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d
         * \,z}}\,f\left(x , y , z\right)\right)}\over{\log 10\,f^3\left(x , y
         *  , z\right)}}-{{{{d^2}\over{d\,x\,d\,y}}\,f\left(x , y , z\right)\,
         * \left({{d}\over{d\,z}}\,f\left(x , y , z\right)\right)}\over{\log 10
         * \,f^2\left(x , y , z\right)}}-{{{{d}\over{d\,x}}\,f\left(x , y , z
         * \right)\,\left({{d^2}\over{d\,y\,d\,z}}\,f\left(x , y , z\right)
         * \right)}\over{\log 10\,f^2\left(x , y , z\right)}}-{{{{d^2}\over{d\,
         * x\,d\,z}}\,f\left(x , y , z\right)\,\left({{d}\over{d\,y}}\,f\left(x
         *  , y , z\right)\right)}\over{\log 10\,f^2\left(x , y , z\right)}}+{{
         *  {{d^3}\over{d\,x\,d\,y\,d\,z}}\,f\left(x , y , z\right)}\over{\log 
         *  10\,f\left(x , y , z\right)}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return (2.0 * (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))
                    *(expr_m.EvaluateDerivative(z))) / (AD_LOG10 * std::pow(expr_m.GetValue(), 3.0))
                    -((expr_m.EvaluateDerivative(x, y))*(expr_m.EvaluateDerivative(z)))
                    / (AD_LOG10 * std::pow(expr_m.GetValue(), 2.0))-((expr_m.EvaluateDerivative(x))
                    *(expr_m.EvaluateDerivative(y, z))) / (AD_LOG10 * std::pow(expr_m.GetValue(), 2.0))
                    -((expr_m.EvaluateDerivative(x, z))*(expr_m.EvaluateDerivative(y)))
                    / (AD_LOG10 * std::pow(expr_m.GetValue(), 2.0)) +
                    expr_m.EvaluateDerivative(x, y, z) / (AD_LOG10 * expr_m.GetValue());
        }

        /**
         * 
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f_{i,j}(x)}\over{\log 10\,f_{i,j}(x)}} \f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return (expr_m.EvaluateDerivative(x, i, j) / (AD_LOG10 * expr_m.GetValue(i, j)));
        }

        /**
         * Evaluates the second-order derivative with respect to x and y at 
         * index {i,j}. 
         * 
         * \f${{{{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y)}\over{\log 10\,f_{i,j}(x,y
         * )}}-{{{{d}\over{d\,x}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,y}}\,f_{i,j
         * }(x,y)\right)}\over{\log 10\,f_{i,j}(x,y)^2}}\f$
         * 
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return (expr_m.EvaluateDerivative(x, y, i, j) / (AD_LOG10 * fx)) -
                    ((expr_m.EvaluateDerivative(x, i, j) * expr_m.EvaluateDerivative(y, i, j)) / (AD_LOG10 * (fx * fx)));
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z
         * at index {i,j}.
         * 
         * \f$ {{2\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         *  }\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j
         *  }(x,y,z)\right)}\over{\log 10\,f_{i,j}(x,y,z)^3}}-{{{{d^2}\over{d\,x
         *  \,d\,y}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)
         *  \right)}\over{\log 10\,f_{i,j}(x,y,z)^2}}-{{{{d}\over{d\,x}}\,f_{i,j
         *  }(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,f_{i,j}(x,y,z)\right)
         *  }\over{\log 10\,f_{i,j}(x,y,z)^2}}-{{{{d^2}\over{d\,x\,d\,z}}\,f_{i,
         *  j}(x,y,z)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)}\over{\log 
         *  10\,f_{i,j}(x,y,z)^2}}+{{{{d^3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y
         *  ,z)}\over{\log 10\,f_{i,j}(x,y,z)}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return (2.0 * (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))
                    *(expr_m.EvaluateDerivative(z, i, j))) / (AD_LOG10 * std::pow(expr_m.GetValue(i, j), 3.0))
                    -((expr_m.EvaluateDerivative(x, y, i, j))*(expr_m.EvaluateDerivative(z, i, j)))
                    / (AD_LOG10 * std::pow(expr_m.GetValue(i, j), 2.0))-((expr_m.EvaluateDerivative(x, i, j))
                    *(expr_m.EvaluateDerivative(y, z, i, j))) / (AD_LOG10 * std::pow(expr_m.GetValue(i, j), 2.0))
                    -((expr_m.EvaluateDerivative(x, z, i, j))*(expr_m.EvaluateDerivative(y, i, j)))
                    / (AD_LOG10 * std::pow(expr_m.GetValue(i, j), 2.0)) +
                    expr_m.EvaluateDerivative(x, y, z, i, j) / (AD_LOG10 * expr_m.GetValue(i, j));
        }

        /**
         * Return the number of rows.
         * 
         * @return 
         */
        size_t GetRows() const {
            return expr_m.GetRows();
        }

        /**
         * True if this expression is a scalar.
         * 
         * @return 
         */
        bool IsScalar() const {
            return expr_m.IsScalar();
        }

        /**
         * Create a string representation of this expression template. 
         * @return 
         */
        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::Log10<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }


        const EXPR& expr_m;
    };

    /**
     * Returns an expression template representing log10.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const Log10<REAL_T, EXPR> log10(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Log10<REAL_T, EXPR>(exp.Cast());
    }

}//end namespace atl

#endif
