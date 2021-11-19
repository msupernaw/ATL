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


//#include "lib/MinMax.hpp"

#include "lib/ThreadPool.hpp"
#include "lib/ACos.hpp"
//#include "lib/ad_cmath.hpp"
#include "lib/Add.hpp"
#include "lib/ASin.hpp"
#include "lib/ATan.hpp"
#include "lib/Ceil.hpp"
#include "lib/CLFAllocator.hpp"
#include "lib/Cos.hpp"
#include "lib/Cosh.hpp"
#include "lib/Divide.hpp"
#include "lib/Exp.hpp"
#include "lib/Expression.hpp"
#include "lib/Fabs.hpp"
#include "lib/Floor.hpp"
//#include "lib/FunctionMinimizer.hpp"
#include "lib/Helpers.hpp"
#include "lib/Log.hpp"
#include "lib/Log10.hpp"
//#include "lib/Matrix.hpp"
#include "lib/MinMax.hpp"
#include "lib/Multiply.hpp"
#include "lib/Platform.hpp"
#include "lib/Pow.hpp"
#include "lib/Real.hpp"
#include "lib/Serialize.hpp"
#ifdef ATL_USE_SSE
#include "lib/SIMD.hpp"
#endif
#include "lib/Sin.hpp"
#include "lib/Sinh.hpp"
#include "lib/Sqrt.hpp"
#include "lib/Subtract.hpp"
#include "lib/Tan.hpp"
#include "lib/Tanh.hpp"
#include "lib/Tape.hpp"
//#include "lib/ThreadPool.hpp"
#include "lib/Variable.hpp"
//#include "lib/Vector.hpp"
#include "lib/Optimization.hpp"
//#include "lib/Utilities/IO/StreamedDataFile.hpp"


namespace atl {


    
    
    
    





    /***********************************
     * Miscellaneous Functions Below   *
     ***********************************/









    /**
     * Introduction
     */

    /**
     * Automatic Differentiation
     */

    /**
     * Optimization
     */

    /**
     * Concurrency
     */
    
  /**
     * function used to protect overflow in exp calculations.
     *
     * Author: Dave Fournier.
     * Original implementation in ADMB.
     *
     * Source: http://admb-project.org/documentation/api/mfexp_8cpp_source.html
     *
     * @param expr
     */
    template<class REAL_T, class EXPR>
    inline const atl::Variable<REAL_T> mfexp(const atl::ExpressionBase<REAL_T, EXPR>& expr) {

        REAL_T b = REAL_T(60.0);
        if (expr.GetValue() <= b && expr.GetValue() >= REAL_T(-1) * b) {
            return atl::exp(expr);
        } else if (expr.GetValue() > b) {
            return /*std::exp(b)*/EXP_OF_B * (REAL_T(1.) + REAL_T(2.) * (expr - b)) / (REAL_T(1.) + expr - b);
        } else {
            return std::exp(REAL_T(-1) * b)*(REAL_T(1.) - expr - b) / (REAL_T(1.) + REAL_T(2.) * (REAL_T(-1) * expr - b));
        }
    }


    

}


#endif /* ATL_HPP */

