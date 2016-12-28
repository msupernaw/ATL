#ifndef ATAN_HPP
#define ATAN_HPP

#include "Expression.hpp"

namespace atl {

    template<class REAL_T, class EXPR>
    struct ATan : public ExpressionBase<REAL_T, ATan<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        ATan(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::atan(expr_m.GetValue());
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::atan(expr_m.GetValue(i, j));
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            expr_m.PushIds(ids);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            expr_m.PushIds(ids, i, j);
        }

        inline const REAL_T EvaluateDerivative(uint32_t id) const {
            REAL_T fx = expr_m.GetValue();
            return expr_m.EvaluateDerivative(id) / (fx * fx + 1.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            REAL_T fx = expr_m.GetValue();
            return (expr_m.EvaluateDerivative(a, b) / (fx * fx + 1.0)) -
                    (2.0 * fx * expr_m.EvaluateDerivative(a) * expr_m.EvaluateDerivative(b)) / ((fx * fx + 1.0)*(fx * fx + 1.0));
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return -(2.0 * (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))
                    *(expr_m.EvaluateDerivative(z))) /
                    std::pow((std::pow(expr_m.GetValue(), 2.0) + 1), 2.0)+
                    (8.0 * std::pow(expr_m.GetValue(), 2.0)*
                    (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))*
                    (expr_m.EvaluateDerivative(z))) /
                    std::pow((std::pow(expr_m.GetValue(), 2.0) + 1), 3.0)-
                    (2.0 * expr_m.GetValue()*(expr_m.EvaluateDerivative(x, y))*
                    (expr_m.EvaluateDerivative(z))) /
                    std::pow((std::pow(expr_m.GetValue(), 2.0) + 1), 2.0)-
                    (2.0 * expr_m.GetValue()*(expr_m.EvaluateDerivative(x))*
                    (expr_m.EvaluateDerivative(y, z))) /
                    std::pow((std::pow(expr_m.GetValue(), 2.0) + 1), 2.0)-
                    (2.0 * expr_m.GetValue()*(expr_m.EvaluateDerivative(x, z))*
                    (expr_m.EvaluateDerivative(y))) /
                    std::pow((std::pow(expr_m.GetValue(), 2.0) + 1), 2.0) +
                    expr_m.EvaluateDerivative(x, y, z) /
                    (std::pow(expr_m.GetValue(), 2.0) + 1);
        }

        inline const REAL_T EvaluateDerivative(uint32_t id, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return expr_m.EvaluateDerivative(id, i, j) / (fx * fx + 1.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return (expr_m.EvaluateDerivative(a, b, i, j) / (fx * fx + 1.0)) -
                    (2.0 * fx * expr_m.EvaluateDerivative(a, i, j) * expr_m.EvaluateDerivative(b, i, j)) / ((fx * fx + 1.0)*(fx * fx + 1.0));
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return -(2.0 * (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))
                    *(expr_m.EvaluateDerivative(z, i, j))) /
                    std::pow((std::pow(expr_m.GetValue(i, j), 2.0) + 1), 2.0)+
                    (8.0 * std::pow(expr_m.GetValue(i, j), 2.0)*
                    (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))*
                    (expr_m.EvaluateDerivative(z, i, j))) /
                    std::pow((std::pow(expr_m.GetValue(i, j), 2.0) + 1), 3.0)-
                    (2.0 * expr_m.GetValue(i, j)*(expr_m.EvaluateDerivative(x, y, i, j))*
                    (expr_m.EvaluateDerivative(z, i, j))) /
                    std::pow((std::pow(expr_m.GetValue(i, j), 2.0) + 1), 2.0)-
                    (2.0 * expr_m.GetValue(i, j)*(expr_m.EvaluateDerivative(x, i, j))*
                    (expr_m.EvaluateDerivative(y, z, i, j))) /
                    std::pow((std::pow(expr_m.GetValue(i, j), 2.0) + 1), 2.0)-
                    (2.0 * expr_m.GetValue(i, j)*(expr_m.EvaluateDerivative(x, z, i, j))*
                    (expr_m.EvaluateDerivative(y, i, j))) /
                    std::pow((std::pow(expr_m.GetValue(i, j), 2.0) + 1), 2.0) +
                    expr_m.EvaluateDerivative(x, y, z, i, j) /
                    (std::pow(expr_m.GetValue(i, j), 2.0) + 1);
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
    inline const ATan<REAL_T, EXPR> atan(const ExpressionBase<REAL_T, EXPR>& exp) {
        return ATan<REAL_T, EXPR>(exp.Cast());
    }

}//end namespace atl

#endif
