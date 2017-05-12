#ifndef ATAN_HPP
#define ATAN_HPP

#include "Expression.hpp"

namespace atl {

    
    /**
     * Expression template to handle arctangent for variable or 
     * container expressions. 
     * 
     * \f$ \arctan f(x,y) \f$
     * 
     * or 
     * 
     * \f$ \arctan f_{i,j}(x,y) \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct ATan : public ExpressionBase<REAL_T, ATan<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        ATan(const ATan<REAL_T, EXPR>& other) :
        expr_m(other.expr_m), b_(other.b_), val_(other.val_) {
        }

        /**
         * Constructor
         * 
         * @param a
         */
        ATan(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Compute the arctangent of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::atan(expr_m.GetValue());
        }

        /**
         * Compute the arctangent of the evaluated expression at
         * index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::atan(expr_m.GetValue(i, j));
        }

        /**
         * Returns true.
         * 
         * @return 
         */
        inline bool IsNonlinear() const {
            return true;
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
            expr_m.PushNLIds(ids, true);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return std::atan(expr_m.ComplexEvaluate(x, h));
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            if (degree == 0) {
                b_.reserve(5);
                val_.reserve(5);
                b_.resize(1);
                val_.resize(1);

                b_[0] = (static_cast<REAL_T> (1.0) + this->expr_m.Taylor(0) * this->expr_m.Taylor(0));
                val_[0] = std::atan(this->expr_m.Taylor(0));

                return val_[0];
            }

            size_t l = val_.size();
            b_.resize(degree + 1);
            val_.resize(degree + 1);

            for (int i = l; i <= degree; i++) {
                val_[i] = static_cast<REAL_T> (0.0);
                b_[i] = static_cast<REAL_T> (2.0) * expr_m.Taylor(0) * expr_m.Taylor(i);

                for (unsigned int j = 1; j < i; ++j) {
                    b_[i] += expr_m.Taylor(j) * expr_m.Taylor(i - j);
                    val_[i] -= static_cast<REAL_T> (j) * val_[j] * b_[i - j];
                }

                val_[i] /= static_cast<REAL_T> (i);
                val_[i] += expr_m.Taylor(i);
                val_[i] /= b_[0];
            }
            return val_[degree];
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return atl::atan(expr_m.ToDynamic());
        }

