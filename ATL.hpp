/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ATL.hpp
 * Author: matthewsupernaw
 *
 * Created on November 3, 2016, 2:01 PM
 */

#ifndef ATL_HPP
#define ATL_HPP
#include "ThreadPool.hpp"
#include "Variable.hpp"
#include "Fabs.hpp"
namespace atl {


    //global thread pool
    atl::ThreadPool thread_pool_g;

    
    
    
    
    /***********************************
     * Miscellaneous Functions Below   *
     ***********************************/
    
    /**
     * Returns the maximum between a and b in a continuous manner using:
     * 
     * (a + b + |a - b|) / 2.0;
     * 
     * @param a
     * @param b
     * @return 
     */
    template <typename T>
    inline const atl::Variable<T> max(const atl::Variable<T>& a, const atl::Variable<T>& b) {
        return (a + b + atl::fabs(a - b)) / 2.0;
    }

    /**
     * Returns the minimum between a and b in a continuous manner using:
     * 
     * (a + b - |a - b|) / 2.0;
     * 
     * @param a
     * @param b
     * @return 
     */
    template <typename T>
    inline const atl::Variable<T> min(const atl::Variable<T>& a, const atl::Variable<T>& b) {
        return (a + b - atl::fabs(a - b)) / 2.0;
    }


}


#endif /* ATL_HPP */

