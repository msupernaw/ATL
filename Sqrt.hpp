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

namespace atl {

    template<class REAL_T, class EXPR>
    struct Sqrt : public ExpressionBase<REAL_T, Sqrt<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Sqrt(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::sqrt(expr_m.GetValue());
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::sqrt(expr_m.GetValue(i, j));
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            expr_m.PushIds(ids);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            expr_m.PushIds(ids, i, j);
        }

        inline const REAL_T EvaluateDerivative(uint32_t id) const {
            return expr_m.EvaluateDerivative(id) / (2.0 * this->GetValue());
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return (expr_m.EvaluateDerivative(a, b) / (2.0 * this->GetValue())) -
                    (expr_m.EvaluateDerivative(a) * expr_m.EvaluateDerivative(b)) / (4.0 * std::pow(expr_m.GetValue(), 1.5));
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return (3.0 * (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))
                    *(expr_m.EvaluateDerivative(z))) / (8.0 * std::pow(expr_m.GetValue(), 2.5))
                    -((expr_m.EvaluateDerivative(x, y))*(expr_m.EvaluateDerivative(z)))
                    / (4.0 * std::pow(expr_m.GetValue(), 1.5))-((expr_m.EvaluateDerivative(x))
                    *(expr_m.EvaluateDerivative(y, z))) / (4.0 * std::pow(expr_m.GetValue(), 1.5))
                    -((expr_m.EvaluateDerivative(x, z))*(expr_m.EvaluateDerivative(y)))
                    / (4.0 * std::pow(expr_m.GetValue(), 1.5)) + expr_m.EvaluateDerivative(x, y, z)
                    / (2.0 * std::sqrt(expr_m.GetValue()));
        }

        inline const REAL_T EvaluateDerivative(uint32_t id, size_t i, size_t j = 0) const {
            return expr_m.EvaluateDerivative(id, i, j) / (2.0 * this->GetValue(i, j));
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            return (expr_m.EvaluateDerivative(a, b, i, j) / (2.0 * this->GetValue(i, j))) -
                    (expr_m.EvaluateDerivative(a, i, j) * expr_m.EvaluateDerivative(b, i, j)) / (4.0 * std::pow(expr_m.GetValue(i, j), 1.5));
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return (3.0 * (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))
                    *(expr_m.EvaluateDerivative(z, i, j))) / (8.0 * std::pow(expr_m.GetValue(i, j), 2.5))
                    -((expr_m.EvaluateDerivative(x, y, i, j))*(expr_m.EvaluateDerivative(z, i, j)))
                    / (4.0 * std::pow(expr_m.GetValue( i, j), 1.5))-((expr_m.EvaluateDerivative(x, i, j))
                    *(expr_m.EvaluateDerivative(y, z, i, j))) / (4.0 * std::pow(expr_m.GetValue( i, j), 1.5))
                    -((expr_m.EvaluateDerivative(x, z, i, j))*(expr_m.EvaluateDerivative(y, i, j)))
                    / (4.0 * std::pow(expr_m.GetValue(i, j), 1.5)) + expr_m.EvaluateDerivative(x, y, z, i, j)
                    / (2.0 * std::sqrt(expr_m.GetValue( i, j)));
        }

        size_t GetColumns() const {
            return expr_m.GetColumns();
        }

        size_t GetRows() const {
            return expr_m.GetRows();
        }

        bool IsScalar() const {
            return expr_m.IsScalar();
        }

        const EXPR& expr_m;
    };

    template<class REAL_T, class EXPR>
    inline const Sqrt<REAL_T, EXPR> sqrt(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Sqrt<REAL_T, EXPR>(exp.Cast());
    }

}//end namespace atl

#endif
