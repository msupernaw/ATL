/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Expression.hpp
 * Author: matthewsupernaw
 *
 * Created on October 31, 2016, 12:55 PM
 */

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <cassert>
#include "Tape.hpp"

#include <sstream>
#include <ostream>
#include <complex>

namespace atl {


    enum ExpressionType {
        ET_BASE = 0,
        REAL_SCALAR,
        VARIABLE_SCALAR,
        VARIABLE_SCALAR_EXPRESSION,
        REAL_MATRIX,
        VARIABLE_MATRIX,
        REAL_MATRIX_EXPRESSION,
        VARIABLE_MATRIX_EXPRESSION,
        REAL_VECTOR,
        VARIABLE_VECTOR,
        REAL_VECTOR_EXPRESSION,
        VARIABLE_VECTOR_EXPRESSION
    };

    enum BinaryMethod {
        EXP_EXP = 0,
        EXP_REAL,
        REAL_EXP
    };

    template<typename T>
    struct ExpressionTrait {
        static ExpressionType et_type;
    };
    template<typename T>
    ExpressionType ExpressionTrait<T>::et_type = ET_BASE;

    template<class REAL_T, class A>
    struct ExpressionBase {

        ExpressionBase() {

        }

        /**
         * Cast this expression template to it's child
         * representation.
         * 
         * @return 
         */
        inline const A & Cast() const {
            return static_cast<const A&> (*this);
        }

        inline const REAL_T GetValue() const {
            return Cast().GetValue();
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return Cast().GetValue(i, j);
        }

        inline bool IsNonlinear() const {
            return Cast().IsNonlinear();
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            Cast().PushIds(ids);
        }

        inline void PushNLIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, bool nl = false)const {
            Cast().PushIds(ids, nl);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            Cast().PushIds(ids, i, j);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return Cast().ComplexEvaluate(x, h);
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return Cast().ToDynamic();
        }

        inline REAL_T EvaluateDerivative(uint32_t x) const {
            return Cast().EvaluateDerivative(x);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return Cast().EvaluateDerivative(x, y);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return Cast().EvaluateDerivative(x, y, z);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, size_t i, size_t j = 0, REAL_T h = 1e-20) const {
            return Cast().ComplexEvaluate(x, i, j, h);
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            return Cast().Taylor(degree);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return Cast().EvaluateDerivative(x, i, j);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return Cast().EvaluateDerivative(x, y, i, j);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return Cast().EvaluateDerivative(x, y, z, i, j);
        }

        const ExpressionBase& operator=(const ExpressionBase & exp) const {
            return *this;
        }

        size_t GetColumns() const {
            return Cast().GetColumns();
        }

        size_t GetRows() const {
            return Cast().GetRows();
        }

        bool IsScalar()const {
            return Cast().IsScalar();
        }

        /**
         * Create a string representation of this expression template. 
         * @return 
         */
        std::string ToExpressionTemplateString() const {
            return Cast().ToExpressionTemplateString();
        }

    };



}

template <class REAL_T, class T>
std::ostream & operator<<(std::ostream &vout, const atl::ExpressionBase<REAL_T, T> &e) {
    vout << e.GetValue();
    return vout;
}

template<class REAL_T, class T, class TT>
inline const int operator==(const atl::ExpressionBase<REAL_T, T>& lhs, const atl::ExpressionBase<REAL_T, TT>& rhs) {
    return lhs.GetValue() == rhs.GetValue();
}

template<class REAL_T, class T, class TT>
inline const int operator!=(const atl::ExpressionBase<REAL_T, T>& lhs, const atl::ExpressionBase<REAL_T, TT>& rhs) {
    return lhs.GetValue() != rhs.GetValue();
}

template<class REAL_T, class T, class TT>
inline const int operator<(const atl::ExpressionBase<REAL_T, T>& lhs, const atl::ExpressionBase<REAL_T, TT>& rhs) {
    return lhs.GetValue() < rhs.GetValue();
}

template<class REAL_T, class T, class TT>
inline const int operator>(const atl::ExpressionBase<REAL_T, T>& lhs, const atl::ExpressionBase<REAL_T, TT>& rhs) {
    return lhs.GetValue() > rhs.GetValue();
}

template<class REAL_T, class T, class TT>
inline const int operator<=(const atl::ExpressionBase<REAL_T, T>& lhs, const atl::ExpressionBase<REAL_T, TT>& rhs) {
    return lhs.GetValue() <= rhs.GetValue();
}

template<class REAL_T, class T, class TT>
inline const int operator>=(const atl::ExpressionBase<REAL_T, T>& lhs, const atl::ExpressionBase<REAL_T, TT>& rhs) {
    return lhs.GetValue() >= rhs.GetValue();
}

template<class REAL_T, class T>
inline const int operator==(const REAL_T &lhs, const atl::ExpressionBase<REAL_T, T>& rhs) {
    return lhs == rhs.GetValue();
}

template<class REAL_T, class T>
inline const int operator!=(const REAL_T &lhs, const atl::ExpressionBase<REAL_T, T>& rhs) {
    return lhs != rhs.GetValue();
}

template<class REAL_T, class T>
inline const int operator<(const REAL_T &lhs, const atl::ExpressionBase<REAL_T, T>& rhs) {
    return lhs < rhs.GetValue();
}

template<class REAL_T, class T>
inline const int operator>(const REAL_T &lhs, const atl::ExpressionBase<REAL_T, T>& rhs) {
    return lhs > rhs.GetValue();
}

template<class REAL_T, class T>
inline const int operator<=(const REAL_T &lhs, const atl::ExpressionBase<REAL_T, T>& rhs) {
    return lhs <= rhs.GetValue();
}

template<class REAL_T, class T>
inline const int operator>=(const REAL_T &lhs, const atl::ExpressionBase<REAL_T, T>& rhs) {
    return lhs >= rhs.GetValue();
}

template<class REAL_T, class T>
inline const int operator==(const atl::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    return lhs.GetValue() == rhs;
}

template<class REAL_T, class T>
inline const int operator!=(const atl::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    return lhs.GetValue() != rhs;
}

template<class REAL_T, class T>
inline const int operator<(const atl::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    return lhs.GetValue() <= rhs;
}

template<class REAL_T, class T>
inline const int operator>(const atl::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    return lhs.GetValue() > rhs;
}

template<class REAL_T, class T>
inline const int operator<=(const atl::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    return lhs.GetValue() <= rhs;
}

template<class REAL_T, class T>
inline const int operator>=(const atl::ExpressionBase<REAL_T, T>& lhs, const REAL_T &rhs) {
    return lhs.GetValue() >= rhs;
}

#endif /* EXPRESSION_HPP */

