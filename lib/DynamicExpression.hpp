/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DynamicExpression.hpp
 * Author: matthewsupernaw
 *
 * Created on May 8, 2017, 1:43 PM
 */

#ifndef DYNAMICEXPRESSION_HPP
#define DYNAMICEXPRESSION_HPP

//#define USE_DELEGATES

#ifdef USE_DELEGATES

#ifndef AD_LOG10
#define AD_LOG10 2.30258509299404590109361379290930926799774169921875
#endif

#include <functional>
#include <cmath>
#include "VariableInfo.hpp"

namespace atl {

    template<typename REAL_T>
    struct DynamicExpressionBase {
        //function wrappers to handle both unary and binary operations. 
        //With unary the std::shared_ptr<DynamicExpressionBase<REAL_T> > second argument is ignored.
        typedef std::function<const REAL_T(std::shared_ptr<DynamicExpressionBase<REAL_T> >, std::shared_ptr<DynamicExpressionBase<REAL_T> >) > ValueFunction;
        typedef std::function<const REAL_T(uint32_t, std::shared_ptr<DynamicExpressionBase<REAL_T> >, std::shared_ptr<DynamicExpressionBase<REAL_T> >) > FirstOrderFunction;
        typedef std::function<const REAL_T(uint32_t, uint32_t, std::shared_ptr<DynamicExpressionBase<REAL_T> >, std::shared_ptr<DynamicExpressionBase<REAL_T> >) > SecondOrderFunction;
        typedef std::function< std::shared_ptr<DynamicExpressionBase<REAL_T> > (uint32_t, std::shared_ptr<DynamicExpressionBase<REAL_T> >, std::shared_ptr<DynamicExpressionBase<REAL_T> >) > DifferentiateFunction;


        ValueFunction value_f;
        FirstOrderFunction first_order_value_f;
        SecondOrderFunction second_order_value_f;
        DifferentiateFunction differentiate_f;
        std::shared_ptr<atl::VariableInfo<REAL_T> >info; //for variable types
        REAL_T value; //for reals
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp1;
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp2;

        DynamicExpressionBase() {
        }

        DynamicExpressionBase(ValueFunction value_f,
                FirstOrderFunction first_order_value_f,
                SecondOrderFunction second_order_value_f,
                DifferentiateFunction differentiate_f,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > exp1,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > exp2) :
        value_f(value_f),
        first_order_value_f(first_order_value_f),
        second_order_value_f(second_order_value_f),
        differentiate_f(differentiate_f),
        exp1(exp1),
        exp2(exp2) {
        }

        inline const REAL_T GetValue() {
            return value_f(exp1, exp2);
        }

        inline const REAL_T EvaluateDerivative(uint32_t x) {
            return first_order_value_f(x, exp1, exp2);
        }

        inline const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) {
            return second_order_value_f(x, y, exp1, exp2);
        }

