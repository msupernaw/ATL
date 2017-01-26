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
//#include "Traits.hpp"

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

        Variable(const Variable<REAL_T>& other) {
            info->value = other.GetValue();
        }

        ~Variable() {
            if (this->info) {
                info->Release();
            }
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
            this->Assign(Variable<REAL_T>::tape, exp, index);

        }

        /**
         * Returns a reference to the raw value.
         * 
         * @return 
         */
        REAL_T& operator*() {
            return this->info->value;
        }

        /**
         * Returns a const reference to the raw value.
         * 
         * @return 
         */
        const REAL_T& operator*() const {
            return this->info->value;
        }

        template<class A>
        inline Variable& operator=(const ExpressionBase<REAL_T, A>& exp) {

            if (Variable<REAL_T>::tape.recording) {
                size_t index = atl::Variable<REAL_T>::tape.NextIndex();
                this->Assign(atl::Variable<REAL_T>::tape, exp, index);
            } else {
                this->SetValue(exp.GetValue());
            }

            return *this;
        }

        template<class A>
        inline Variable& Assign(const ExpressionBase<REAL_T, A>& exp) {


            if (Variable<REAL_T>::tape.recording) {
                size_t index = atl::Variable<REAL_T>::tape.NextIndex();
                this->Assign(atl::Variable<REAL_T>::tape, exp, index);
            } else {
                this->SetValue(exp.GetValue());
            }

            return *this;
        }

        /**
         * Assign using the specified tape structure.
         * 
         * @param tape
         * @param exp
         * @return 
         */
        template<class A>
        inline Variable& Assign(atl::Tape<REAL_T>& tape, const ExpressionBase<REAL_T, A>& exp) {


            if (Variable<REAL_T>::tape.recording) {
                size_t index = atl::Variable<REAL_T>::tape.NextIndex();
                this->Assign(tape, exp, index);
            } else {
                this->SetValue(exp.GetValue());
            }

            return *this;
        }

        /**
         * Assign using a tape entry at at the specified index.
         * 
         * @param exp
         * @param index
         * @return 
         */
        template<class A>
        inline Variable& Assign(const ExpressionBase<REAL_T, A>& exp, size_t index) {

            if (Variable<REAL_T>::tape.recording) {
                this->Assign(atl::Variable<REAL_T>::tape, exp, index);
            } else {
                this->SetValue(exp.GetValue());
            }

            return *this;
        }

        /**
         * Assign using a specified tape at a specified entry at index.
         * 
         * @param exp
         * @param index
         * @return 
         */
        template<class A>
        inline Variable& Assign(atl::Tape<REAL_T>& tape, const ExpressionBase<REAL_T, A>& exp, size_t index) {

            if (tape.recording) {
                atl::StackEntry<REAL_T>& entry = tape.stack[index];
                exp.PushIds(entry.ids);
                entry.w = this->info;
                entry.first.resize(entry.ids.size());
                typename atl::StackEntry<REAL_T>::vi_iterator it;
                typename atl::StackEntry<REAL_T>::vi_iterator jt;
                typename atl::StackEntry<REAL_T>::vi_iterator kt;
                size_t i = 0;
                size_t j = 0;
                size_t k = 0;
                switch (tape.derivative_trace_level) {

                    case FIRST_ORDER_REVERSE:
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.first[i] = exp.EvaluateDerivative((*it)->id);
                            i++;
                        }
                        break;

                    case SECOND_ORDER_REVERSE:
                        entry.is_nl = exp.IsNonlinear();
                        exp.PushNLIds(entry.nl_ids);
                        entry.second.resize(entry.ids.size() * entry.ids.size());
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.first[i] = exp.EvaluateDerivative((*it)->id);
                            j = 0;
                            for (jt = entry.ids.begin(); jt != entry.ids.end(); ++jt) {
                                entry.second[i * entry.ids.size() + j] = exp.EvaluateDerivative((*it)->id, (*jt)->id);
                                j++;
                            }
                            i++;
                        }
                        break;

                    case THIRD_ORDER_REVERSE:
                        entry.is_nl = exp.IsNonlinear();
                        exp.PushNLIds(entry.nl_ids);
                        entry.second.resize(entry.ids.size() * entry.ids.size());
                        entry.third.resize(entry.ids.size() * entry.ids.size() * entry.ids.size());
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.first[i] = exp.EvaluateDerivative((*it)->id);
                            j = 0;
                            for (jt = entry.ids.begin(); jt != entry.ids.end(); ++jt) {
                                entry.second[i * entry.ids.size() + j] = exp.EvaluateDerivative((*it)->id, (*jt)->id);
                                k = 0;
                                for (kt = entry.ids.begin(); kt != entry.ids.end(); ++kt) {
                                    entry.third[i * entry.ids.size() * entry.ids.size() + j * entry.ids.size() + k] = exp.EvaluateDerivative((*it)->id, (*jt)->id, (*kt)->id);
                                    k++;
                                }
                                j++;
                            }
                            i++;
                        }
                        break;

                    default:
                        std::cout << "Unknown Derivative Trace Level.\n";
                        exit(0);
                }
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

        inline const Variable operator-() {
            return static_cast<REAL_T> (-1.0) * (*this);
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

        inline void SetValue(const REAL_T& val) {
            this->info->value = val;
        }

        inline const REAL_T GetValue() const {
            return info->value;
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return info->value;
        }

        /**
         * Returns false.
         * 
         * @return 
         */
        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            ids.insert(info);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            ids.insert(info);
        }

        inline void PushNLIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, bool nl = false)const {
            if (nl) {
                ids.insert(info);
            }
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            if (this->info->id == x) {
                return std::complex<REAL_T>(this->GetValue(), h);
            } else {
                return std::complex<REAL_T>(this->GetValue());
            }
        }

        inline REAL_T EvaluateDerivative(uint32_t x) const {
            return info->id == x ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return info->id == x ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
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

        const std::string ToExpressionTemplateString() const {
            std::stringstream ss;
            ss << "atl::Variable<T>";
            return ss.str();
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

