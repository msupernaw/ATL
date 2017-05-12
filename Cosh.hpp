/* 
 * File:   Cosh.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:50 PM
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

#ifndef COSH_HPP
#define COSH_HPP

#include "Expression.hpp"

namespace atl {

    /**
     * Expression template to handle hyperbolic cosine for variable or 
     * container expressions. 
     * 
     * \f$ \cosh f(x,y) \f$
     * 
     * or 
     * 
     * \f$ \cosh f_{i,j}(x,y) \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct Cosh : public ExpressionBase<REAL_T, Cosh<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Cosh(const Cosh<REAL_T, EXPR>& other) :
        expr_m(other.expr_m), sinh_(other.sinh_), val_(other.val_) {
        }

        /**
         * Constructor. 
         * 
         * @param a
         */
        Cosh(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Computes the hyperbolic cosine of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::cosh(expr_m.GetValue());
        }

        /**
         * Computes the hyperbolic cosine of the evaluated expression at 
         * index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::cosh(expr_m.GetValue(i, j));
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
         *  Push variable info into a set. 
         * 
         * @param ids
         * @param i
         * @param j
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            expr_m.PushIds(ids);
        }

        /**
         *  Push variable info into a set at index {i,j}. 
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
            return std::cosh(expr_m.ComplexEvaluate(x, h));
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            if (degree == 0) {
                sinh_.reserve(5);
                val_.reserve(5);
                sinh_.resize(1);
                val_.resize(1);
                sinh_[0] = std::sinh(this->expr_m.Taylor(0));
                val_[0] = std::cosh(this->expr_m.Taylor(0));
                return val_[0];
            }

            size_t l = val_.size();
            sinh_.resize(degree + 1);
            val_.resize(degree + 1);

            for (int i = l; i <= degree; i++) {
                val_[i] = static_cast<REAL_T> (0.0);
                sinh_[i] = static_cast<REAL_T> (0.0);
                for (unsigned int j = 0; j <= i; ++j) {
                    sinh_[i] += static_cast<REAL_T> (j) * expr_m.Taylor(j) * val_[i - j];
                    val_[i] += static_cast<REAL_T> (j) * expr_m.Taylor(j) * sinh_[i - j];
                }
                sinh_[i] /= static_cast<REAL_T> (i);
                val_[i] /= static_cast<REAL_T> (i);
            }
            return val_[degree];
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return atl::cosh(expr_m.ToDynamic());
        }

        /**
         * 
         * Evaluates the first-order derivative of this expression with respect 
         * to x.
         * 
         * \f$ \sinh f(x)\,\left({{d}\over{d\,x}}\,f(x)\right)\f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {
            return expr_m.EvaluateDerivative(x) * std::sinh(expr_m.GetValue());
        }

        /**
         * 
         * Evaluates the second-order derivative of this expression with respect 
         * to x and y.
         * 
         * \f$ \cosh f\left(x , y\right)\,\left({{d}\over{d\,x}}\,f\left(x , y
         * \right)\right)\,\left({{d}\over{d\,y}}\,f\left(x , y\right)\right)+
         * \sinh f\left(x , y\right)\,\left({{d^2}\over{d\,x\,d\,y}}\,f\left(x
         * , y\right)\right)\f$
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return ((std::cosh(expr_m.GetValue()) * expr_m.EvaluateDerivative(x) * expr_m.EvaluateDerivative(y))
                    + (std::sinh(expr_m.GetValue()) * expr_m.EvaluateDerivative(x, y)));
        }

        /**
         * 
         * Evaluates the third-order derivative of this expression with respect 
         * to x, y and z.
         * 
         * \f$ \sinh f\left(x , y , z\right)\,\left({{d}\over{d\,x}}\,f\left(x , y
         * , z\right)\right)\,\left({{d}\over{d\,y}}\,f\left(x , y , z\right)
         * \right)\,\left({{d}\over{d\,z}}\,f\left(x , y , z\right)\right)+
         * \cosh f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d\,y}}\,f
         * \left(x , y , z\right)\right)\,\left({{d}\over{d\,z}}\,f\left(x , y
         *  , z\right)\right)+\cosh f\left(x , y , z\right)\,\left({{d}\over{d
         * \,x}}\,f\left(x , y , z\right)\right)\,\left({{d^2}\over{d\,y\,d\,z
         * }}\,f\left(x , y , z\right)\right)+\cosh f\left(x , y , z\right)\,
         * \left({{d^2}\over{d\,x\,d\,z}}\,f\left(x , y , z\right)\right)\,
         * \left({{d}\over{d\,y}}\,f\left(x , y , z\right)\right)+\sinh f\left(
         * x , y , z\right)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f\left(x , y
         * , z\right)\right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return std::sinh(expr_m.GetValue())*(expr_m.EvaluateDerivative(x))
                    *(expr_m.EvaluateDerivative(y))*(expr_m.EvaluateDerivative(z))
                    + std::cosh(expr_m.GetValue())*(expr_m.EvaluateDerivative(x, y))
                    *(expr_m.EvaluateDerivative(z)) + std::cosh(expr_m.GetValue())
                    *(expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y, z))
                    + std::cosh(expr_m.GetValue())*(expr_m.EvaluateDerivative(x, z))
                    *(expr_m.EvaluateDerivative(y)) + std::sinh(expr_m.GetValue())
                    *(expr_m.EvaluateDerivative(x, y, z));

        }

        /**
         * 
         * Evaluates the first-order derivative of this expression with respect 
         * to x at index {i,i}.
         * 
         * \f$ \sinh f_{i,j}(x)\,\left({{d}\over{d\,x}}\,f_{i,j}(x)\right)\f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return expr_m.EvaluateDerivative(x, i, j) * std::sinh(expr_m.GetValue(i, j));
        }

        /**
         * Evaluates the second-order derivative of this expression with respect 
         * to x and y at index {i,i}.
         * 
         * \f$ \cosh f_{i,j}(x,y)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y)\right)\,
         * \left({{d}\over{d\,y}}\,f_{i,j}(x,y)\right)+\sinh f_{i,j}(x,y)\,
         * \left({{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y)\right) \f$ 
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return ((std::cosh(expr_m.GetValue(i, j)) * expr_m.EvaluateDerivative(x, i, j) * expr_m.EvaluateDerivative(y, i, j))
                    + (std::sinh(expr_m.GetValue(i, j)) * expr_m.EvaluateDerivative(x, y, i, j)));
        }

        /**
         * Evaluates the third-order derivative of this expression with respect 
         * to x, y and z at index {i,i}.
         * 
         * \f$ \sinh f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)
         * \,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,
         * z}}\,f_{i,j}(x,y,z)\right)+\cosh f_{i,j}(x,y,z)\,\left({{d^2}\over{d
         * \,x\,d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j}(
         * x,y,z)\right)+ \\ \cosh f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,f_{i,j}(
         * x,y,z)\right)\,\left({{d^2}\over{d\,y\,d\,z}}\,f_{i,j}(x,y,z)\right)
         * +\cosh f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,f_{i,j}(x,y,z
         * )\right)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)+\sinh f_{i,j
         * }(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)
         * \right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return std::sinh(expr_m.GetValue(i, j))*(expr_m.EvaluateDerivative(x, i, j))
                    *(expr_m.EvaluateDerivative(y, i, j))*(expr_m.EvaluateDerivative(z, i, j))
                    + std::cosh(expr_m.GetValue(i, j))*(expr_m.EvaluateDerivative(x, y, i, j))
                    *(expr_m.EvaluateDerivative(z, i, j)) + std::cosh(expr_m.GetValue(i, j))
                    *(expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, z, i, j))
                    + std::cosh(expr_m.GetValue(i, j))*(expr_m.EvaluateDerivative(x, z, i, j))
                    *(expr_m.EvaluateDerivative(y, i, j)) + std::sinh(expr_m.GetValue(i, j))
                    *(expr_m.EvaluateDerivative(x, y, z, i, j));
        }

        /**
         * Return the number of columns.
         *
         * @return
         */
        size_t GetColumns() const {
            return expr_m.GetColumns();
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
            ss << "atl::Cosh<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }

        const EXPR& expr_m;
        mutable std::vector<REAL_T> sinh_;
        mutable std::vector<REAL_T> val_;
    };

    /**
     * Returns an expression template representing hyperbolic cosine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const Cosh<REAL_T, EXPR> cosh(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Cosh<REAL_T, EXPR>(exp.Cast());
    }



}//end namespace atl

#endif
