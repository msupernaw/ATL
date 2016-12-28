#ifndef ACOS_HPP
#define ACOS_HPP

#include "Expression.hpp"

namespace atl {

    template<class REAL_T, class EXPR>
    struct ACos : public ExpressionBase<REAL_T, ACos<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        ACos(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::acos(expr_m.GetValue());
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::acos(expr_m.GetValue(i, j));
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            expr_m.PushIds(ids);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            expr_m.PushIds(ids, i, j);
        }

        inline const REAL_T EvaluateDerivative(uint32_t id) const {
            REAL_T fx = expr_m.GetValue();
            return -1.0 * expr_m.EvaluateDerivative(id) / std::sqrt(1.0 - fx * fx);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            REAL_T fx = expr_m.GetValue();
            return (((-1.0 * (fx * expr_m.EvaluateDerivative(a) * expr_m.EvaluateDerivative(b)))
                    / std::pow((1.0 - fx * fx), 1.5))
                    - (expr_m.EvaluateDerivative(a, b) / std::sqrt(1.0 - fx * fx)));
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return -((expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))*
                    (expr_m.EvaluateDerivative(z))) /
                    std::pow((1 - std::pow(expr_m.GetValue(), 2.0)), (1.5))-
                    (3 * std::pow(expr_m.GetValue(), 2.0)*
                    (expr_m.EvaluateDerivative(x))*
                    (expr_m.EvaluateDerivative(y))*
                    (expr_m.EvaluateDerivative(z)))
                    / std::pow((1 - std::pow(expr_m.GetValue(), 2.0)), (2.5))-
                    (expr_m.GetValue()*(expr_m.EvaluateDerivative(x, y))*
                    (expr_m.EvaluateDerivative(z))) /
                    std::pow((1 - std::pow(expr_m.GetValue(), 2.0)), (1.5))
                    -(expr_m.GetValue()*
                    (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y, z)))
                    / std::pow((1 - std::pow(expr_m.GetValue(), 2.0)), (1.5))-
                    (expr_m.GetValue()*(expr_m.EvaluateDerivative(x, z))*
                    (expr_m.EvaluateDerivative(y))) /
                    std::pow((1 - std::pow(expr_m.GetValue(), 2.0)), (1.5))
                    - expr_m.EvaluateDerivative(x, y, z) /
                    std::sqrt(1 - std::pow(expr_m.GetValue(), 2.0));
        }

        inline const REAL_T EvaluateDerivative(uint32_t id, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return -1.0 * expr_m.EvaluateDerivative(id, i, j) / std::sqrt(1.0 - fx * fx);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return (((-1.0 * (fx * expr_m.EvaluateDerivative(a, i, j) * expr_m.EvaluateDerivative(b, i, j)))
                    / std::pow((1.0 - fx * fx), 1.5))
                    - (expr_m.EvaluateDerivative(a, b, i, j) / std::sqrt(1.0 - fx * fx)));
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return -((expr_m.EvaluateDerivative(x,i,j))*(expr_m.EvaluateDerivative(y,i,j))*
                    (expr_m.EvaluateDerivative(z,i,j))) /
                    std::pow((1 - std::pow(expr_m.GetValue(i,j), 2.0)), (1.5))-
                    (3 * std::pow(expr_m.GetValue(i,j), 2.0)*
                    (expr_m.EvaluateDerivative(x,i,j))*
                    (expr_m.EvaluateDerivative(y,i,j))*
                    (expr_m.EvaluateDerivative(z,i,j)))
                    / std::pow((1 - std::pow(expr_m.GetValue(i,j), 2.0)), (2.5))-
                    (expr_m.GetValue(i,j)*(expr_m.EvaluateDerivative(x, y,i,j))*
                    (expr_m.EvaluateDerivative(z,i,j))) /
                    std::pow((1 - std::pow(expr_m.GetValue(i,j), 2.0)), (1.5))
                    -(expr_m.GetValue(i,j)*
                    (expr_m.EvaluateDerivative(x,i,j))*(expr_m.EvaluateDerivative(y, z,i,j)))
                    / std::pow((1 - std::pow(expr_m.GetValue(i,j), 2.0)), (1.5))-
                    (expr_m.GetValue()*(expr_m.EvaluateDerivative(x, z,i,j))*
                    (expr_m.EvaluateDerivative(y,i,j))) /
                    std::pow((1 - std::pow(expr_m.GetValue(i,j), 2.0)), (1.5))
                    - expr_m.EvaluateDerivative(x, y, z,i,j) /
                    std::sqrt(1 - std::pow(expr_m.GetValue(i,j), 2.0));
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
    inline const ACos<REAL_T, EXPR> acos(const ExpressionBase<REAL_T, EXPR>& exp) {
        return ACos<REAL_T, EXPR>(exp.Cast());
    }

}//end namespace atl

#endif
