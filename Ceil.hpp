#ifndef CEIL_HPP
#define CEIL_HPP

#include "Expression.hpp"
#include <exception>

namespace atl {

    /**
     * Evaluate the ceiling of an expression. This template is not 
     * differentiable. 
     */
    template<class REAL_T, class EXPR>
    struct Ceil : public ExpressionBase<REAL_T, Ceil<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Ceil(const Ceil<REAL_T, EXPR>& other) :
        expr_m(other.expr_m) {
        }

        /**
         * Constructor
         * 
         * @param a
         */
        Ceil(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Computes the ceiling of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::ceil(expr_m.GetValue());
        }

        /**
         * Computes the ceiling of the evaluated expression at index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::ceil(expr_m.GetValue(i, j));
        }

        /**
         * Returns false.
         * 
         * @return 
         */
        inline bool IsNonlinear() const {
            return false;
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
           throw std::exception("ceil not available for complex numbers");
//            return std::ceil(expr_m.ComplexEvaluate(x, h));
        }

        /**
         *  Push variable info into a set. 
         * 
         * @param ids
         * @param i
         * @param j
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

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return atl::ceil(expr_m.ToDynamic());
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateFirstDerivative(uint32_t x) const {
            return 0.0;
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateSecondDerivative(uint32_t x, uint32_t y) const {
            return 0.0;
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateThirdDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return 0.0;
        }

        /**
         * Returns 0. Not differentiable.
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateFirstDerivativeAt(uint32_t x, size_t i, size_t j = 0) const {
            return 0.0;
        }

        /**
         * Returns 0. Not differentiable. 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateSecondDerivativeAt(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return 0.0;
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
        inline REAL_T EvaluateThirdDerivativeAt(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return 0.0;
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
            ss << "atl::Ceil<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }

        const EXPR& expr_m;
    };

    /**
     * Ceiling of the expression exp. Not Differentiable.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const Ceil<REAL_T, EXPR> ceil(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Ceil<REAL_T, EXPR>(exp.Cast());
    }


}//end namespace atl

#endif

