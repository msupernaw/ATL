/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Add.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 12:43 PM
 */

#ifndef ADD_HPP
#define ADD_HPP

//#include "Traits.hpp"
#include "Real.hpp"
#include "Expression.hpp"
#include <cassert>


namespace atl {

    /**
     * Expression template to handle addition.
     * 
     * \f$ f(x) + g(x) \f$
     * 
     * or
     * 
     * \f$ f_{i,j}(x) + g_{i,j}(x) \f$
     */
    template <class REAL_T, class LHS, class RHS>
    struct Add : public ExpressionBase<REAL_T, Add<REAL_T, LHS, RHS> > {
        typedef REAL_T BASE_TYPE;

        Add(const Add<REAL_T, LHS, RHS>& other) :
        real_m(other.real_m), lhs_m(other.lhs_m), rhs_m(other.rhs_m), value_m(other.value_m) {
        }

        /**
         * Constructor for two variable types.
         * 
         * @param lhs
         * @param rhs
         */
        Add(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {

        }

        /**
         * Constructor for real plus variable type.
         * 
         */
        Add(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        /**
         * Constructor for variable plus real type.
         * @param lhs
         * @param rhs
         */
        Add(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;
        }

        /**
         * Compute the addition of the lhs and rhs expressions.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() + rhs_m.GetValue();
        }

        /**
         * Compute the addition of the lhs and rhs expressions at index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return lhs_m.GetValue(i, j) + rhs_m.GetValue(i, j);
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
            return lhs_m.ComplexEvaluate(x, h) + rhs_m.ComplexEvaluate(x, h);
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            return lhs_m.Taylor(degree) + rhs_m.Taylor(degree);
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
#ifdef USE_DELEGATES
            return atl::AddFunctions<REAL_T>::Create(lhs_m.ToDynamic(),rhs_m.ToDynamic());
#else
            return std::make_shared<atl::AddDynamic<REAL_T> >(lhs_m.ToDynamic(), rhs_m.ToDynamic());
#endif
        }

        /**
         * Evaluates the first-order derivative with respect 
         * to x.
         * 
         * \f$ {{d}\over{d\,x}}\,g\left(x\right)+{{d}\over{d\,x}}\,f\left(x\right) \f$
         * 
         * @param x
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x) const {
            return lhs_m.EvaluateDerivative(x) + rhs_m.EvaluateDerivative(x);
        }

        /**
         * Evaluates the second-order derivative with respect to x and y.
         * 
         * \f$ {{d^2}\over{d\,x\,d\,y}}\,g\left(x , y\right)+{{d^2}\over{d\,x\,d\,
         * y}}\,f\left(x , y\right) \f$
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return lhs_m.EvaluateDerivative(x, y) + rhs_m.EvaluateDerivative(x, y);
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z.
         * 
         * \f$ {{d^3}\over{d\,x\,d\,y\,d\,z}}\,g\left(x , y , z\right)+{{d^3
         * }\over{d\,x\,d\,y\,d\,z}}\,f\left(x , y , z\right) \f$
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return lhs_m.EvaluateDerivative(x, y, z) + rhs_m.EvaluateDerivative(x, y, z);
        }

        /**
         * Evaluates the first-order derivative of this expression with respect 
         * to x at index {i,j}.
         * 
         * @param a
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return lhs_m.EvaluateDerivative(x, i, j) + rhs_m.EvaluateDerivative(x, i, j);
        }

        /**
         * Evaluates the second-order derivative of this expression with respect 
         * to x at index {i,j}.
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return lhs_m.EvaluateDerivative(x, y, i, j) + rhs_m.EvaluateDerivative(x, y, i, j);
        }

        /**
         * 
         * Evaluates the second-order derivative of this expression with respect 
         * to x at index {i,j}.
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return lhs_m.EvaluateDerivative(x, y, z, i, j) + rhs_m.EvaluateDerivative(x, y, z, i, j);
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
         * @return 
         */
        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::Add<T," << lhs_m.ToExpressionTemplateString() << ", " << rhs_m.ToExpressionTemplateString() << " >";
            return ss.str();
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
    inline const Add<REAL_T, LHS, RHS> operator+(const ExpressionBase<REAL_T, LHS>& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Add<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    /**
     * Operator for addition of an expression template and a real value.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS>
    inline const Add<REAL_T, LHS, Real<REAL_T> > operator+(const ExpressionBase<REAL_T, LHS>& a,
            REAL_T b) {

        return Add<REAL_T, LHS, Real<REAL_T> > (a.Cast(), b);
    }

    /**
     * Operator for the addition of a real value and an expression template.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class RHS>
    inline const Add<REAL_T, Real<REAL_T>, RHS> operator+(const REAL_T& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Add<REAL_T, Real<REAL_T>, RHS > (a, b.Cast());
    }


}


#endif /* ADD_HPP */

