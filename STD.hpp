/*
 * File:   STD.hpp
 *
 * Author: Matthew Supernaw
 * National Oceanic and Atmospheric Administration
 * National Marine Fisheries Service
 * Sustainable Fisheries Division
 * Email: matthew.supernaw@noaa.gov
 *
 * Created on October 5, 2021, 9:36 AM
 *
 * This File is part of the NOAA, National Marine Fisheries Service
 * Analytics Template Library project.
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained by
 * using this  software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express or
 * implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.
 *
 * Please cite the author(s) in any work or product based on this material.
 *
 */
#ifndef STD_HPP
#define STD_HPP

#include "ATL.hpp"

namespace std {

    /**
     * Returns a expression template representing arccosine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::ACos<REAL_T, EXPR> acos(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::ACos<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns an expression template representing arcsine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::ASin<REAL_T, EXPR> asin(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::ASin<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns an expression template representing arctangent.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::ATan<REAL_T, EXPR> atan(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::ATan<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns an expression template representing cosine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Cos<REAL_T, EXPR> cos(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Cos<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns an expression template representing hyperbolic cosine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Cosh<REAL_T, EXPR> cosh(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Cosh<REAL_T, EXPR>(exp.Cast());
    }

    template<class REAL_T, class EXPR>
    inline const atl::Exp<REAL_T, EXPR> exp(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Exp<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns an expression template representing log. 
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Log<REAL_T, EXPR> log(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Log<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns an expression template representing log10.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Log10<REAL_T, EXPR> log10(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Log10<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns a expression template representing pow.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS, class RHS>
    inline const atl::Pow<REAL_T, LHS, RHS> pow(const atl::ExpressionBase<REAL_T, LHS>& a,
            const atl::ExpressionBase<REAL_T, RHS>& b) {
        return atl::Pow<REAL_T, LHS, RHS > (a.Cast(), b.Cast());
    }

    /**
     * Returns a expression template representing pow.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class LHS>
    inline const atl::Pow<REAL_T, LHS, atl::Real<REAL_T> > pow(const atl::ExpressionBase<REAL_T, LHS>& a,
            REAL_T b) {

        return atl::Pow<REAL_T, LHS, atl::Real<REAL_T> > (a.Cast(), b);
    }

    /**
     * Returns a expression template representing pow.
     * 
     * @param a
     * @param b
     * @return 
     */
    template <class REAL_T, class RHS>
    inline const atl::Pow<REAL_T, atl::Real<REAL_T>, RHS> pow(const REAL_T& a,
            const atl::ExpressionBase<REAL_T, RHS>& b) {
        return atl::Pow<REAL_T, atl::Real<REAL_T>, RHS > (a, b.Cast());
    }

    /**
     * Creates a expression template representing sine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Sin<REAL_T, EXPR> sin(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Sin<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Creates a expression template representing hyperbolic sine.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Sinh<REAL_T, EXPR> sinh(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Sinh<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Returns an expression template representing the square root.
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Sqrt<REAL_T, EXPR> sqrt(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Sqrt<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Creates a expression template representing tangent. 
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Tan<REAL_T, EXPR> tan(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Tan<REAL_T, EXPR>(exp.Cast());
    }

    /**
     * Creates a expression template representing hyperbolic tangent. 
     * 
     * @param exp
     * @return 
     */
    template<class REAL_T, class EXPR>
    inline const atl::Tanh<REAL_T, EXPR> tanh(const atl::ExpressionBase<REAL_T, EXPR>& exp) {
        return atl::Tanh<REAL_T, EXPR>(exp.Cast());
    }



}




#endif /* STD_HPP */

