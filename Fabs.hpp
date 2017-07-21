/* 
 * File:   Fabs.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:53 PM
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


#ifndef FABS_HPP
#define FABS_HPP

#include "Expression.hpp"
//#include "Pow.hpp" // for ad_fabs
//#include "Sqrt.hpp"
//#include "Variable.hpp"
//#include "Multiply.hpp"
namespace atl {

    /**
     * Expression template to handle the absolute value for variable or 
     * container expressions. Not differentiable at zero. For a differentiable
     * estimate see \ref ad_fabs.
     * 
     * \f$ \arccos f(x,y) \f$
     * 
     * or 
     * 
     * \f$ \arccos f_{i,j}(x,y) \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct Fabs : public ExpressionBase<REAL_T, Fabs<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Fabs(const Fabs<REAL_T, EXPR>& other) :
        expr_m(other.expr_m) {
        }

        /**
         * Constructor
         * 
         * @param a
         */
        Fabs(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Computes the absolute value of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::fabs(expr_m.GetValue());
        }

        /**
         * Computes the absolute value of the evaluated expression at 
         * index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::fabs(expr_m.GetValue(i, j));
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
            expr_m.PushNLIds(ids, nl);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return std::fabs(expr_m.ComplexEvaluate(x, h));
        }

        inline const REAL_T Taylor(uint32_t degree) const {


            REAL_T tmp = expr_m.Taylor(0) * expr_m.Taylor(degree);
            //            }
            if (tmp < static_cast<REAL_T> (0.0)) {
                return static_cast<REAL_T> (-1.0);
            } else if (tmp > static_cast<REAL_T> (0.0)) {
                return static_cast<REAL_T> (1.0);
            } else {
                return static_cast<REAL_T> (0.0);
            }
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return atl::fabs(expr_m.ToDynamic());
        }

        /**
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ {{f(x)\,\left({{d}\over{d\,x}}\,f(x)\right)}\over{
         *  \left| f(x)\right| }} \f$
         * 
         * note: undefined at f(x) = 0. see \ref ad_fabs for an alternative.
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {

            return (expr_m.EvaluateDerivative(x) * expr_m.GetValue()) / this->GetValue();
        }

        /**
         * Evaluates the second-order derivative with respect to x and y.
         * 
         * \f$ {{f(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,f(x,y)\right)
         *  }\over{\left| f(x,y)\right| }} \f$
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {

            return (expr_m.EvaluateDerivative(x, y) * expr_m.GetValue()) / this->GetValue();
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z 
         * at index {i,j}.
         * 
         * \f$ {{f(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f(x,y
         * ,z)\right)}\over{\left| f(x,y,z)\right| }} \f$
         * 
         * note: undefined at f(x) = 0. see \ref ad_fabs for an alternative.
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {

            return (expr_m.GetValue()*(expr_m.EvaluateDerivative(x, y, z))) / this->GetValue();
        }

        /**
         * Evaluates the first-order derivative with respect to x at index 
         * {i,j}.
         * 
         * \f$ {{f_{i,j}(x)\,\left({{d}\over{d\,x}}\,f_{i,j}(x)\right)}\over{
         *  \left| f_{i,j}(x)\right| }} \f$
         * 
         * note: undefined at f(x) = 0. see \ref ad_fabs for an alternative.
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {

            return (expr_m.EvaluateDerivative(x, i, j) * expr_m.GetValue(i, j)) / this->GetValue(i, j);
        }

        /**
         * Evaluates the second-order derivative with respect to x and y 
         * at index {i,j}.
         * 
         * \f$ {{f_{i,j}(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y)\right)
         * }\over{\left| f_{i,j}(x,y)\right| }} \f$  
         * 
         * note: undefined at f(x) = 0. see \ref ad_fabs for an alternative.
         *        
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {

            return (expr_m.EvaluateDerivative(x, y, i, j) * expr_m.GetValue(i, j)) / this->GetValue(i, j);
        }

        /**
         * Evaluates the third-order derivative with respect to x, y, and z 
         * at index {i,j}.
         * 
         * \f$ {{f_{i,j}(x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y
         * ,z)\right)}\over{\left| f_{i,j}(x,y,z)\right| }} \f$
         * 
         * note: undefined at f(x) = 0. see \ref ad_fabs for an alternative.
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {

            return (expr_m.GetValue(i, j)*(expr_m.EvaluateDerivative(x, y, z, i, j))) / this->GetValue(i, j);
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
            ss << "atl::Fabs<T," << expr_m.ToExpressionTemplateString() << " >";

            return ss.str();
        }


        const EXPR& expr_m;
    };

    template<class REAL_T, class EXPR>
    const Fabs<REAL_T, EXPR> fabs(const ExpressionBase<REAL_T, EXPR>& exp) {

        return Fabs<REAL_T, EXPR>(exp.Cast());
    }

    template<class REAL_T, class EXPR>
    inline const atl::Fabs<REAL_T, EXPR> abs(const atl::ExpressionBase<REAL_T, EXPR>& expr) {

        return atl::Fabs<REAL_T, EXPR > (expr.Cast());
    }



    /**
     * 
     * Used when expr could evaluate to zero, which will result in a NaN for 
     * derivative values. 
     * 
     * Evaluates:
     * 
     * \f$ (expr^2+C)^.5 \f$
     * 
     * @param expr
     * @param C default = 1e-5
     * @return 
     */
    template<class REAL_T, class EXPR>
    const atl::Variable<REAL_T> ad_fabs(const atl::ExpressionBase<REAL_T, EXPR>& expr, atl::Variable<REAL_T> C = 1e-5) {
        return atl::sqrt((expr * expr) + C); //, .5);
    }

#define AD_FABS(x) atl::sqrt((x * x) +1e-5)




}//end namespace atl

namespace std {

    template<class REAL_T, class EXPR>
    const atl::Variable<REAL_T> fabs(const atl::ExpressionBase<REAL_T, EXPR>& expr, atl::Variable<REAL_T> C = 1e-5) {
        return atl::sqrt((expr * expr) + C); //, .5);
    }
}


#endif
