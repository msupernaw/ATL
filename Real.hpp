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

        Real(REAL_T v = static_cast<REAL_T> (0.0)) {
            value = v;
        }
        
        Real(const Real<REAL_T>& other) :
        value(other.value) {
            
        }

        ~Real(){
            
        }

        Real& operator=(const REAL_T& v) {
            value = v;
            return *this;
        }

        operator REAL_T() const { return value; }
        
        inline const REAL_T GetValue() const {
            return value;
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return value;
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
        }

        inline REAL_T EvaluateDerivative(uint32_t a) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        size_t GetColumns() const {
            return std::numeric_limits<size_t>::max();
        }

        size_t GetRows() const {
            return std::numeric_limits<size_t>::max();
        }

        bool IsScalar() const {
            return true;
        }

    };


}

#endif /* REAL_HPP */

