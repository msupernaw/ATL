#ifndef CEIL_HPP
#define CEIL_HPP

#include "Expression.hpp"

namespace atl {

    template<class REAL_T, class EXPR>
    struct Ceil : public ExpressionBase<REAL_T, Ceil<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Ceil(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        inline const REAL_T GetValue() const {
            return std::ceil(expr_m.GetValue());
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::ceil(expr_m.GetValue(i, j));
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            expr_m.PushIds(ids);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            expr_m.PushIds(ids, i, j);
        }

        inline const REAL_T EvaluateDerivative(uint32_t id) const {
            return 0.0;
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return 0.0;
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return 0.0;
        }

        inline const REAL_T EvaluateDerivative(uint32_t id, size_t i, size_t j = 0) const {
            return 0.0;
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            return 0.0;
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return 0.0;
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
    inline const Ceil<REAL_T, EXPR> ceil(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Ceil<REAL_T, EXPR>(exp.Cast());
    }

}//end namespace atl

#endif
