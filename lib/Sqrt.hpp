/* 
 * File:   Sqrt.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:31 PM
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



#ifndef SQRT_HPP
#define SQRT_HPP

#include "Expression.hpp"

#include "Real.hpp"

namespace atl {

  
    /**
     * Expression template to handle square root of a variable or 
     * container expressions. 
     * 
     * \f$  \sqrt f(x) \f$
     * 
     * or 
     * 
     * \f$ \sqrt f_{i,j}(x)  \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct Sqrt : public ExpressionBase<REAL_T, Sqrt<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Sqrt(const Sqrt<REAL_T, EXPR>& other) :
        expr_m(other.expr_m) {
        }

        /**
         * Constructor 
         * @param a
         */
        Sqrt(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Compute the square root of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::sqrt(expr_m.GetValue());
        }

        /**
         * Compute the square root of the evaluated expression at index 
         * {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::sqrt(expr_m.GetValue(i, j));
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
            return std::sqrt(expr_m.ComplexEvaluate(x, h));
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            if (degree == 0) {
                val_.reserve(5);
                val_.resize(1);
                val_[0] = std::sqrt(this->expr_m.Taylor(0));
                return val_[0];
            }

            size_t l = val_.size();


            for (int i = l; i <= degree; i++) {
                val_[i] = static_cast<REAL_T> (0.0);

                for (int k = 1; k < i; k++) {
                    val_[i] -= static_cast<REAL_T> (k) * val_[k] * val_[i - k];
                }
                val_[i] /= static_cast<REAL_T> (i);
                val_[i] += expr_m.Taylor(i) / static_cast<REAL_T> (2.0);
                val_[i] /= val_[0];
            }
            return val_[degree];
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return atl::sqrt(expr_m.ToDynamic());
        }

        /**
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f(x)}\over{2\,\sqrt{f(x)}}} \f$
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateFirstDerivative(uint32_t x) const {
            return expr_m.EvaluateFirstDerivative(x) / (2.0 * this->GetValue());
        }

        /**
         * Evaluates the second-order derivative with respect to x and y.
         * 
         * \f$ {{{{d^2}\over{d\,x\,d\,y}}\,f(x,y)}\over{2\,\sqrt{f(x,y
         *  )}}}-{{{{d}\over{d\,x}}\,f(x,y)\,\left({{d}\over{d\,y}}\,
         * f(x,y)\right)}\over{4\,f(x,y)^{{{3}\over{2}}}}} \f$
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateSecondDerivative(uint32_t x, uint32_t y) const {
            return (expr_m.EvaluateSecondDerivative(x, y) / (2.0 * this->GetValue())) -
                    (expr_m.EvaluateFirstDerivative(x) * expr_m.EvaluateFirstDerivative(y)) / (4.0 * std::pow(expr_m.GetValue(), 1.5));
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z.
         * 
         * \f$ {{3\,\left({{d}\over{d\,x}}\,f(x,y,z)\right)\,\left({{d
         *  }\over{d\,y}}\,f(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j
         *  }(x,y,z)\right)}\over{8\,f(x,y,z)^{{{5}\over{2}}}}}-{{{{d^2
         *  }\over{d\,x\,d\,y}}\,f(x,y,z)\,\left({{d}\over{d\,z}}\,f
         *  (x,y,z)\right)}\over{4\,f(x,y,z)^{{{3}\over{2}}}}}-{{{{d
         *  }\over{d\,x}}\,f(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,f_{i,
         *  j}(x,y,z)\right)}\over{4\,f(x,y,z)^{{{3}\over{2}}}}}-{{{{d^2
         *  }\over{d\,x\,d\,z}}\,f(x,y,z)\,\left({{d}\over{d\,y}}\,f
         *  (x,y,z)\right)}\over{4\,f(x,y,z)^{{{3}\over{2}}}}}+{{{{d^3
         *  }\over{d\,x\,d\,y\,d\,z}}\,f(x,y,z)}\over{2\,\sqrt{f(x,y
         *  ,z)}}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateThirdDerivative(uint32_t x, uint32_t y, uint32_t z) const {

            return (3.0 * (expr_m.EvaluateFirstDerivative(x))*(expr_m.EvaluateFirstDerivative(y))
                    *(expr_m.EvaluateFirstDerivative(z))) / (8.0 * std::pow(expr_m.GetValue(), 2.5))
                    -((expr_m.EvaluateSecondDerivative(x, y))*(expr_m.EvaluateFirstDerivative(z)))
                    / (4.0 * std::pow(expr_m.GetValue(), 1.5))-((expr_m.EvaluateFirstDerivative(x))
                    *(expr_m.EvaluateSecondDerivative(y, z))) / (4.0 * std::pow(expr_m.GetValue(), 1.5))
                    -((expr_m.EvaluateSecondDerivative(x, z))*(expr_m.EvaluateFirstDerivative(y)))
                    / (4.0 * std::pow(expr_m.GetValue(), 1.5)) + expr_m.EvaluateThirdDerivative(x, y, z)
                    / (2.0 * std::sqrt(expr_m.GetValue()));
        }

        /**
         * Evaluates the first-order derivative with respect to x at index {i,j}.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f_{i,j}(x)}\over{2\,\sqrt{f_{i,j}(x)}}} \f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateFirstDerivativeAt(uint32_t x, size_t i, size_t j = 0) const {

            return expr_m.EvaluateFirstDerivativeAt(x, i, j) / (2.0 * this->GetValue(i, j));
        }

        /**
         * Evaluates the second-order derivative with respect to x and y at index
         * {i,j}.
         * 
         * \f$ {{{{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y)}\over{2\,\sqrt{f_{i,j}(x,y
         *  )}}}-{{{{d}\over{d\,x}}\,f_{i,j}(x,y)\,\left({{d}\over{d\,y}}\,f_{i,
         *  j}(x,y)\right)}\over{4\,f_{i,j}(x,y)^{{{3}\over{2}}}}} \f$
         * 
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateSecondDerivativeAt(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {

            return (expr_m.EvaluateSecondDerivativeAt(x, y, i, j) / (2.0 * this->GetValue(i, j))) -
                    (expr_m.EvaluateFirstDerivativeAt(x, i, j) * expr_m.EvaluateFirstDerivativeAt(y, i, j)) / (4.0 * std::pow(expr_m.GetValue(i, j), 1.5));
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z
         * at index {i,j}.
         * 
         * \f$ {{3\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         *  }\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j
         *  }(x,y,z)\right)}\over{8\,f_{i,j}(x,y,z)^{{{5}\over{2}}}}}-{{{{d^2
         *  }\over{d\,x\,d\,y}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,z}}\,f_{i,j}
         *  (x,y,z)\right)}\over{4\,f_{i,j}(x,y,z)^{{{3}\over{2}}}}}-{{{{d
         *  }\over{d\,x}}\,f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,y\,d\,z}}\,f_{i,
         *  j}(x,y,z)\right)}\over{4\,f_{i,j}(x,y,z)^{{{3}\over{2}}}}}-{{{{d^2
         *  }\over{d\,x\,d\,z}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,y}}\,f_{i,j}
         *  (x,y,z)\right)}\over{4\,f_{i,j}(x,y,z)^{{{3}\over{2}}}}}+{{{{d^3
         *  }\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)}\over{2\,\sqrt{f_{i,j}(x,y
         *  ,z)}}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateThirdDerivativeAt(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {

            return (3.0 * (expr_m.EvaluateFirstDerivativeAt(x, i, j))*(expr_m.EvaluateFirstDerivativeAt(y, i, j))
                    *(expr_m.EvaluateFirstDerivativeAt(z, i, j))) / (8.0 * std::pow(expr_m.GetValue(i, j), 2.5))
                    -((expr_m.EvaluateSecondDerivativeAt(x, y, i, j))*(expr_m.EvaluateFirstDerivativeAt(z, i, j)))
                    / (4.0 * std::pow(expr_m.GetValue(i, j), 1.5))-((expr_m.EvaluateFirstDerivativeAt(x, i, j))
                    *(expr_m.EvaluateDerivative(y, z, i, j))) / (4.0 * std::pow(expr_m.GetValue(i, j), 1.5))
                    -((expr_m.EvaluateSecondDerivativeAt(x, z, i, j))*(expr_m.EvaluateFirstDerivativeAt(y, i, j)))
                    / (4.0 * std::pow(expr_m.GetValue(i, j), 1.5)) + expr_m.EvaluateThirdDerivativeAt(x, y, z, i, j)
                    / (2.0 * std::sqrt(expr_m.GetValue(i, j)));
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
            ss << "atl::Sqrt<T," << expr_m.ToExpressionTemplateString() << " >";

            return ss.str();
        }


        const EXPR& expr_m;
        mutable std::vector<REAL_T> val_;
    };

    /**
     * Returns an expression template representing the square root.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const Sqrt<REAL_T, EXPR> sqrt(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Sqrt<REAL_T, EXPR>(exp.Cast());
    }


}//end namespace atl

#endif

