#ifndef ACOS_HPP
#define ACOS_HPP

#include "Expression.hpp"

namespace atl {

   

    /**
     * Expression template to handle arccosine for variable or 
     * container expressions. 
     * 
     * \f$ \arccos f(x,y) \f$
     * 
     * or 
     * 
     * \f$ \arccos f_{i,j}(x,y) \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct ACos : public ExpressionBase<REAL_T, ACos<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        ACos(const ACos<REAL_T, EXPR>& other) :
        expr_m(other.expr_m), b_(other.b_), val_(other.val_) {
        }

        /**
         * Constructor
         * 
         * @param a
         */
        ACos(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Compute the arccosine of the expression.
         *  
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::acos(expr_m.GetValue());
        }

        /**
         * 
         * Compute the arccosine of the expression evaluated at index {i,j}.
         * 
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::acos(expr_m.GetValue(i, j));
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
            return std::acos(expr_m.ComplexEvaluate(x, h));
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            if (degree == 0) {
                b_.reserve(5);
                val_.reserve(5);
                b_.resize(1);
                val_.resize(1);

                b_[0] = std::sqrt(static_cast<REAL_T> (1.0) - this->expr_m.Taylor(0) * this->expr_m.Taylor(0));
                val_[0] = std::acos(this->expr_m.Taylor(0));

                return val_[0];
            }

            size_t l = val_.size();
            b_.resize(degree + 1);
            val_.resize(degree + 1);
            REAL_T temp = static_cast<REAL_T> (0.0);
            for (int i = l; i <= degree; i++) {
                temp = static_cast<REAL_T> (0.0);

                for (int k = 0; k <= i; k++) {
                    temp -= this->expr_m.Taylor(k) * this->expr_m.Taylor(i - k);
                }
                val_[i] = static_cast<REAL_T> (0.0);
                b_[i] = static_cast<REAL_T> (0.0);

                for (unsigned int j = 1; j < i; ++j) {
                    b_[i] -= static_cast<REAL_T> (j) * b_[j] * b_[i - j];
                    val_[i] -= static_cast<REAL_T> (j) * val_[j] * b_[i - j];
                }
                b_[i] /= static_cast<REAL_T> (i);
                val_[i] /= static_cast<REAL_T> (i);

                b_[i] += temp / static_cast<REAL_T> (2.0);
                val_[i] -= this->expr_m.Taylor(i);

                b_[i] /= b_[0];
                val_[i] /= b_[0];
            }
            return val_[degree];
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return std::make_shared<atl::ACosDynamic<REAL_T> >(expr_m.ToDynamic());
        }

        /**
         * Evaluates the first-order derivative of this expression with respect 
         * to x.
         * 
         * \f$
         * -{{{{d}\over{d\,x}}\,f\left(x \right)}\over{\sqrt{1-f^2
         * \left(x \right)}}} \f$
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {
            REAL_T fx = expr_m.GetValue();
            return -1.0 * expr_m.EvaluateDerivative(x) / std::sqrt(1.0 - fx * fx);
        }

        /**
         * Evaluates the second-order mixed partial with respect to x and y.
         * 
         * \f$
         * -{{f\left(x , y\right)\,\left({{d}\over{d\,x}}\,f\left(x , y\right)
         * \right)\,\left({{d}\over{d\,y}}\,f\left(x , y\right)\right)}\over{
         * \left(1-f^2\left(x , y\right)\right)^{{{3}\over{2}}}}}-{{{{d^2
         * }\over{d\,x\,d\,y}}\,f\left(x , y\right)}\over{\sqrt{1-f^2\left(x , 
         * y\right)}}} \f$
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fx = expr_m.GetValue();
            return (((-1.0 * (fx * expr_m.EvaluateDerivative(x) * expr_m.EvaluateDerivative(y)))
                    / std::pow((1.0 - fx * fx), 1.5))
                    - (expr_m.EvaluateDerivative(x, y) / std::sqrt(1.0 - fx * fx)));
        }

        /**
         * Evaluates the third-order mixed partial with respect to x,y, and z.
         * 
         * \f$
         * -{{{{d}\over{d\,x}}\,f\left(x , y , z\right)\,\left({{d}\over{d\,y
         * }}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d\,z}}\,f\left(x
         * , y , z\right)\right)}\over{\left(1-f^2\left(x , y , z\right)
         * \right)^{{{3}\over{2}}}}}-{{3\,f^2\left(x , y , z\right)\,\left({{d
         * }\over{d\,x}}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d\,y
         * }}\,f\left(x , y , z\right)\right)\,\left({{d}\over{d\,z}}\,f\left(x
         * , y , z\right)\right)}\over{\left(1-f^2\left(x , y , z\right)
         * \right)^{{{5}\over{2}}}}}-{{f\left(x , y , z\right)\,\left({{d^2
         * }\over{d\,x\,d\,y}}\,f\left(x , y , z\right)\right)\,\left({{d
         * }\over{d\,z}}\,f\left(x , y , z\right)\right)}\over{\left(1-f^2
         * \left(x , y , z\right)\right)^{{{3}\over{2}}}}}- \\ {{f\left(x , y , z
         * \right)\,\left({{d}\over{d\,x}}\,f\left(x , y , z\right)\right)\,
         * \left({{d^2}\over{d\,y\,d\,z}}\,f\left(x , y , z\right)\right)
         * }\over{\left(1-f^2\left(x , y , z\right)\right)^{{{3}\over{2}}}}}-{{
         * f\left(x , y , z\right)\,\left({{d^2}\over{d\,x\,d\,z}}\,f\left(x , 
         * y , z\right)\right)\,\left({{d}\over{d\,y}}\,f\left(x , y , z\right)
         * \right)}\over{\left(1-f^2\left(x , y , z\right)\right)^{{{3}\over{2
         * }}}}}-{{{{d^3}\over{d\,x\,d\,y\,d\,z}}\,f\left(x , y , z\right)
         * }\over{\sqrt{1-f^2\left(x , y , z\right)}}} 
         * \f$
         * @param x
         * @param y
         * @param z
         * @return 
         */
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

