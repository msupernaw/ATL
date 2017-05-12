/* 
 * File:   Tanh.hpp
 * Author: matthewsupernaw
 *
 * Created on June 18, 2014, 12:51 PM
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


#ifndef TANH_HPP
#define TANH_HPP

#include "Expression.hpp"

namespace atl {

   

    /**
     * Expression template to handle hyperbolic tangent for variable or 
     * container expressions. 
     * 
     * \f$  \tanh f(x) \f$
     * 
     * or 
     * 
     * \f$ \tanh f_{i,j}(x)  \f$
     * 
     */
    template<class REAL_T, class EXPR>
    struct Tanh : public ExpressionBase<REAL_T, Tanh<REAL_T, EXPR> > {
        typedef REAL_T BASE_TYPE;

        Tanh(const Tanh<REAL_T, EXPR>& other) :
        expr_m(other.expr_m) {
        }

        /**
         * Constructor.
         * 
         * @param a
         */
        Tanh(const ExpressionBase<REAL_T, EXPR>& a)
        : expr_m(a.Cast()) {
        }

        /**
         * Computes the hyperbolic tangent of the evaluated expression.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return std::tanh(expr_m.GetValue());
        }

        /**
         * Computes the hyperbolic tangent of the evaluated expression
         * at index {i,j}.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return std::tanh(expr_m.GetValue(i, j));
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
            expr_m.PushNLIds(ids, true);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return std::tanh(expr_m.ComplexEvaluate(x, h));
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            if (degree == 0) {
                val_.reserve(5);
                val_.resize(1);
                b_.reserve(5);
                b_.resize(1);
                val_[0] = std::tanh(this->expr_m.Taylor(0));
                b_[0] = val_[0] * val_[0];
                return val_[0];
            }

            size_t l = val_.size();
            val_.resize(degree + 1);
            b_.resize(degree + 1);

            for (int i = l; i <= degree; i++) {
                REAL_T tmp = static_cast<REAL_T> (i);
                val_[i] = expr_m.Taylor(i);

                for (int k = 1; k <= i; k++) {
                    val_[i] -= static_cast<REAL_T> (k) * expr_m.Taylor(k) * b_[i - k] / tmp;
                }

                b_[i] = val_[0] * val_[i];
                for (int k = 1; k <= i; k++) {
                    b_[i] += val_[k] * val_[i - k];
                }
            }
            return val_[degree];
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return atl::tanh(expr_m.ToDynamic());
        }

        /**
         * Evaluates the first-order derivative with respect to x.
         * 
         * \f$ {\rm sech}\; ^2f(x)\,\left({{d}\over{d\,x}}\,f(x)\right) \f$
         * 
         * @param x
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x) const {
            REAL_T sech = 1.0 / std::cosh(expr_m.GetValue());
            return expr_m.EvaluateDerivative(x) * sech*sech;
        }

        /**
         * Evaluates the second-order derivative with respect to x and y.
         * 
         * \f$ {\rm sech}\; ^2f(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,f
         * (x,y)\right)-2\,{\rm sech}\; ^2f(x,y)\,\tanh f(x,y)\,
         *  \left({{d}\over{d\,x}}\,f(x,y)\right)\,\left({{d}\over{d\,y}}
         *  \,f(x,y)\right) \f$
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T sech = 1.0 / std::cosh(expr_m.GetValue());
            REAL_T sech2 = sech*sech;
            return sech2 * expr_m.EvaluateDerivative(x, y) - 2.0 * sech2 * this->GetValue() * expr_m.EvaluateDerivative(x) * expr_m.EvaluateDerivative(y);

        }

        /**
         * Evaluates the third-order derivative with respect to x, y, z.
         * 
         * \f$ 4\,{\rm sech}\; ^2f(x,y,z)\,\tanh ^2f(x,y,z)\,\left({{d
         *  }\over{d\,x}}\,f(x,y,z)\right)\,\left({{d}\over{d\,y}}\,f
         *  (x,y,z)\right)\,\left({{d}\over{d\,z}}\,f(x,y,z)\right)- \\ 2\,
         *  {\rm sech}\; ^4f(x,y,z)\,\left({{d}\over{d\,x}}\,f(x,y,z
         *  )\right)\,\left({{d}\over{d\,y}}\,f(x,y,z)\right)\,\left({{d
         *  }\over{d\,z}}\,f(x,y,z)\right)- \\ 2\,{\rm sech}\; ^2f(x,y,z
         *  )\,\tanh f(x,y,z)\,\left({{d^2}\over{d\,x\,d\,y}}\,f(x,y
         *  ,z)\right)\,\left({{d}\over{d\,z}}\,f(x,y,z)\right)- \\ 2\,
         *  {\rm sech}\; ^2f(x,y,z)\,\tanh f(x,y,z)\,\left({{d
         *  }\over{d\,x}}\,f(x,y,z)\right)\,\left({{d^2}\over{d\,y\,d\,z}}
         *  \,f(x,y,z)\right)- \\ 2\,{\rm sech}\; ^2f(x,y,z)\,\tanh f(x,y,z)\,
         * \left({{d^2}\over{d\,x\,d\,z}}\,f(x,y,z)\right)\,
         * \left({{d}\over{d\,y}}\,f(x,y,z)\right)+ \\ {\rm sech}\; ^2f
         * (x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f(x,y,z)\right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            REAL_T sech = 1.0 / std::cosh(expr_m.GetValue());
            return 4.0 * std::pow(sech, 2.0) * std::pow(std::tanh(expr_m.GetValue()), 2.0)
                    * (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))
                    *(expr_m.EvaluateDerivative(z)) - 2.0 * std::pow(sech, 4.0)
                    * (expr_m.EvaluateDerivative(x))*(expr_m.EvaluateDerivative(y))
                    *(expr_m.EvaluateDerivative(z)) - 2.0 * std::pow(sech, 2.0)
                    * std::tanh(expr_m.GetValue())*(expr_m.EvaluateDerivative(x, y))*
                    (expr_m.EvaluateDerivative(z)) - 2.0 * std::pow(sech, 2.0)
                    * std::tanh(expr_m.GetValue())*(expr_m.EvaluateDerivative(x))
                    *(expr_m.EvaluateDerivative(y, z)) - 2.0 * std::pow(sech, 2.0)
                    * std::tanh(expr_m.GetValue())*(expr_m.EvaluateDerivative(x, z))
                    *(expr_m.EvaluateDerivative(y)) + std::pow(sech, 2.0)
                    * (expr_m.EvaluateDerivative(x, y, z));
        }

        /**
         * Evaluates the first-order derivative with respect to x at index 
         * {i,j}.
         * 
         * \f$ {\rm sech}\; ^2f_{i,j}(x)\,\left({{d}\over{d\,x}}\,f_{i,j}(x)
         * \right) \f$
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline const REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            REAL_T sech = 1.0 / std::cosh(expr_m.GetValue(i, j));
            return expr_m.EvaluateDerivative(x, i, j) * sech*sech;
        }

        /**
         * Evaluates the second-order derivative with respect to x and y at 
         * index {i,j}.
         * 
         * \f$ {\rm sech}\; ^2f_{i,j}(x,y)\,\left({{d^2}\over{d\,x\,d\,y}}\,f_{i,j
         *  }(x,y)\right)-2\,{\rm sech}\; ^2f_{i,j}(x,y)\,\tanh f_{i,j}(x,y)\,
         *  \left({{d}\over{d\,x}}\,f_{i,j}(x,y)\right)\,\left({{d}\over{d\,y}}
         *  \,f_{i,j}(x,y)\right) \f$
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            REAL_T sech = 1.0 / std::cosh(expr_m.GetValue(i, j));
            REAL_T sech2 = sech*sech;
            return sech2 * expr_m.EvaluateDerivative(x, y, i, j) - 2.0 * sech2 * this->GetValue(i, j) * expr_m.EvaluateDerivative(x, i, j) * expr_m.EvaluateDerivative(y, i, j);

        }

        /**
         * Evaluates the third-order derivative with respect to x, y, z at index 
         * {i,j}.
         * 
         * \f$ 4\,{\rm sech}\; ^2f_{i,j}(x,y,z)\,\tanh ^2f_{i,j}(x,y,z)\,\left({{d
         *  }\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d}\over{d\,y}}\,f_{i,j
         *  }(x,y,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)- \\ 2\,
         *  {\rm sech}\; ^4f_{i,j}(x,y,z)\,\left({{d}\over{d\,x}}\,f_{i,j}(x,y,z
         *  )\right)\,\left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)\,\left({{d
         *  }\over{d\,z}}\,f_{i,j}(x,y,z)\right)- \\ 2\,{\rm sech}\; ^2f_{i,j}(x,y,z
         *  )\,\tanh f_{i,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,y}}\,f_{i,j}(x,y
         *  ,z)\right)\,\left({{d}\over{d\,z}}\,f_{i,j}(x,y,z)\right)- \\ 2\,
         *  {\rm sech}\; ^2f_{i,j}(x,y,z)\,\tanh f_{i,j}(x,y,z)\,\left({{d
         *  }\over{d\,x}}\,f_{i,j}(x,y,z)\right)\,\left({{d^2}\over{d\,y\,d\,z}}
         *  \,f_{i,j}(x,y,z)\right)- \\ 2\,{\rm sech}\; ^2f_{i,j}(x,y,z)\,\tanh f_{i
         * ,j}(x,y,z)\,\left({{d^2}\over{d\,x\,d\,z}}\,f_{i,j}(x,y,z)\right)\,
         * \left({{d}\over{d\,y}}\,f_{i,j}(x,y,z)\right)+ \\ {\rm sech}\; ^2f_{i,j}
         * (x,y,z)\,\left({{d^3}\over{d\,x\,d\,y\,d\,z}}\,f_{i,j}(x,y,z)\right) \f$
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            REAL_T sech = 1.0 / std::cosh(expr_m.GetValue(i, j));
            return 4.0 * std::pow(sech, 2.0) * std::pow(std::tanh(expr_m.GetValue()), 2.0)
                    * (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))
                    *(expr_m.EvaluateDerivative(z, i, j)) - 2.0 * std::pow(sech, 4.0)
                    * (expr_m.EvaluateDerivative(x, i, j))*(expr_m.EvaluateDerivative(y, i, j))
                    *(expr_m.EvaluateDerivative(z, i, j)) - 2.0 * std::pow(sech, 2.0)
                    * std::tanh(expr_m.GetValue(i, j))*(expr_m.EvaluateDerivative(x, y, i, j))*
                    (expr_m.EvaluateDerivative(z, i, j)) - 2.0 * std::pow(sech, 2.0)
                    * std::tanh(expr_m.GetValue(i, j))*(expr_m.EvaluateDerivative(x, i, j))
                    *(expr_m.EvaluateDerivative(y, z, i, j)) - 2.0 * std::pow(sech, 2.0)
                    * std::tanh(expr_m.GetValue(i, j))*(expr_m.EvaluateDerivative(x, z, i, j))
                    *(expr_m.EvaluateDerivative(y, i, j)) + std::pow(sech, 2.0)
                    * (expr_m.EvaluateDerivative(x, y, z, i, j));
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
            ss << "atl::Tanh<T," << expr_m.ToExpressionTemplateString() << " >";
            return ss.str();
        }


        const EXPR& expr_m;
        mutable std::vector<REAL_T> val_;
        mutable std::vector<REAL_T> b_;
    };

    /**
     * Creates a expression template representing hyperbolic tangent. 
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const Tanh<REAL_T, EXPR> tanh(const ExpressionBase<REAL_T, EXPR>& exp) {
        return Tanh<REAL_T, EXPR>(exp.Cast());
    }


}//end namespace atl

#endif
