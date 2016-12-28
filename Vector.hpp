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

    template<typename T>
    struct VariableVector : VariableMatrix<T> {

        using VariableMatrix<T>::operator =;
        
        VariableVector(size_t rows = 0) :
        VariableMatrix<T>(1,rows) {
        }
        

    };
}


#endif /* VECTOR_HPP */

