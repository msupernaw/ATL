/* 
 * File:   Floor.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:54 PM
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


#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "Expression.hpp"

namespace atl {

    /**
     * Expression template to handle cosine for variable or 
     * container expressions. 
     * 
     * \f$ \floor f(x,y) \f$
     * 
     * or 
     * 
     * \f$ \floor f_{i,j}(x,y) \f$
     * 
     * 
     * This is not differentiable and calls to the derivative member functions
     * result in 0. 
     * 
     * 
     */
    template<class REAL_T, class EXPR>
    struct Floor : public ExpressionBase<REAL_T, Floor<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        /**
         * Constructor
         * 
         * @param a
         */
        Floor(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Compute the floor of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::floor(expr_m.GetValue());
        }

        /**
         * Compute the floor of the evaluated expression at index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::floor(expr_m.GetValue(i, j));
        }

        /**
         * Returns false.
         * 
         * @return 
         */
        inline bool IsNonlinear() const {
            return false;
        }

        /**
         *  Push variable info into a set. 
         * 
         * @param ids
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
            expr_m.PushNLIds(ids, nl);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return std::floor(expr_m.ComplexEvaluate(x, h));
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param id
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t id) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param a
         * @param b
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param id
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t id, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param a
         * @param b
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
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
            ss << "atl::Floor<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }


        const EXPR& expr_m;
    };

    /**
     * Returns an expression template representing floor.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const Floor<REAL_T, EXPR> floor(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Floor<REAL_T, EXPR>(exp.Cast());
    }

}//end namespace atl

#endif