        /**
         * Evaluates the first-order derivative of this expression with respect 
         * to x.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f(x)}\over{f(x)^2+1}} \f$
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {
            REAL_T fx = expr_m.GetValue();
            return expr_m.EvaluateDerivative(x) / (fx * fx + 1.0);
        }

        /**
         * Evaluates the second-order mixed partial with respect to x and y.
         * 
         * \f$ {{{{d^2}\over{d\,x\,d\,y}}\,f(x,y)}\over{f(x,y)^2+1}}-
         * {{2\,f(x,y)\,\left({{d}\over{d\,x}}\,f(x,y)\right)\,
         * \left({{d}\over{d\,y}}\,f(x,y)\right)}\over{\left(f(x,y)
         * ^2+1\right)^2}} \f$
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fx = expr_m.GetValue();
            return (expr_m.EvaluateDerivative(x, y) / (fx * fx + 1.0)) -
                    (2.0 * fx * expr_m.EvaluateDerivative(x) *
                    expr_m.EvaluateDerivative(y)) / ((fx * fx + 1.0)*(fx * fx + 1.0));
        }

        /**
         *  Evaluates the third-order mixed partial with respect to x,y, and z.
         * 
         * \f$ -{{2\,\left({{d}\over{d\,x}}\,f\left(x , y , z\right)\right)\,
         *  \left({{d}\over{d\,y}}\,f\left(x , y , z\right)\right)\,\left({{d
         *  }\over{d\,z}}\,f\left(x , y , z\right)\right)}\over{\left(f^2\left(x
         *   , y , z\right)+1\right)^2}}+{{8\,f^2\left(x , y , z\right)\,\left(
         *  {{d}\over{d\,x}}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d
         *  \,y}}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d\,z}}\,f
         *  \left(x , y , z\right)\right)}\over{\left(f^2\left(x , y , z\right)+
         *  1\right)^3}}-{{2\,f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d
         *  \,y}}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d\,z}}\,f
         *  \left(x , y , z\right)\right)}\over{\left(f^2\left(x , y , z\right)+
         *  1\right)^2}}- \\ {{2\,f\left(x , y , z\right)\,\left({{d}\over{d\,x}}\,f
         *  \left(x , y , z\right)\right)\,\left({{d^2}\over{d\,y\,d\,z}}\,f
         *  \left(x , y , z\right)\right)}\over{\left(f^2\left(x , y , z\right)+
         *  1\right)^2}}-{{2\,f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d
         *  \,z}}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d\,y}}\,f
         *  \left(x , y , z\right)\right)}\over{\left(f^2\left(x , y , z\right)+
         *  1\right)^2}}+{{{{d^3}\over{d\,x\,d\,y\,d\,z}}\,f\left(x , y , z
         *  \right)}\over{f^2\left(x , y , z\right)+1}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
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

        /**
         * Evaluates the first-order derivative of this expression with respect 
         * to x at index {i,j}.
         * 
         * \f$ {{{{d}\over{d\,x}}\,f_{i,j}(x)}\over{f_{i,j}(x)^2+1}} \f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return expr_m.EvaluateDerivative(x, i, j) / (fx * fx + 1.0);
        }

        /**
         * Evaluates the second-order mixed partial with respect to x and y at
         * index {i,j}.
         * 
         * \f$ {{{{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y)}\over{f_{i,j}(x,y)^2+1}}-
         * {{2\,f_{i,j}(x,y)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y)\right)\,
         * \left({{d}\over{d\,y}}\,f_{i,j}(x,y)\right)}\over{\left(f_{i,j}(x,y)
         * ^2+1\right)^2}} \f$
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return (expr_m.EvaluateDerivative(x, y, i, j) / (fx * fx + 1.0)) -
                    (2.0 * fx * expr_m.EvaluateDerivative(x, i, j) *
                    expr_m.EvaluateDerivative(y, i, j)) / ((fx * fx + 1.0)*(fx * fx + 1.0));
        }

        /**
         * Evaluates the third-order mixed partial with respect to x,y, and z at
         * index {i,j}.
         * 
         * \f$ -{{2\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         * }\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j
         * }(x,y,z)\right)}\over{\left(f_{i,j}(x,y,z)^2+1\right)^2}}+{{8\,f_{i,
         * j}(x,y,z)^2\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{
         * d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,
         * j}(x,y,z)\right)}\over{\left(f_{i,j}(x,y,z)^2+1\right)^3}}-{{2\,f_{i
         * ,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y,z)\right)\,
         * \left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)}\over{\left(f_{i,j}(x,
         * y,z)^2+1\right)^2}}- \\ {{2\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,f_{
         * i,j}(x,y,z)\right)\,\left({{d^2}\over{d\,y\,d\,z}}\,f_{i,j}(x,y,z)
         * \right)}\over{\left(f_{i,j}(x,y,z)^2+1\right)^2}}-{{2\,f_{i,j}(x,y,z
         * )\,\left({{d^2}\over{d\,x\,d\,z}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         * }\over{d\,y}}\,f_{i,j}(x,y,z)\right)}\over{\left(f_{i,j}(x,y,z)^2+1
         * \right)^2}}+{{{{d^3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)}\over{f
         * _{i,j}(x,y,z)^2+1}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
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

        /**
         * Return the number of columns.
         *
         * @return
         */
        size_t GetColumns() const {
            return expr_m.GetColumns();
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
            ss << "atl::ATan<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }
        const EXPR& expr_m;
        mutable std::vector<REAL_T> b_;
        mutable std::vector<REAL_T> val_;
    };

    /**
     * Returns an expression template representing arctangent.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const ATan<REAL_T, EXPR> atan(const ExpressionBase<REAL_T, EXPR>& exp) {
        return ATan<REAL_T, EXPR>(exp.Cast());
    }


}//end namespace atl

#endif
