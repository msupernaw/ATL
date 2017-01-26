/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Traits.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 10:11 AM
 */

#ifndef TRAITS_HPP
#define TRAITS_HPP

#include "Variable.hpp"
#include "Matrix.hpp"

namespace atl {

    template<typename T>
    struct TypeTrait {
        typedef T base_type;
//        const std::string str = "T";

    };

    template<>
    struct TypeTrait<atl::Variable<long double> > {
        typedef long double base_type;
//        std::string str = "long double";
    };

    template<>
    struct TypeTrait<atl::Variable<double> > {
        typedef double base_type;
//        std::string str = "double";
    };

    template<>
    struct TypeTrait<atl::Variable<float> > {
        typedef float base_type;
//        std::string str = "float";
    };

    template<typename T>
    struct ContainerTrait {
        static bool is_container;
    };

      
    
    
    template<typename T>
    bool ContainerTrait<T>::is_container = false;
    
    template<>
    bool ContainerTrait<VariableMatrix<long double> >::is_container = true;
    
    template<>
    bool ContainerTrait<VariableMatrix< double> >::is_container = true;
    
    template<>
    bool ContainerTrait<VariableMatrix<float> >::is_container = true;



    
   
    
    
}


#endif /* TRAITS_HPP */