        inline std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t x) {
            return differentiate_f(x, exp1, exp2);
        }

    };



    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > acos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > asin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > atan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > ceil(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cosh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > exp(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > fabs(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > floor(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log10(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > pow(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sinh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sqrt(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tanh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator+(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator/(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator*(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator-(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<typename REAL_T>
    struct ACosFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::ACosFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::ACosFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::ACosFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::ACosFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct ASinFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::ASinFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::ASinFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::ASinFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::ASinFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct ATanFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::ATanFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::ATanFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::ATanFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::ATanFunctions<double>::Differentiate),
                    exp, NULL));
            return ret;

        }

    };

    template<typename REAL_T>
    struct AddFunctions {

        static inline const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static inline const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->EvaluateDerivative(id) + rhs->EvaluateDerivative(id);
        }

        static inline const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->EvaluateDerivative(x, y) + rhs->EvaluateDerivative(x, y);
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->Differentiate(id) + rhs->Differentiate(id);
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::AddFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::AddFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::AddFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::AddFunctions<double>::Differentiate),
                    lhs, rhs));
            //            std::cout<<lhs->GetValue()<<"\n";
            return ret;

        }

    };

    template<typename REAL_T>
    struct CeilFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::CeilFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::CeilFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::CeilFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::CeilFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct CosFunctions {

        static inline const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return std::cos(lhs->GetValue());
        }

        static inline const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return -1.0 * lhs->EvaluateDerivative(id) * std::sin(lhs->GetValue());
        }

        static inline const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return ((-1.0 * std::cos(lhs->GetValue()) * lhs->EvaluateDerivative(x) * lhs->EvaluateDerivative(y))
                    - (1.0 * std::sin(lhs->GetValue()) * lhs->EvaluateDerivative(x, y)));
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::CosFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::CosFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::CosFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::CosFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct CoshFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::CoshFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::CoshFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::CoshFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::CoshFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct DivideFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::DivideFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::DivideFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::DivideFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::DivideFunctions<double>::Differentiate),
                    lhs, rhs));
            return ret;

        }

    };

    template<typename REAL_T>
    struct ExpFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::ExpFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::ExpFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::ExpFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::ExpFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct FabsFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::FabsFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::FabsFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::FabsFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::FabsFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct FloorFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::FloorFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::FloorFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::FloorFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::FloorFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct LogFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::LogFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::LogFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::LogFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::LogFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct Log10Functions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::Log10Functions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::Log10Functions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::Log10Functions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::Log10Functions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct MultiplyFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::MultiplyFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::MultiplyFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::MultiplyFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::MultiplyFunctions<double>::Differentiate),
                    lhs, rhs));
            return ret;

        }

    };

    template<typename REAL_T>
    struct PowFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::PowFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::PowFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::PowFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::PowFunctions<double>::Differentiate),
                    lhs, rhs));
            return ret;

        }

    };

    template<typename REAL_T>
    struct RealFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(const REAL_T& val) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::RealFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::RealFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::RealFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::RealFunctions<double>::Differentiate),
                    NULL, NULL));
            ret->value = val;
            return ret;

        }

    };

    template<typename REAL_T>
    struct SinFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::SinFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::SinFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::SinFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::SinFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct SinhFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::SinhFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::SinhFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::SinhFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::SinhFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct SqrtFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::SqrtFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::SqrtFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::SqrtFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::SqrtFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct SubtractFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::SubtractFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::SubtractFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::SubtractFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::SubtractFunctions<double>::Differentiate),
                    lhs, rhs));
            return ret;

        }

    };

    template<typename REAL_T>
    struct TanFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::TanFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::TanFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::TanFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::TanFunctions<double>::Differentiate),
                    exp, exp));
            return ret;

        }

    };

    template<typename REAL_T>
    struct TanhFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->GetValue() + rhs->GetValue();
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(
                std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::TanhFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::TanhFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::TanhFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::TanhFunctions<double>::Differentiate),
                    exp, exp));

            return ret;

        }

    };

    template<typename REAL_T>
    struct VariableFunctions {

        static const REAL_T GetValue(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return lhs->info->value;
        }

        static const REAL_T EvaluateDerivative1(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return static_cast<REAL_T> (lhs->info->id == id);
        }

        static const REAL_T EvaluateDerivative2(uint32_t x,
                uint32_t y, std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return 0.0;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs) {
            return NULL;
        }

        static std::shared_ptr<DynamicExpressionBase<REAL_T> > Create(std::shared_ptr<atl::VariableInfo<REAL_T> > info) {
            std::shared_ptr<atl::DynamicExpressionBase<double> > var(new atl::DynamicExpressionBase<double>());
            var->info = info;
            std::shared_ptr<atl::DynamicExpressionBase<double> > ret(new atl::DynamicExpressionBase<double>(
                    atl::DynamicExpressionBase<double>::ValueFunction(atl::VariableFunctions<double>::GetValue),
                    atl::DynamicExpressionBase<double>::FirstOrderFunction(atl::VariableFunctions<double>::EvaluateDerivative1),
                    atl::DynamicExpressionBase<double>::SecondOrderFunction(atl::VariableFunctions<double>::EvaluateDerivative2),
                    atl::DynamicExpressionBase<double>::DifferentiateFunction(atl::VariableFunctions<double>::Differentiate),
                    var, NULL));
            //            ret->info = info;
            //            std::cout<<__func__<<" "<<ret->info->id<<std::endl;
            return ret;

        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > acos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return ACosFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > asin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return ASinFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > atan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return ATanFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > ceil(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return CeilFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return CosFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cosh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return CoshFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > exp(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return ExpFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > fabs(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return FabsFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > floor(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return FloorFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return LogFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log10(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return Log10Functions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > pow(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return PowFunctions<REAL_T>::Create(lhs, rhs);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return SinFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sinh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return SinhFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sqrt(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return SqrtFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return TanFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tanh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return TanhFunctions<REAL_T>::Create(exp);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator+(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return AddFunctions<REAL_T>::Create(lhs, rhs);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator/(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return DivideFunctions<REAL_T>::Create(lhs, rhs);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator*(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return MultiplyFunctions<REAL_T>::Create(lhs, rhs);
    }

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator-(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return SubtractFunctions<REAL_T>::Create(lhs, rhs);
    }


}


#else


#include <cmath>
#include <sstream>
#include <unordered_map>
#include "VariableInfo.hpp"
#include "Platform.hpp"
#include "Utilities/MemoryPool.hpp"
#include "CLFAllocator.hpp"


#ifndef ATL_WINDOWS
//#include "clfmalloc.h"
#include "Tape.hpp"
#endif


#ifndef AD_LOG10
#define AD_LOG10 2.30258509299404590109361379290930926799774169921875
#endif

namespace atl {

    enum DynamicExpressionType {
        DYNAMIC_REAL_SCALAR = 0,
        DYNAMIC_EXPRESSION
    };

    template<typename REAL_T>
    struct DynamicExpressionBase {

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) = 0;
        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() = 0;
        virtual inline const REAL_T GetValue() const = 0;
        virtual const REAL_T EvaluateDerivative(uint32_t id) const = 0;
        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const = 0;
        virtual void PushOrder(int i) = 0;

        /**
         * For expressions that were differentiated, variables become "Differentiated 
         * Variables". the id is set and if the "Differentiated Varaible" has the 
         * same id, it returns one at a call to "GetValue()" otherwise zero. This
         * function must be called before the forward sweep on the expression
         * is performed.
         * 
         * @param id
         */
        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {

        }

        /**
         * When an expression is differentiated, dependent variables must be mapped to
         * a new set of dependent variables. 
         * @param vmap
         */
        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {

        }
        virtual const REAL_T Taylor(uint32_t degree) const = 0;
        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() = 0;
        virtual const std::string ToString() const = 0;


    };


    //Prototypes
    template<typename REAL_T>
    struct ACosDynamic;

    template<typename REAL_T>
    struct ASinDynamic;

    template<typename REAL_T>
    struct ATanDynamic;

    template<typename REAL_T>
    struct AddDynamic;

    template<typename REAL_T>
    struct CielDynamic;

    template<typename REAL_T>
    struct CosDynamic;

    template<typename REAL_T>
    struct CoshDynamic;

    template<typename REAL_T>
    struct DivideDynamic;

    template<typename REAL_T>
    struct ExpDynamic;

    template<typename REAL_T>
    struct FabsDynamic;

    template<typename REAL_T>
    struct FloorDynamic;

    template<typename REAL_T>
    struct LogDynamic;

    template<typename REAL_T>
    struct Log10Dynamic;

    template<typename REAL_T>
    struct MultiplyDynamic;

    template<typename REAL_T>
    struct PowDynamic;

    template<typename REAL_T>
    struct RealDynamic;

    template<typename REAL_T>
    struct SinDynamic;

    template<typename REAL_T>
    struct SinhDynamic;

    template<typename REAL_T>
    struct SqrtDynamic;

    template<typename REAL_T>
    struct SubtractDynamic;

    template<typename REAL_T>
    struct TanDynamic;

    template<typename REAL_T>
    struct TanhDynamic;

    template<typename REAL_T>
    struct VariableDynamic;

    template<typename REAL_T>
    struct DifferentiatedVariableDynamic;

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > acos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > asin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > atan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > ceil(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cosh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > exp(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > fabs(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > floor(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log10(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > pow(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sinh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sqrt(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tanh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator+(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator/(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator*(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator-(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs);

    template<typename REAL_T>
    struct ACosDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp;

        ACosDynamic(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) :
        exp(exp) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > neg_one = std::make_shared<atl::RealDynamic<REAL_T> >(-1.0);
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<atl::RealDynamic<REAL_T> >(1.0);
            return neg_one * exp->Differentiate(id) / atl::sqrt(one - exp->Clone() * exp->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > neg_one = std::make_shared<atl::RealDynamic<REAL_T> >(-1.0);
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<atl::RealDynamic<REAL_T> >(1.0);
            return neg_one * exp->Differentiate() / atl::sqrt(one - exp->Clone() * exp->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return std::acos(exp->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            REAL_T fx = exp->GetValue();
            return -1.0 * exp->EvaluateDerivative(id) / std::sqrt(1.0 - fx * fx);
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fx = exp->GetValue();
            return (((-1.0 * (fx * exp->EvaluateDerivative(x) * exp->EvaluateDerivative(y)))
                    / std::pow((1.0 - fx * fx), 1.5))
                    - (exp->EvaluateDerivative(x, y) / std::sqrt(1.0 - fx * fx)));
        }

        virtual void PushOrder(int i) {
            exp->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<ACosDynamic<REAL_T> >(exp->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "acos(" << exp->ToString() << ")";
            return ss.str();
        }

#ifndef ATL_WINDOWS

        void* operator new(size_t size){
            return malloc(size);
        }

        void free(void* ptr) {
            free(ptr);
        }

#endif

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > acos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<ACosDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct ASinDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp;

        ASinDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) :
        exp(exp) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            const std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            return exp->Differentiate(id) / atl::sqrt(one - exp->Clone() * exp->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            const std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            return exp->Differentiate() / atl::sqrt(one - exp->Clone() * exp->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return std::asin(this->exp->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            REAL_T fx = exp->GetValue();
            return exp->EvaluateDerivative(id) / std::sqrt(1.0 - fx * fx);
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fx = exp->GetValue();
            return ((((fx * exp->EvaluateDerivative(x) * exp->EvaluateDerivative(y)))
                    / std::pow((1.0 - fx * fx), 1.5))
                    +(exp->EvaluateDerivative(x, y) / std::sqrt(1.0 - fx * fx)));
        }

        virtual void PushOrder(int i) {
            exp->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<ASinDynamic<REAL_T> >(exp->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "asin(" << exp->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > asin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<ASinDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct ATanDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp;

        ATanDynamic(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) :
        exp(exp) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            return exp->Differentiate(id) / (exp->Clone() * exp->Clone() + one);
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            return exp->Differentiate() / (exp->Clone() * exp->Clone() + one);
        }

        virtual inline const REAL_T GetValue() const {
            return std::atan(this->exp->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            REAL_T fx = exp->GetValue();
            return exp->EvaluateDerivative(id) / (fx * fx + 1.0);
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fx = exp->GetValue();
            return (exp->EvaluateDerivative(x, y) / (fx * fx + 1.0)) -
                    (2.0 * fx * exp->EvaluateDerivative(x) *
                    exp->EvaluateDerivative(y)) / ((fx * fx + 1.0)*(fx * fx + 1.0));
        }

        virtual void PushOrder(int i) {
            exp->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<ATanDynamic<REAL_T> >(exp->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "atan(" << exp->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > atan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<ATanDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct AddDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m;
        std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m;
        //        static util::MemoryPool<AddDynamic<REAL_T> > pool;

        AddDynamic() {

        }

        AddDynamic(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs_m,
                const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs_m) :
        lhs_m(lhs_m), rhs_m(rhs_m) {
        }

        AddDynamic(AddDynamic<REAL_T>* other) :
        lhs_m(other->lhs_m), rhs_m(other->rhs_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return lhs_m->Differentiate(id) + rhs_m->Differentiate(id);
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return lhs_m->Differentiate() + rhs_m->Differentiate();
        }

        virtual inline const REAL_T GetValue() const {
            return lhs_m->GetValue() + rhs_m->GetValue();
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return lhs_m->EvaluateDerivative(id) + rhs_m->EvaluateDerivative(id);
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return lhs_m->EvaluateDerivative(x, y) + rhs_m->EvaluateDerivative(x, y);
        }

        virtual void PushOrder(int i) {
            lhs_m->PushOrder(i);
            rhs_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            lhs_m->DifferentiatedBy(id);
            rhs_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            lhs_m->SwapDependents(vmap);
            rhs_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return (lhs_m->Clone() + rhs_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "(" << lhs_m->ToString() << " + " << rhs_m->ToString() << ")";
            return ss.str();
        }

        //
        //#ifndef ATL_WINDOWS
        //
        //        static void* operator new(size_t size){
        //            return AddDynamic<REAL_T>::pool.malloc();
        //        }
        //
        //        static void free(AddDynamic<REAL_T>* ptr) {
        //            AddDynamic<REAL_T>::pool.free(ptr);
        //        }
        //
        //#endif

    };

    //    template<typename REAL_T>
    //    util::MemoryPool<AddDynamic<REAL_T> > AddDynamic<REAL_T>::pool(1000000);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator+(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return std::shared_ptr<AddDynamic<REAL_T> >(new AddDynamic<REAL_T>(lhs, rhs)); //
        //        return std::shared_ptr<AddDynamic<REAL_T> >(new AddDynamic<REAL_T>(lhs, rhs), AddDynamic<REAL_T>::free);
    }

    template<typename REAL_T>
    struct CielDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp;

        CielDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) :
        exp(exp) {
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return std::shared_ptr<DynamicExpressionBase<REAL_T> >(new RealDynamic<REAL_T>(0.0));
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return std::shared_ptr<DynamicExpressionBase<REAL_T> >(new RealDynamic<REAL_T>(0.0));
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual inline const REAL_T GetValue() const {
            return std::ceil(this->exp->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return 0.0;
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return 0.0;
        }

        virtual void PushOrder(int i) {
            exp->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<CielDynamic<REAL_T> >(exp->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "ceil(" << exp->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > ceil(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<CielDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct CosDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > expr_m;
        //        static util::MemoryPool<CosDynamic<REAL_T> > pool;

        CosDynamic() {
        }

        CosDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > expr_m) :
        expr_m(expr_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > neg_one = std::make_shared<RealDynamic<REAL_T> >(static_cast<REAL_T> (-1.0));
            return neg_one * expr_m->Differentiate(id) * atl::sin(expr_m);
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > neg_one = std::make_shared<RealDynamic<REAL_T> >(static_cast<REAL_T> (-1.0));
            return neg_one * expr_m->Differentiate() * atl::sin(expr_m);
        }

        virtual inline const REAL_T GetValue() const {
            return std::cos(expr_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return -1.0 * expr_m->EvaluateDerivative(id) * std::sin(expr_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return ((-1.0 * std::cos(expr_m->GetValue()) * expr_m->EvaluateDerivative(x) * expr_m->EvaluateDerivative(y))
                    - (1.0 * std::sin(expr_m->GetValue()) * expr_m->EvaluateDerivative(x, y)));
        }

        virtual void PushOrder(int i) {
            expr_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            expr_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            expr_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return atl::cos(expr_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "cos(" << expr_m->ToString() << ")";
            return ss.str();
        }


#ifndef ATL_WINDOWS

        //        static void* operator new(size_t size){
        //            return CosDynamic<REAL_T>::pool.malloc();
        //        }
        //
        //        static void free(void* ptr) {
        //            CosDynamic<REAL_T>::pool.free(ptr);
        //        }

#endif

    };

    //    template<typename REAL_T>
    //    util::MemoryPool<CosDynamic<REAL_T> > CosDynamic<REAL_T>::pool(1000000);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cos(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<CosDynamic<REAL_T> >(exp);
        //        return std::shared_ptr<CosDynamic<REAL_T> >(new CosDynamic<REAL_T>(exp), CosDynamic<REAL_T>::free);
    }

    template<typename REAL_T>
    struct CoshDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        CoshDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return exp_m->Differentiate(id) * atl::sinh(exp_m->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return exp_m->Differentiate() * atl::sinh(exp_m->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return std::cosh(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) * std::sinh(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return ((std::cosh(exp_m->GetValue()) * exp_m->EvaluateDerivative(x) * exp_m->EvaluateDerivative(y))
                    + (std::sinh(exp_m->GetValue()) * exp_m->EvaluateDerivative(x, y)));
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<CoshDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "cosh(" << exp_m->ToString() << ")";
            return ss.str();
        }
    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > cosh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<CoshDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct DivideDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m;
        std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m;

        DivideDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m) :
        lhs_m(lhs_m), rhs_m(rhs_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return (rhs_m->Clone() * lhs_m->Differentiate(id) -
                    rhs_m->Differentiate(id) * lhs_m->Clone()) / (rhs_m->Clone() * rhs_m->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return (rhs_m->Clone() * lhs_m->Differentiate() -
                    rhs_m->Differentiate() * lhs_m->Clone()) / (rhs_m->Clone() * rhs_m->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return lhs_m->GetValue() / rhs_m->GetValue();
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            //            if (rhs_m->Type() == atl::DYNAMIC_REAL_SCALAR) {
            //                return lhs_m->EvaluateDerivative(id) / rhs_m->GetValue();
            //            }

            return (rhs_m->GetValue() * lhs_m->EvaluateDerivative(id) -
                    rhs_m->EvaluateDerivative(id) * lhs_m->GetValue()) / (rhs_m->GetValue() * rhs_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T lhs_value_m = lhs_m->GetValue();
            REAL_T rhs_value_m = rhs_m->GetValue();
            REAL_T aa = ((2.0 * lhs_value_m * rhs_m->EvaluateDerivative(x) * rhs_m->EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m * rhs_value_m)); //(2*f(a,b)*('diff(g(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^3
            REAL_T bb = ((lhs_m->EvaluateDerivative(x) * rhs_m->EvaluateDerivative(y)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),a,1))*('diff(g(a,b),b,1)))/g(a,b)^2
            REAL_T cc = ((lhs_value_m * rhs_m->EvaluateDerivative(x, y)) / (rhs_value_m * rhs_value_m)); //(f(a,b)*('diff(g(a,b),a,1,b,1)))/g(a,b)^2
            REAL_T dd = ((lhs_m->EvaluateDerivative(y) * rhs_m->EvaluateDerivative(x)) / (rhs_value_m * rhs_value_m)); //(('diff(f(a,b),b,1))*('diff(g(a,b),a,1)))/g(a,b)^2
            REAL_T ee = lhs_m->EvaluateDerivative(x, y) / rhs_value_m;
            return aa - bb - cc - dd + ee;
        }

        virtual void PushOrder(int i) {
            lhs_m->PushOrder(i);
            rhs_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            lhs_m->DifferentiatedBy(id);
            rhs_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            lhs_m->SwapDependents(vmap);
            rhs_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<DivideDynamic<REAL_T> >(lhs_m->Clone(), rhs_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "(" << lhs_m->ToString() << " / " << rhs_m->ToString() << ")";
            return ss.str();
        }
    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator/(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return std::make_shared<DivideDynamic<REAL_T> >(lhs, rhs);
    }

    template<typename REAL_T>
    struct ExpDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        ExpDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return exp_m->Differentiate(id) * this->Clone();
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return exp_m->Differentiate() * this->Clone();
        }

        virtual inline const REAL_T GetValue() const {
            return std::exp(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) * this->GetValue();
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return this->GetValue() *(exp_m->EvaluateDerivative(x))*
                    (exp_m->EvaluateDerivative(y)) + this->GetValue() *
                    (exp_m->EvaluateDerivative(x, y));
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<ExpDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "exp(" << exp_m->ToString() << ")";
            return ss.str();
        }
    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > exp(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<ExpDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct FabsDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        FabsDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return (exp_m->Differentiate(id) * exp_m->Clone()) / this->Clone();
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return (exp_m->Differentiate() * exp_m->Clone()) / this->Clone();
        }

        virtual inline const REAL_T GetValue() const {
            return std::fabs(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) * this->GetValue();
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return (exp_m->EvaluateDerivative(x, y) * exp_m->GetValue()) / this->GetValue();
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<FabsDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "fabs(" << exp_m->ToString() << ")";
            return ss.str();
        }


    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > fabs(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<FabsDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct FloorDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp;

        FloorDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) :
        exp(exp) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return std::shared_ptr<DynamicExpressionBase<REAL_T> >(new RealDynamic<REAL_T>(0.0));
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return std::shared_ptr<DynamicExpressionBase<REAL_T> >(new RealDynamic<REAL_T>(0.0));
        }

        virtual inline const REAL_T GetValue() const {
            return std::floor(this->exp->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return 0.0;
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return 0.0;
        }

        virtual void PushOrder(int i) {
            exp->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<FloorDynamic<REAL_T> >(exp->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "floor(" << exp->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > floor(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<FloorDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct LogDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        LogDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return exp_m->Differentiate(id) / exp_m;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return exp_m->Differentiate() / exp_m;
        }

        virtual inline const REAL_T GetValue() const {
            return std::log(this->exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) / exp_m->GetValue();
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return (exp_m->EvaluateDerivative(x, y) / exp_m->GetValue()) -
                    (exp_m->EvaluateDerivative(x) * exp_m->EvaluateDerivative(y))
                    / (exp_m->GetValue() * exp_m->GetValue());
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<LogDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "log(" << exp_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<LogDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct Log10Dynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp;

        Log10Dynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp) :
        exp(exp) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return (exp->Differentiate(id) / (std::shared_ptr<DynamicExpressionBase<REAL_T> >(new RealDynamic<REAL_T>(AD_LOG10)) * exp->Clone()));
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return (exp->Differentiate() / (std::shared_ptr<DynamicExpressionBase<REAL_T> >(new RealDynamic<REAL_T>(AD_LOG10)) * exp->Clone()));
        }

        virtual inline const REAL_T GetValue() const {
            return std::log10(exp->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return (exp->EvaluateDerivative(id) / (AD_LOG10 * exp->GetValue()));
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fx = exp->GetValue();
            return (exp->EvaluateDerivative(x, y) / (AD_LOG10 * fx)) -
                    ((exp->EvaluateDerivative(x) * exp->EvaluateDerivative(y)) / (AD_LOG10 * (fx * fx)));
        }

        virtual void PushOrder(int i) {
            exp->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<Log10Dynamic<REAL_T> >(exp->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "log10(" << exp->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > log10(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<Log10Dynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct MultiplyDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m;
        std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m;
        //        static util::MemoryPool<MultiplyDynamic<REAL_T> > pool;

        MultiplyDynamic() {

        }

        MultiplyDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m) :
        lhs_m(lhs_m), rhs_m(rhs_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return (lhs_m->Clone() * rhs_m->Differentiate(id) +
                    lhs_m->Differentiate(id) * rhs_m->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return (lhs_m->Clone() * rhs_m->Differentiate() +
                    lhs_m->Differentiate() * rhs_m->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return lhs_m->GetValue() * rhs_m->GetValue();
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return (lhs_m->GetValue() * rhs_m->EvaluateDerivative(id) +
                    lhs_m->EvaluateDerivative(id) * rhs_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return lhs_m->EvaluateDerivative(x) * rhs_m->EvaluateDerivative(y) +
                    lhs_m->GetValue() * rhs_m->EvaluateDerivative(x, y) +
                    lhs_m->EvaluateDerivative(y) * rhs_m->EvaluateDerivative(x) +
                    rhs_m->GetValue() * lhs_m->EvaluateDerivative(x, y);
        }

        virtual void PushOrder(int i) {
            lhs_m->PushOrder(i);
            rhs_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            lhs_m->DifferentiatedBy(id);
            rhs_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            lhs_m->SwapDependents(vmap);
            rhs_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<MultiplyDynamic<REAL_T> >(lhs_m->Clone(), rhs_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "(" << lhs_m->ToString() << " * " << rhs_m->ToString() << ")";
            return ss.str();
        }


        //#ifndef ATL_WINDOWS
        //
        //        static void* operator new(size_t size){
        //            return MultiplyDynamic<REAL_T>::pool.malloc();
        //        }
        //
        //        static void free(void* ptr) {
        //            MultiplyDynamic<REAL_T>::pool.free(ptr);
        //        }
        //
        //#endif

    };
    //
    //    template<typename REAL_T>
    //    util::MemoryPool<MultiplyDynamic<REAL_T> > MultiplyDynamic<REAL_T>::pool(1000000);

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator*(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return std::make_shared<MultiplyDynamic<REAL_T> >(lhs, rhs);
        //        return std::shared_ptr<MultiplyDynamic<REAL_T> >(new MultiplyDynamic<REAL_T>(lhs, rhs), MultiplyDynamic<REAL_T>::free);
    }

    template<typename REAL_T>
    struct PowDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m;
        std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m;

        PowDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m) :
        lhs_m(lhs_m), rhs_m(rhs_m) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {



            if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_EXPRESSION) {

                return atl::pow(lhs_m->Clone(), rhs_m->Clone()) * (atl::log(lhs_m->Clone()) *
                        rhs_m->Differentiate(id) + rhs_m->Clone() * lhs_m->Differentiate(id) / lhs_m->Clone());

            } else if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_REAL_SCALAR) {
                std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
                return atl::pow(lhs_m->Clone(), rhs_m->Clone() - one)*(lhs_m->Clone()) * rhs_m->Clone();
            } else if (lhs_m->Type() == DYNAMIC_REAL_SCALAR && rhs_m->Type() == DYNAMIC_EXPRESSION) {
                return rhs_m->Differentiate(id) * atl::pow(lhs_m->Clone(), rhs_m->Clone()) * atl::log(lhs_m->Clone());
            }



            return NULL;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {

            if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_EXPRESSION) {

                return atl::pow(lhs_m->Clone(), rhs_m->Clone()) * (atl::log(lhs_m->Clone()) *
                        rhs_m->Differentiate() + rhs_m->Clone() * lhs_m->Differentiate() / lhs_m->Clone());

            } else if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_REAL_SCALAR) {
                std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
                return atl::pow(lhs_m->Clone(), rhs_m->Clone() - one)*(lhs_m->Clone()) * rhs_m->Clone();
            } else if (lhs_m->Type() == DYNAMIC_REAL_SCALAR && rhs_m->Type() == DYNAMIC_EXPRESSION) {
                return rhs_m->Differentiate() * atl::pow(lhs_m->Clone(), rhs_m->Clone()) * atl::log(lhs_m->Clone());
            }


            return NULL; //suppress warning
        }

        virtual inline const REAL_T GetValue() const {
            return std::pow(lhs_m->GetValue(), rhs_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            REAL_T g = rhs_m->GetValue();
            REAL_T f = lhs_m->GetValue();
            REAL_T fx = lhs_m->EvaluateDerivative(id);
            REAL_T gx = rhs_m->EvaluateDerivative(id);
            if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_EXPRESSION) {
                return std::pow(f, g) * (std::log(f) * gx + g * fx / f);
            } else if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_REAL_SCALAR) {
                return std::pow(f, g - 1)*(fx) * g;
            } else if (lhs_m->Type() == DYNAMIC_REAL_SCALAR && rhs_m->Type() == DYNAMIC_EXPRESSION) {
                return gx * std::pow(f, g) * std::log(f);
            }
            return 0.0;
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T fxy; // = lhs_m.EvaluateDerivative(x, y);
            REAL_T g; // = rhs_m.GetValue();
            REAL_T f; // = lhs_m.GetValue();
            REAL_T fx; // = lhs_m.EvaluateDerivative(x);
            REAL_T gy; // = rhs_m.EvaluateDerivative(y);
            REAL_T fy; // = lhs_m.EvaluateDerivative(y);
            REAL_T gx; // = rhs_m.EvaluateDerivative(x);
            REAL_T gxy; // = rhs_m.EvaluateDerivative(x, y);
            //            return std::pow(f, g)*(((fx * gy) / f) + std::log(f) * gxy + (fy * gx / f) -
            //                    (g * fx * fy) / (f * f) + g * fxy / f) + std::pow(f, g)*(std::log(f) * gx +
            //                    g * fx / f)*(std::log(f) * gy + g * fy / f);
            REAL_T ret = 0.0;

            if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_EXPRESSION) {
                fxy = lhs_m->EvaluateDerivative(x, y);
                g = rhs_m->GetValue();
                f = lhs_m->GetValue();
                fx = lhs_m->EvaluateDerivative(x);
                gy = rhs_m->EvaluateDerivative(y);
                fy = lhs_m->EvaluateDerivative(y);
                gx = rhs_m->EvaluateDerivative(x);
                gxy = rhs_m->EvaluateDerivative(x, y);

                return std::pow(f, g)*(((fx * gy) / f) + std::log(f) * gxy + (fy * gx / f) -
                        (g * fx * fy) / (f * f) + g * fxy / f) + std::pow(f, g)*(std::log(f) * gx +
                        g * fx / f)*(std::log(f) * gy + g * fy / f);


            } else if (lhs_m->Type() == DYNAMIC_EXPRESSION && rhs_m->Type() == DYNAMIC_REAL_SCALAR) {
                f = this->lhs_m->GetValue();
                ret = std::pow(f, rhs_m->GetValue() - static_cast<REAL_T> (2.0)) *
                        lhs_m->EvaluateDerivative(x) * lhs_m->EvaluateDerivative(y)*
                        (rhs_m->GetValue() - static_cast<REAL_T> (1.0)) * rhs_m->GetValue() +
                        std::pow(f, rhs_m->GetValue() - static_cast<REAL_T> (1.0)) *
                        lhs_m->EvaluateDerivative(x, y) * rhs_m->GetValue();
                return ret;

            } else if (lhs_m->Type() == DYNAMIC_REAL_SCALAR && rhs_m->Type() == DYNAMIC_EXPRESSION) {
                fxy = this->GetValue();
                return rhs_m->EvaluateDerivative(x) * rhs_m->EvaluateDerivative(y) * fxy *
                        (std::log(this->lhs_m->GetValue() * this->lhs_m->GetValue())) +
                        rhs_m->EvaluateDerivative(x, y) * fxy * std::log(this->lhs_m->GetValue());
            }
            return ret;
        }

        virtual void PushOrder(int i) {
            lhs_m->PushOrder(i);
            rhs_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            lhs_m->DifferentiatedBy(id);
            rhs_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            lhs_m->SwapDependents(vmap);
            rhs_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<PowDynamic<REAL_T> >(lhs_m->Clone(), rhs_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "pow(" << lhs_m->ToString() << ", " << rhs_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > pow(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return std::make_shared<PowDynamic<REAL_T> >(lhs, rhs);
    }

    template<typename REAL_T>
    struct RealDynamic : DynamicExpressionBase<REAL_T> {
        REAL_T value_m;

        RealDynamic(REAL_T val) : value_m(val) {

        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_REAL_SCALAR;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return std::make_shared<RealDynamic<REAL_T> >(0.0);
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return std::make_shared<RealDynamic<REAL_T> >(0.0);
        }

        virtual inline const REAL_T GetValue() const {
            return this->value_m;
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return 0.0;
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return 0.0;
        }

        virtual void PushOrder(int i) {
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<RealDynamic<REAL_T> >(value_m);
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "real(" << value_m << ")";
            return ss.str();
        }
    };

    template<typename REAL_T>
    struct SinDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        SinDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return exp_m->Differentiate(id) * atl::cos(exp_m->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return exp_m->Differentiate() * atl::cos(exp_m->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return std::sin(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) * std::cos(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return (std::cos(exp_m->GetValue()) * exp_m->EvaluateDerivative(x, y))-
                    std::sin(exp_m->GetValue()) * exp_m->EvaluateDerivative(x) * exp_m->EvaluateDerivative(y);
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<SinDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "sin(" << exp_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sin(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<SinDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct SinhDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        SinhDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return exp_m->Differentiate(id) * atl::cosh(exp_m->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return exp_m->Differentiate() * atl::cosh(exp_m->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return std::sinh(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) * std::cosh(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return ((std::sinh(exp_m->GetValue()) * exp_m->EvaluateDerivative(x) * exp_m->EvaluateDerivative(y))
                    + (std::cosh(exp_m->GetValue()) * exp_m->EvaluateDerivative(x, y)));
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<SinhDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "sinh(" << exp_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sinh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<SinhDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct SqrtDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        SqrtDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > two = std::make_shared<RealDynamic<REAL_T> >(static_cast<REAL_T> (2.0));
            return exp_m->Differentiate(id) / (two * this->Clone());
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > two = std::make_shared<RealDynamic<REAL_T> >(static_cast<REAL_T> (2.0));
            return exp_m->Differentiate() / (two * this->Clone());
        }

        virtual inline const REAL_T GetValue() const {
            return std::sqrt(this->exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) / (2.0 * this->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return (exp_m->EvaluateDerivative(x, y) / (2.0 * this->GetValue())) -
                    (exp_m->EvaluateDerivative(x) * exp_m->EvaluateDerivative(y)) / (4.0 * std::pow(exp_m->GetValue(), 1.5));
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<SqrtDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "sqrt(" << exp_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > sqrt(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<SqrtDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct SubtractDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m;
        std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m;

        SubtractDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > lhs_m,
                std::shared_ptr<DynamicExpressionBase<REAL_T> > rhs_m) :
        lhs_m(lhs_m), rhs_m(rhs_m) {
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return lhs_m->Differentiate(id) - rhs_m->Differentiate(id);
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return lhs_m->Differentiate() - rhs_m->Differentiate();
        }

        virtual inline const REAL_T GetValue() const {
            return lhs_m->GetValue() - rhs_m->GetValue();
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return lhs_m->EvaluateDerivative(id) - rhs_m->EvaluateDerivative(id);
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return lhs_m->EvaluateDerivative(x, y) - rhs_m->EvaluateDerivative(x, y);
        }

        virtual void PushOrder(int i) {
            lhs_m->PushOrder(i);
            rhs_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            lhs_m->DifferentiatedBy(id);
            rhs_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            lhs_m->SwapDependents(vmap);
            rhs_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<SubtractDynamic<REAL_T> >(lhs_m->Clone(), rhs_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "(" << lhs_m->ToString() << " - " << rhs_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > operator-(
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& lhs,
            const std::shared_ptr<DynamicExpressionBase<REAL_T> >& rhs) {
        return std::make_shared<SubtractDynamic<REAL_T> >(lhs, rhs);
    }

    template<typename REAL_T>
    struct TanDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        TanDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            return exp_m->Differentiate(id) * (one / atl::cos(exp_m->Clone()))*(one / atl::cos(exp_m->Clone()));
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            return exp_m->Differentiate() * (one / atl::cos(exp_m->Clone()))*(one / atl::cos(exp_m->Clone()));
        }

        virtual inline const REAL_T GetValue() const {
            return std::tan(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            return exp_m->EvaluateDerivative(id) * (1.0 / std::cos(exp_m->GetValue()))*(1.0 / std::cos(exp_m->GetValue()));
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T sec2 = (1.0 / std::cos(exp_m->GetValue())) * (1.0 / std::cos(exp_m->GetValue()));
            return 2.0 * sec2 * this->GetValue() * exp_m->EvaluateDerivative(x) * exp_m->EvaluateDerivative(y) +
                    sec2 * exp_m->EvaluateDerivative(x, y);
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<TanDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "tan(" << exp_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tan(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<TanDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct TanhDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m;

        TanhDynamic(std::shared_ptr<DynamicExpressionBase<REAL_T> > exp_m) :
        exp_m(exp_m) {
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            std::shared_ptr<DynamicExpressionBase<REAL_T> > sech = one / atl::cosh(exp_m->Clone());
            return exp_m->Differentiate(id) * sech*sech;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            std::shared_ptr<DynamicExpressionBase<REAL_T> > one = std::make_shared<RealDynamic<REAL_T> >(1.0);
            std::shared_ptr<DynamicExpressionBase<REAL_T> > sech = one / atl::cosh(exp_m->Clone());
            return exp_m->Differentiate() * sech*sech;
        }

        virtual inline const REAL_T GetValue() const {
            return std::tanh(exp_m->GetValue());
        }

        virtual const REAL_T EvaluateDerivative(uint32_t id) const {
            REAL_T sech = 1.0 / std::cosh(exp_m->GetValue());
            return exp_m->EvaluateDerivative(id) * sech*sech;
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            REAL_T sech = 1.0 / std::cosh(exp_m->GetValue());
            REAL_T sech2 = sech*sech;
            return sech2 * exp_m->EvaluateDerivative(x, y) - 2.0 * sech2 * this->GetValue() * exp_m->EvaluateDerivative(x) * exp_m->EvaluateDerivative(y);
        }

        virtual void PushOrder(int i) {
            exp_m->PushOrder(i);
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            exp_m->DifferentiatedBy(id);
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            exp_m->SwapDependents(vmap);
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::make_shared<TanhDynamic<REAL_T> >(exp_m->Clone());
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "tanh(" << exp_m->ToString() << ")";
            return ss.str();
        }

    };

    template<class REAL_T>
    inline const std::shared_ptr<DynamicExpressionBase<REAL_T> > tanh(const std::shared_ptr<DynamicExpressionBase<REAL_T> >& exp) {
        return std::make_shared<TanhDynamic<REAL_T> >(exp);
    }

    template<typename REAL_T>
    struct VariableDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<atl::VariableInfo<REAL_T> > info;
        //        static util::MemoryPool<VariableDynamic<REAL_T> > pool;

        VariableDynamic() {
        }

        VariableDynamic(std::shared_ptr<atl::VariableInfo<REAL_T> > info) :
        info(info) {

        }

        VariableDynamic(const VariableDynamic<REAL_T>& other) :
        info(other.info) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return std::make_shared<RealDynamic<REAL_T> >(static_cast<REAL_T> (this->info->id == id));
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return std::shared_ptr<DynamicExpressionBase<REAL_T> >(new DifferentiatedVariableDynamic<REAL_T>(info, 1));
        }

        virtual inline const REAL_T GetValue() const {
            return info->value;
        }

        const REAL_T EvaluateDerivative(uint32_t id) const {
            return info->id == id ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
            //            if (this->info.get() == NULL) {
            //                std::cout << "Null variable info\n" << std::flush;
            //                return 0;
            //            }
            //            return this->info->id == id ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return 0.0;
        }

        virtual void PushOrder(int i) {
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >::const_iterator it;
            it = vmap.find(this->info->id);
            if (it != vmap.end()) {
                this->info = (*it).second;
            }
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::shared_ptr<VariableDynamic<REAL_T> >(new VariableDynamic<REAL_T>(this->info)); //, VariableDynamic<REAL_T>::free);
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "variable[" << info->id << "," << info->value << "]";
            return ss.str();
        }

        //        static void* operator new(size_t size){
        //            return VariableDynamic<REAL_T>::pool.malloc();
        //        }
        //
        //        static void free(void* ptr) {
        //            VariableDynamic<REAL_T>::pool.free(ptr);
        //        }
        //

    };

    template<typename REAL_T>
    struct DifferentiatedVariableDynamic : DynamicExpressionBase<REAL_T> {
        std::shared_ptr<atl::VariableInfo<REAL_T> > info;
        uint32_t diff_id = 0;
        int order = 0;

        DifferentiatedVariableDynamic() {
        }

        DifferentiatedVariableDynamic(std::shared_ptr<atl::VariableInfo<REAL_T> > info, int order) :
        info(info), order(order) {

        }

        DifferentiatedVariableDynamic(const VariableDynamic<REAL_T>& other) :
        info(other.info) {
        }

        virtual DynamicExpressionType Type() {
            return DYNAMIC_EXPRESSION;
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate(uint32_t id) {
            return std::make_shared<RealDynamic<REAL_T> >(static_cast<REAL_T> (0.0));
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > Differentiate() {
            return std::make_shared<RealDynamic<REAL_T> >(static_cast<REAL_T> (0.0));
        }

        virtual inline const REAL_T GetValue() const {
            return this->info->id == diff_id ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
        }

        const REAL_T EvaluateDerivative(uint32_t id) const {
            return 0.0;
        }

        virtual const REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return 0.0;
        }

        virtual void PushOrder(int i) {
            if (order == 0) {
                this->order = i;
            }
        }

        virtual void DifferentiatedBy(const std::vector<uint32_t>& id) {
            assert(order<=id.size());
            this->diff_id = id[order-1];
        }

        virtual void SwapDependents(const typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >& vmap) {
            typename std::unordered_map<uint32_t, std::shared_ptr<atl::VariableInfo<REAL_T> > >::const_iterator it;
            it = vmap.find(this->info->id);
            if (it != vmap.end()) {
                this->info = (*it).second;
            }
        }

        virtual const REAL_T Taylor(uint32_t degree) const {
            return 0.0;
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > Clone() {
            return std::shared_ptr<DifferentiatedVariableDynamic<REAL_T> >(new DifferentiatedVariableDynamic<REAL_T>(this->info));
        }

        const std::string ToString() const {
            std::stringstream ss;
            ss << "diffv(";
            this->info->id == diff_id ? ss << static_cast<REAL_T> (1.0) : ss << static_cast<REAL_T> (0.0);
            ss << ")"; //," << info->value << "]";
            return ss.str();
        }
    };
    
 

}
#endif

#endif /* DYNAMICEXPRESSION_HPP */

