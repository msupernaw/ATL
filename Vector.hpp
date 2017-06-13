/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vector.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 10:07 AM
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Matrix.hpp"


namespace atl {

    /**
     * VariableVector is a 1 x M VariableMatrix.
     */
    template<typename T>
    struct VariableVector : VariableMatrix<T> {
//        using VariableMatrix<T>::operator=;

        /**
         * Constructor. 
         * @param columns
         */
        VariableVector(size_t columns = 0) :
        VariableMatrix<T>(1, columns) {
        }

        /**
         * Sets the size of this vector.
         * 
         * @param size
         */
        void SetSize(size_t size) {
            this->columns = size;
            this->data_m.resize(size);
        }

         atl::Variable<T>& operator()(size_t j) {
            return this->data_m[j];
        }
        
        /**
         * Returns the size of this vector.
         * @return 
         */
        size_t GetSize() {
            return this->columns;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::VariableVector<T>";
            return ss.str();
        }


    };
    
    
    /**
     * RealVector is a 1 x M RealMatrix.
     */
    template<typename T>
    struct RealVector : RealMatrix<T> {
        using RealMatrix<T>::operator=;

        /**
         * Constructor. 
         * @param columns
         */
        RealVector(size_t columns = 0) :
        RealMatrix<T>(1, columns) {
        }

        /**
         * Sets the size of this vector.
         * 
         * @param size
         */
        void SetSize(size_t size) {
            this->columns = size;
            this->data_m.resize(size);
        }

        /**
         * Returns the size of this vector.
         * @return 
         */
        size_t GetSize() {
            return this->columns;
        }

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::RealVector<T>";
            return ss.str();
        }


    };
    
}


#endif /* VECTOR_HPP */

