/* 
 * File:   Log.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:34 PM
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


#ifndef LOG_HPP
#define LOG_HPP

#include "Expression.hpp"

namespace atl {

    

    /**
     * Expression template to handle log for variable or 
     * container expressions. 
     * 
     * \f$ \log f(x,y) \f$
     * 
     * or 
     * 
     * \f$ \log f_{i,j}(x,y) \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct Log : public ExpressionBase<REAL_T, Log<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Log(const Log<REAL_T, EXPR>& other) :
        expr_m(other.expr_m.Cast()), val_(other.val_) {
        }

        /**
         * Constructor 
         * 
         * @param a
         */
        Log(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Computes the log of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::log(expr_m.GetValue());
        }

        /**
         * Computes the log of the evaluated expression at index {i,j}.
         * 
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::log(expr_m.GetValue(i, j));
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
            return std::log(expr_m.ComplexEvaluate(x, h));
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            if (degree == 0) {
                val_.reserve(5);
                val_.resize(1);

                val_[0] = std::log(this->expr_m.Taylor(0));

                return val_[0];
            }

            size_t l = val_.size();
            val_.resize(degree + 1);

            for (unsigned int i = l; i <= degree; ++i) {
                val_[i] = expr_m.Taylor(i);
                for (unsigned int j = 1; j < i; ++j) {
                    val_[i] -= (static_cast<REAL_T> (1.0) -
                            static_cast<REAL_T> (j) / static_cast<REAL_T> (i)) *
                            expr_m.Taylor(j) * val_[i - j];
                }
                val_[i] /= expr_m.Taylor(0);
            }

            return val_[degree];
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return atl::log(expr_m.ToDynamic());
        }

        /**
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f_{i,j}(x)}\over{f_{i,j}(x)}} \f$
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {
            return expr_m.EvaluateDerivative(x) / expr_m.GetValue();
        }

        /**
         * Evaluates the second-order derivative with respect to x and y.
         * 
         * \f$ {{{{d^2}\over{d\,x\,d\,y}}\,f(x,y)}\over{f(x,y)}}-{{{{d
         *  }\over{d\,x}}\,f(x,y)\,\left({{d}\over{d\,y}}\,f(x,y)
         *  \right)}\over{f(x,y)^2}} \f$
         * 
         * @param x
         * @param y
         * 
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {

            return (expr_m.EvaluateDerivative(x, y) / expr_m.GetValue()) -
                    (expr_m.EvaluateDerivative(x) * expr_m.EvaluateDerivative(y))
                    / (expr_m.GetValue() * expr_m.GetValue());
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z.
         * 
         * \f$ {{2\,\left({{d}\over{d\,x}}\,f(x,y,z)\right)\,\left({{d
         *  }\over{d\,y}}\,f(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j
         *  }(x,y,z)\right)}\over{f(x,y,z)^3}}-{{{{d^2}\over{d\,x\,d\,y}}
         *  \,f(x,y,z)\,\left({{d}\over{d\,z}}\,f(x,y,z)\right)
         *  }\over{f(x,y,z)^2}}-{{{{d}\over{d\,x}}\,f(x,y,z)\,\left(
         *  {{d^2}\over{d\,y\,d\,z}}\,f(x,y,z)\right)}\over{f(x,y,z)
         *  ^2}}-{{{{d^2}\over{d\,x\,d\,z}}\,f(x,y,z)\,\left({{d}\over{d\,
         *  y}}\,f(x,y,z)\right)}\over{f(x,y,z)^2}}+{{{{d^3}\over{d
         *  \,x\,d\,y\,d\,z}}\,f(x,y,z)}\over{f(x,y,z)}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return (2.0 * (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))*
                    (expr_m.EvaluateDerivative(z))) / std::pow(expr_m.GetValue(), 3.0)-
                    ((expr_m.EvaluateDerivative(x, y))*(expr_m.EvaluateDerivative(z))) /
                    std::pow(expr_m.GetValue(), 2.0)-((expr_m.EvaluateDerivative(x))*
                    (expr_m.EvaluateDerivative(y, z))) / std::pow(expr_m.GetValue(), 2.0)
                    -((expr_m.EvaluateDerivative(x, z))*(expr_m.EvaluateDerivative(y))) /
                    std::pow(expr_m.GetValue(), 2.0) + expr_m.EvaluateDerivative(x, y, z) /
                    expr_m.GetValue();

        }

        /**
         * Evaluates the first-order derivative with respect to x at 
         * index {i,j}.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f_{i,j}(x)}\over{f_{i,j}(x)}} \f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return expr_m.EvaluateDerivative(x, i, j) / expr_m.GetValue(i, j);
        }

        /**
         * Evaluates the second-order derivative with respect to x and y at 
         * index {i,j}.
         * 
         * \f$ {{{{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y)}\over{f_{i,j}(x,y)}}-{{{{d
         *  }\over{d\,x}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y)
         *  \right)}\over{f_{i,j}(x,y)^2}} \f$
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return (expr_m.EvaluateDerivative(x, y, i, j) / this->GetValue(i, j)) -
                    (expr_m.EvaluateDerivative(x, i, j) * expr_m.EvaluateDerivative(y, i, j))
                    / (expr_m.GetValue(i, j) * this->GetValue(i, j));
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z 
         * at index {i,j}.
         * 
         * \f$ {{2\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         *  }\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j
         *  }(x,y,z)\right)}\over{f_{i,j}(x,y,z)^3}}-{{{{d^2}\over{d\,x\,d\,y}}
         *  \,f_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)
         *  }\over{f_{i,j}(x,y,z)^2}}-{{{{d}\over{d\,x}}\,f_{i,j}(x,y,z)\,\left(
         *  {{d^2}\over{d\,y\,d\,z}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)
         *  ^2}}-{{{{d^2}\over{d\,x\,d\,z}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,
         *  y}}\,f_{i,j}(x,y,z)\right)}\over{f_{i,j}(x,y,z)^2}}+{{{{d^3}\over{d
         *  \,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)}\over{f_{i,j}(x,y,z)}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return (2.0 * (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))*
                    (expr_m.EvaluateDerivative(z, i, j))) / std::pow(expr_m.GetValue(i, j), 3.0)-
                    ((expr_m.EvaluateDerivative(x, y, i, j))*(expr_m.EvaluateDerivative(z, i, j))) /
                    std::pow(expr_m.GetValue(i, j), 2.0)-((expr_m.EvaluateDerivative(x, i, j))*
                    (expr_m.EvaluateDerivative(y, z, i, j))) / std::pow(expr_m.GetValue(i, j), 2.0)
                    -((expr_m.EvaluateDerivative(x, z, i, j))*(expr_m.EvaluateDerivative(y, i, j))) /
                    std::pow(expr_m.GetValue(i, j), 2.0) + expr_m.EvaluateDerivative(x, y, z, i, j) /
                    expr_m.GetValue(i, j);
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
            ss << "atl::Log<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }


        const EXPR& expr_m;
        mutable std::vector<REAL_T> val_;

    };

    /**
     * Returns an expression template representing log. 
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const Log<REAL_T, EXPR> log(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Log<REAL_T, EXPR>(exp.Cast());
    }



}//end namespace atl


namespace std {

    /**
     * Returns an expression template representing log. 
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Log<REAL_T, EXPR> log(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Log<REAL_T, EXPR>(exp.Cast());
    }
}

#endif
