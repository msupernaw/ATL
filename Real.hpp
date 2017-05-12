/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Real.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 5:21 PM
 */

#ifndef REAL_HPP
#define REAL_HPP

#include "Expression.hpp"


namespace atl {

    /**
     * Wrapper class for real numbers. Used in expression templates to avoid 
     * separate templates for operations involving real values in binary 
     * expressions.
     */
    template<typename REAL_T>
    struct Real : atl::ExpressionBase<REAL_T, Real<REAL_T> > {
        typedef REAL_T base_type;
        REAL_T value;

        /**
         * Constructor.
         * 
         * @param v
         */
        Real(REAL_T v = static_cast<REAL_T> (0.0)) {
            value = v;
        }

        /**
         * Copy constructor.
         * 
         * @param other
         */
        Real(const Real<REAL_T>& other) :
        value(other.value) {

        }

        ~Real() {

        }

        /**
         * Assignment operator.
         * 
         * @param v
         * @return 
         */
        Real& operator=(const REAL_T& v) {
            value = v;
            return *this;
        }

        operator REAL_T() const {
            return value;
        }

        /**
         * Returns the value.
         * 
         * @return 
         */
        inline const REAL_T GetValue() const {
            return value;
        }

        /**
         * Returns the value.
         * 
         * @return 
         */
        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return value;
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
         * Nothing done.
         * 
         * @param ids
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
        }

        /**
         * Nothing done.
         * 
         * @param ids
         * @param i
         * @param j
         */
        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
        }

        inline void PushNLIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, bool nl = false)const {

        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return std::complex<REAL_T>(this->value);
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            return degree == 0 ? this->value : static_cast<REAL_T> (0.0);
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
#ifdef USE_DELEGATES
            return atl::RealFunctions<REAL_T>::Create(this->value);
#else
            return std::make_shared<atl::RealDynamic<REAL_T> >(this->value);
#endif
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @param y
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @param y
         * @param z
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @param y
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns 0.
         * 
         * @param x
         * @param y
         * @param z
         * @param i
         * @param j
         * @return 
         */
        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        /**
         * Returns std::numeric_limits<size_t>::max();
         * @return 
         */
        size_t GetColumns() const {
            return std::numeric_limits<size_t>::max();
        }

        /**
         * std::numeric_limits<size_t>::max();
         * 
         * @return 
         */
        size_t GetRows() const {
            return std::numeric_limits<size_t>::max();
        }

        /**
         * Returns true.
         * 
         * @return 
         */
        bool IsScalar() const {
            return true;
        }

        /**
         * Creates a string representation of this expression template.
         * 
         * @return 
         */
        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::Real<T>";
            return ss.str();
        }


    };



}

#endif /* REAL_HPP */