        /**
         * Evaluates the first-order derivative of this expression with respect 
         * to x at index {i,j}.
         * 
         * \f$
         * -{{{{d}\over{d\,x}}\,f\left(x \right)}\over{\sqrt{1-f^2
         * \left(x \right)}}} \f$
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return -1.0 * expr_m.EvaluateDerivative(x, i, j) / std::sqrt(1.0 - fx * fx);
        }

        /**
         * 
         * Evaluates the second-order mixed partial with respect to x and y at
         * index {i,j}.
         * 
         * \f$ 
         * -{{f_{i,j}(x,y)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y)\right)\,
         * \left({{d}\over{d\,y}}\,f_{i,j}(x,y)\right)}\over{\left(1-f_{i,j}(x,
         * y)^2\right)^{{{3}\over{2}}}}}-{{{{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,
         * y)}\over{\sqrt{1-f_{i,j}(x,y)^2}}} 
         * \f$
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            REAL_T fx = expr_m.GetValue(i, j);
            return (((-1.0 * (fx * expr_m.EvaluateDerivative(x, i, j) * expr_m.EvaluateDerivative(y, i, j)))
                    / std::pow((1.0 - fx * fx), 1.5))
                    - (expr_m.EvaluateDerivative(x, y, i, j) / std::sqrt(1.0 - fx * fx)));
        }

        /**
         * Evaluates the third-order mixed partial with respect to x,y, and z
         * at index {i,j}.
         * 
         * \f$ -{{{{d}\over{d\,x}}\,f_{i,j}(x,y,z)\,\left({{d}\over{d\,y}}\,f_{i,j
         * }(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)
         * }\over{\left(1-f_{i,j}(x,y,z)^2\right)^{{{3}\over{2}}}}}-{{3\,f_{i,j
         * }(x,y,z)^2\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         * }\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j
         * }(x,y,z)\right)}\over{\left(1-f_{i,j}(x,y,z)^2\right)^{{{5}\over{2}}
         * }}}-{{f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y,z)
         * \right)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)}\over{\left(1
         * -f_{i,j}(x,y,z)^2\right)^{{{3}\over{2}}}}}- \\ {{f_{i,j}(x,y,z)\,\left(
         * {{d}\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d^2}\over{d\,y\,d\,
         * z}}\,f_{i,j}(x,y,z)\right)}\over{\left(1-f_{i,j}(x,y,z)^2\right)^{{{
         * 3}\over{2}}}}}-{{f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,f_{
         * i,j}(x,y,z)\right)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)
         * }\over{\left(1-f_{i,j}(x,y,z)^2\right)^{{{3}\over{2}}}}}-{{{{d^3
         * }\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)}\over{\sqrt{1-f_{i,j}(x,y,
         * z)^2}}} \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return -((expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))*
                    (expr_m.EvaluateDerivative(z, i, j))) /
                    std::pow((1 - std::pow(expr_m.GetValue(i, j), 2.0)), (1.5))-
                    (3 * std::pow(expr_m.GetValue(i, j), 2.0)*
                    (expr_m.EvaluateDerivative(x, i, j))*
                    (expr_m.EvaluateDerivative(y, i, j))*
                    (expr_m.EvaluateDerivative(z, i, j)))
                    / std::pow((1 - std::pow(expr_m.GetValue(i, j), 2.0)), (2.5))-
                    (expr_m.GetValue(i, j)*(expr_m.EvaluateDerivative(x, y, i, j))*
                    (expr_m.EvaluateDerivative(z, i, j))) /
                    std::pow((1 - std::pow(expr_m.GetValue(i, j), 2.0)), (1.5))
                    -(expr_m.GetValue(i, j)*
                    (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, z, i, j)))
                    / std::pow((1 - std::pow(expr_m.GetValue(i, j), 2.0)), (1.5))-
                    (expr_m.GetValue()*(expr_m.EvaluateDerivative(x, z, i, j))*
                    (expr_m.EvaluateDerivative(y, i, j))) /
                    std::pow((1 - std::pow(expr_m.GetValue(i, j), 2.0)), (1.5))
                    - expr_m.EvaluateDerivative(x, y, z, i, j) /
                    std::sqrt(1 - std::pow(expr_m.GetValue(i, j), 2.0));
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
            ss << "atl::ACos<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }

        const EXPR& expr_m;
        mutable std::vector<REAL_T> b_;
        mutable std::vector<REAL_T> val_;
    };

    /**
     * Returns a expression template representing arccosine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const ACos<REAL_T, EXPR> acos(const ExpressionBase<REAL_T, EXPR>& exp) {
        return ACos<REAL_T, EXPR>(exp.Cast());
    }



}//end namespace atl

#endif
