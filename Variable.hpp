/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Variable.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 10:06 AM
 */

#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "VariableInfo.hpp"
#include "Expression.hpp"

namespace atl {


    template<typename REAL_T>
    struct Variable;

    template<>
    ExpressionType ExpressionTrait<Variable<float> >::et_type = atl::VARIABLE_SCALAR;

    template<>
    ExpressionType ExpressionTrait<Variable<double> >::et_type = atl::VARIABLE_SCALAR;

    template<>
    ExpressionType ExpressionTrait<Variable<long double> >::et_type = atl::VARIABLE_SCALAR;

    template<typename REAL_T>
    struct Variable : atl::ExpressionBase<REAL_T, Variable<REAL_T> > {
        static Tape<REAL_T> tape;
        typedef REAL_T base_type;

        atl::VariableInfo<REAL_T>* info = new atl::VariableInfo<REAL_T>();

        Variable(REAL_T v = static_cast<REAL_T> (0.0)) {
            info->value = v;
        }

        Variable(const Variable<REAL_T>& other) :
        info(other.info) {
        }

        Variable& operator=(const REAL_T& v) {
            this->info->value = v;
            return *this;
        }


        //        Variable& operator = (const Variable<REAL_T>& other){
        //            this->info->value = other.info->value;
        //            return *this;
        //        }

        template<class A>
        Variable(const ExpressionBase<REAL_T, A>& exp) {


            size_t index = atl::Variable<REAL_T>::tape.NextIndex();



            atl::StackEntry<REAL_T>& entry = atl::Variable<REAL_T>::tape.stack[index++];
            exp.PushIds(entry.ids);
            entry.w = this->info;
            entry.first.resize(entry.ids.size());
            typename atl::StackEntry<REAL_T>::vi_iterator it;
            size_t k = 0;
            for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                entry.first[k] = exp.EvaluateDerivative((*it)->id);
                k++;
            }


            this->info->value = exp.GetValue();
        }

        template<class A>
        inline Variable& operator=(const ExpressionBase<REAL_T, A>& exp) {


            size_t index = atl::Variable<REAL_T>::tape.NextIndex();



            atl::StackEntry<REAL_T>& entry = atl::Variable<REAL_T>::tape.stack[index++];
            exp.PushIds(entry.ids);
            entry.w = this->info;
            entry.first.resize(entry.ids.size());
            typename atl::StackEntry<REAL_T>::vi_iterator it;
            size_t k = 0;
            for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                entry.first[k] = exp.EvaluateDerivative((*it)->id);
                k++;
            }


            this->info->value = exp.GetValue();
            return *this;
        }

        inline Variable& operator+=(const REAL_T& val) {
            *this = *this+val;
            return *this;
        }

        template<class A>
        inline Variable& operator+=(const ExpressionBase<REAL_T, A>& exp) {
            *this = *this+exp;
            return *this;
        }

        inline Variable& operator-=(const REAL_T& val) {
            *this = *this-val;
            return *this;
        }

        template<class A>
        inline Variable& operator-=(const ExpressionBase<REAL_T, A>& exp) {
            *this = *this-exp;
            return *this;
        }

        inline Variable& operator*=(const REAL_T& val) {
            *this = *this*val;
            return *this;
        }

        template<class A>
        inline Variable& operator*=(const ExpressionBase<REAL_T, A>& exp) {
            *this = *this*exp;
            return *this;
        }

        inline Variable& operator/=(const REAL_T& val) {
            *this = *this / val;
            return *this;
        }

        template<class A>
        inline Variable& operator/=(const ExpressionBase<REAL_T, A>& exp) {
            *this = *this / exp;
            return *this;
        }

        inline Variable& operator++() {
            *this = *this+static_cast<REAL_T> (1.0);
            return *this;
        }

        inline const Variable operator++(int i) {
            Variable temp = *this;
            *this = static_cast<REAL_T> (1.0)+ (*this);
            return temp;
        }

        inline Variable& operator--() {
            *this = *this-static_cast<REAL_T> (1.0);
            return *this;
        }

        inline const Variable operator--(int i) {
            Variable temp = *this;
            *this = (*this) - static_cast<REAL_T> (1.0);
            return temp;
        }

        inline const REAL_T GetValue() const {
            return info->value;
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return info->value;
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            ids.insert(info);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            ids.insert(info);
        }

        inline REAL_T EvaluateDerivative(uint32_t a) const {
            return info->id == a ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, uint32_t b) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t a, size_t i, size_t j = 0) const {
            return info->id == a ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
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

    template<typename REAL_T>
    Tape<REAL_T> Variable<REAL_T>::tape;

    template<typename REAL_T>
    std::ostream& operator<<(std::ostream& out, const Variable<REAL_T>& v) {
        out << v.GetValue();
        return out;
    }


}



#endif /* VARIABLE_HPP */

