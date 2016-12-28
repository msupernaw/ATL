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

#include "Traits.hpp"
#include "Real.hpp"
#include <cassert>


namespace atl {

    template <class REAL_T, class LHS, class RHS>
    struct Add : public ExpressionBase<REAL_T, Add<REAL_T, LHS, RHS> > {
        typedef REAL_T BASE_TYPE;

        Add(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {

        }

        Add(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        Add(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;
        }

        inline const REAL_T GetValue() const {
            return lhs_m.GetValue() + rhs_m.GetValue();
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return lhs_m.GetValue(i, j) + rhs_m.GetValue(i, j);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            lhs_m.PushIds(ids);
            rhs_m.PushIds(ids);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            lhs_m.PushIds(ids, i, j);
            rhs_m.PushIds(ids, i, j);
        }

        inline REAL_T EvaluateDerivative(uint32_t a) const {
            return lhs_m.EvaluateDerivative(a) + rhs_m.EvaluateDerivative(a);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return lhs_m.EvaluateDerivative(a, b) + rhs_m.EvaluateDerivative(a, b);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return lhs_m.EvaluateDerivative(x, y, z) + rhs_m.EvaluateDerivative(x, y, z);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, size_t i, size_t j = 0) const {
            return lhs_m.EvaluateDerivative(a, i, j) + rhs_m.EvaluateDerivative(a, i, j);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            return lhs_m.EvaluateDerivative(a, b, i, j) + rhs_m.EvaluateDerivative(a, b, i, j);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return lhs_m.EvaluateDerivative(x, y, z, i, j) + rhs_m.EvaluateDerivative(x, y, z, i, j);
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

    template <class REAL_T, class LHS>
    inline const Add<REAL_T, LHS, Real<REAL_T> > operator+(const ExpressionBase<REAL_T, LHS>& a,
            REAL_T b) {

        return Add<REAL_T, LHS, Real<REAL_T> > (a.Cast(), b);
    }

    template <class REAL_T, class LHS, class RHS>
    inline const Add<REAL_T, Real<REAL_T>, RHS> operator+(const REAL_T& a,
            const ExpressionBase<REAL_T, RHS>& b) {
        return Add<REAL_T, Real<REAL_T>, RHS > (a, b.Cast());
    }


}


#endif /* ADD_HPP */

