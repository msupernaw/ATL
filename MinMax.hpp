/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MinMax.hpp
 * Author: matthewsupernaw
 *
 * Created on December 28, 2016, 11:24 AM
 */

#ifndef MINMAX_HPP
#define MINMAX_HPP

#include "Add.hpp"
#include "Divide.hpp"
#include "Subtract.hpp"
#include "Fabs.hpp"
#include "Multiply.hpp"
#include "Matrix.hpp"

namespace atl {

    template <class REAL_T, class LHS, class RHS>
    struct Min : public ExpressionBase<REAL_T, Min<REAL_T, LHS, RHS> > {
        typedef REAL_T BASE_TYPE;

        /**
         * Constructor for two variable types.
         * 
         * @param lhs
         * @param rhs
         */
        Min(const ExpressionBase<REAL_T, LHS>& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(lhs.Cast()), rhs_m(rhs.Cast()) {

        }

        /**
         * Constructor for real plus variable type.
         * 
         */
        Min(const REAL_T& lhs, const ExpressionBase<REAL_T, RHS>& rhs)
        : lhs_m(real_m), rhs_m(rhs.Cast()) {
            real_m.value = lhs;
        }

        /**
         * Constructor for variable plus real type.
         * @param lhs
         * @param rhs
         */
        Min(const ExpressionBase<REAL_T, LHS>& lhs, const REAL_T& rhs)
        : lhs_m(lhs.Cast()), rhs_m(real_m) {
            real_m.value = rhs;
        }

        /**
         * Compute the min of the lhs and rhs expressions.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            //(a + b - atl::fabs(a - b))*.5;
            return (lhs_m.GetValue() + rhs_m.GetValue() - std::fabs(lhs_m.GetValue() - rhs_m.GetValue()))*.5;
        }

        /**
         * Compute the min of the lhs and rhs expressions at index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {

            return lhs_m.GetValue(i, j) + rhs_m.GetValue(i, j);
        }

        /**
         *  Push variable info into a set. 
         * 
         * @param ids
         * @param i
         * @param j
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

        /**
         * Evaluates the first-order derivative of this expression with respect 
         * to x.
         * 
         * \f$ {{d}\over{d\,x}}\,g\left(x\right)+{{d}\over{d\,x}}\,f\left(x\right) \f$
         * 
         * @param x
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x) const {
            return 0.5 * (-(1.0 * (lhs_m.GetValue() - rhs_m.GetValue())*
                    (lhs_m.EvaluateDerivative(x) - lhs_m.EvaluateDerivative(x))) /
                    std::pow(C + std::pow(lhs_m.GetValue() - rhs_m.GetValue(), 2.0), 0.5) +
                    lhs_m.EvaluateDerivative(x) + lhs_m.EvaluateDerivative(x));
        }

        /**
         * \f$ {{d^2}\over{d\,x\,d\,y}}\,g\left(x , y\right)+{{d^2}\over{d\,x\,d\,
         * y}}\,f\left(x , y\right) \f$
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return 0.5 * (-(1.0 * (lhs_m.EvaluateDerivative(x) -
                    rhs_m.EvaluateDerivative(x))*(lhs_m.EvaluateDerivative(y)
                    - rhs_m.EvaluateDerivative(y))) / std::pow(C + std::pow(lhs_m.GetValue() -
                    rhs_m.GetValue(), 2.0), 0.5) - (1.0 * (lhs_m.GetValue() -
                    rhs_m.GetValue())*(lhs_m.EvaluateDerivative(x, y) -
                    rhs_m.EvaluateDerivative(x, y))) / std::pow(C + std::pow(lhs_m.GetValue() -
                    rhs_m.GetValue(), 2), 0.5) +
                    (1.0 * std::pow(lhs_m.GetValue() - rhs_m.GetValue(), 2.0) *
                    (lhs_m.EvaluateDerivative(x) - rhs_m.EvaluateDerivative(x))*
                    (lhs_m.EvaluateDerivative(y) - rhs_m.EvaluateDerivative(y))) /
                    std::pow(C + std::pow(lhs_m.GetValue() - rhs_m.GetValue(), 2.0), 1.5) +
                    rhs_m.EvaluateDerivative(x, y) + lhs_m.EvaluateDerivative(x, y));

        }

        /**
         * \f$ {{d^3}\over{d\,x\,d\,y\,d\,z}}\,g\left(x , y , z\right)+{{d^3
         * }\over{d\,x\,d\,y\,d\,z}}\,f\left(x , y , z\right) \f$
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return 0.5 * (-((rhs_m.GetValue() - lhs_m.GetValue())*(rhs_m.EvaluateDerivative(x, y, z) - lhs_m.EvaluateDerivative(x, y, z)))
                    / std::fabs(rhs_m.GetValue() - lhs_m.GetValue()) + rhs_m.EvaluateDerivative(x, y, z) + lhs_m.EvaluateDerivative(x, y, z));
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
            return 0.5 * (-(1.0 * (lhs_m.GetValue(i, j) - rhs_m.GetValue(i, j))*
                    (lhs_m.EvaluateDerivative(x, i, j) - lhs_m.EvaluateDerivative(x, i, j))) /
                    std::pow(C + std::pow(lhs_m.GetValue(i, j) - rhs_m.GetValue(i, j), 2.0), 0.5) +
                    lhs_m.EvaluateDerivative(x, i, j) + lhs_m.EvaluateDerivative(x, i, j));
        }

        /**
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return 0.5 * (-(1.0 * (lhs_m.EvaluateDerivative(x, i, j) -
                    rhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, i, j)
                    - rhs_m.EvaluateDerivative(y, i, j))) / std::pow(C + std::pow(lhs_m.GetValue() -
                    rhs_m.GetValue(), 2.0), 0.5) - (1.0 * (lhs_m.GetValue() -
                    rhs_m.GetValue())*(lhs_m.EvaluateDerivative(x, y, i, j) -
                    rhs_m.EvaluateDerivative(x, y, i, j))) / std::pow(C + std::pow(lhs_m.GetValue() -
                    rhs_m.GetValue(), 2), 0.5) +
                    (1.0 * std::pow(lhs_m.GetValue() - rhs_m.GetValue(), 2.0) *
                    (lhs_m.EvaluateDerivative(x, i, j) - rhs_m.EvaluateDerivative(x, i, j))*
                    (lhs_m.EvaluateDerivative(y, i, j) - rhs_m.EvaluateDerivative(y, i, j))) /
                    std::pow(C + std::pow(lhs_m.GetValue() - rhs_m.GetValue(), 2.0), 1.5) +
                    rhs_m.EvaluateDerivative(x, y, i, j) + lhs_m.EvaluateDerivative(x, y, i, j));
        }

        /**
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            REAL_T diff = lhs_m.GetValue() - rhs_m.GetValue();
            REAL_T tmp0 = std::pow(diff, 2);
            REAL_T tmp1 = std::pow(C + std::pow(diff, 2), .5);
            REAL_T tmp2 = std::pow(C + std::pow(diff, 2.0), 1.5);
            REAL_T tmp3 = (1.0 * std::pow(diff, 2.0));
            return 0.5 * (-(1.0 * (lhs_m.EvaluateDerivative(x, y, i, j) - rhs_m.EvaluateDerivative(x, y, i, j))
                    *(lhs_m.EvaluateDerivative(z, i, j) - rhs_m.EvaluateDerivative(z, i, j))) /
                    tmp1 -
                    (1.0 * (lhs_m.EvaluateDerivative(x, i, j) - rhs_m.EvaluateDerivative(x, i, j))*
                    (lhs_m.EvaluateDerivative(y, z, i, j) - rhs_m.EvaluateDerivative(y, z, i, j))) /
                    tmp1 -
                    (1.0 * (lhs_m.EvaluateDerivative(x, z, i, j) - rhs_m.EvaluateDerivative(x, z, i, j))*
                    (lhs_m.EvaluateDerivative(y, i, j) - rhs_m.EvaluateDerivative(y, i, j))) /
                    tmp1 -
                    (1.0 * (lhs_m.GetValue(i, j) - rhs_m.GetValue(i, j))*(lhs_m.EvaluateDerivative(x, y, z, i, j) -
                    rhs_m.EvaluateDerivative(x, y, z, i, j))) /
                    tmp1 +
                    (3.0 * (lhs_m.GetValue(i, j) - rhs_m.GetValue(i, j))*(lhs_m.EvaluateDerivative(x, i, j) -
                    rhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, i, j) -
                    rhs_m.EvaluateDerivative(y, i, j))*(lhs_m.EvaluateDerivative(z, i, j) -
                    rhs_m.EvaluateDerivative(z, i, j))) /
                    tmp2 +
                    (1.0 * tmp0 * (lhs_m.EvaluateDerivative(x, y, i, j) -
                    rhs_m.EvaluateDerivative(x, y, i, j))*(lhs_m.EvaluateDerivative(z, i, j) - rhs_m.EvaluateDerivative(z, i, j))) /
                    tmp2 +
                    (1.0 * tmp0 * (lhs_m.EvaluateDerivative(x, i, j) -
                    rhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, z, i, j) - rhs_m.EvaluateDerivative(y, z, i, j))) /
                    tmp2 +
                    (1.0 * tmp0 * (lhs_m.EvaluateDerivative(x, z, i, j) -
                    rhs_m.EvaluateDerivative(x, z, i, j))*(lhs_m.EvaluateDerivative(y, i, j) - rhs_m.EvaluateDerivative(y, i, j))) /
                    tmp2 -
                    (3.0 * std::pow(lhs_m.GetValue(i, j) - rhs_m.GetValue(i, j), 3.0) * (lhs_m.EvaluateDerivative(x, i, j) -
                    rhs_m.EvaluateDerivative(x, i, j))*(lhs_m.EvaluateDerivative(y, i, j) -
                    rhs_m.EvaluateDerivative(y, i, j))*(lhs_m.EvaluateDerivative(z, i, j) - rhs_m.EvaluateDerivative(z, i, j))) /
                    std::pow(C + tmp0, 2.5) +
                    rhs_m.EvaluateDerivative(x, y, z, i, j) + lhs_m.EvaluateDerivative(x, y, z, i, j));
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
        REAL_T C;
    };

    /**
     * Returns the maximum between a and b in a continuous manner using:
     * 
     * (a + b + \ref atl::ad_fabs(a - b)) *.5;
     * 
     * This is an approximation with minimal error.
     * 
     * @param a
     * @param b
     * @param C default = 1e-5
     * @return 
     */
    template <typename T>
    inline const atl::Variable<T> ad_max(const atl::Variable<T>& a, const atl::Variable<T>& b, atl::Variable<T> C = static_cast<T>(1e-5)) {
//        atl::Variable<T> aminusb = a-b;
        return (a + b + atl::ad_fabs(a-b,C))*static_cast<T>(.5);
    }





#define AD_MAX(a,b) (a + b + atl::ad_fabs(a - b))*.5

    /**
     * Returns the minimum between a and b in a continuous manner using:
     * 
     * (a + b - \ref atl::ad_fabs(a - b))*.5;
     * 
     * This is an approximation with minimal error.
     * 
     * @param a
     * @param b
     * @param C default = 1e-5
     * @return 
     */
    template <typename T>
    inline const atl::Variable<T> ad_min(const atl::Variable<T>& a, const atl::Variable<T>& b,  atl::Variable<T> C = 1e-5) {
        return  (a + b - atl::ad_fabs(a - b,C))*.5;
    }

#define AD_MIN(a,b) (a + b - atl::ad_fabs(a - b))*.5

    
//            template <class REAL_T, class LHS, class RHS>
//        inline const Min<REAL_T, LHS, RHS> min(const ExpressionBase<REAL_T, LHS>& a,
//                const ExpressionBase<REAL_T, RHS>& b) {
//            return Min<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
//        }
//            
       
    
}


#endif /* MINMAX_HPP */

