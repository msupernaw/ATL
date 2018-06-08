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
#include "Transformations.h"
#include <vector>
#include <valarray>
#include <cassert>

namespace atl {

    
    
     /**
     * Base class for parameter transformations. Used in optimization
     * problems involving bounded parameters.
     * @param val
     * @param min
     * @param max
     * @return
     */
    template<typename REAL_T>
    class ParameterTransformation {
    public:
        /**
         * Convert the external value to its internal representation.
         *
         * @param val
         * @param min
         * @param max
         * @return
         */
        virtual REAL_T External2Internal(REAL_T val, REAL_T min_, REAL_T max_) const = 0;
        /**
         * Convert a variables internal value to its external representation.
         * @param val
         * @param min
         * @param max
         * @return
         */
        virtual REAL_T Internal2External(REAL_T val, REAL_T min_, REAL_T max_) const = 0;
        /**
         * The derivative of Internal2External.
         * @param val
         * @param min
         * @param max
         * @return
         */
        virtual REAL_T DerivativeInternal2External(REAL_T val, REAL_T min_, REAL_T max_) const = 0;

    };

    /**
     * Tanh transformation for a variable.
     */
    template<typename REAL_T>
    class TanhParameterTransformation : public ParameterTransformation<REAL_T> {
    public:

        virtual REAL_T External2Internal(REAL_T val, REAL_T min_, REAL_T max_) const {
            return min_ + .5 * (max_ - min_)*(1.0 + std::tanh(val));
        }

        virtual REAL_T Internal2External(REAL_T val, REAL_T min_, REAL_T max_) const {
            return std::atanh(2.0 * (val - min_) / (max_ - min_) - 1.0);
        }

        virtual REAL_T DerivativeInternal2External(REAL_T val, REAL_T min_, REAL_T max_)const {
            return 2.0 / ((max_ - min_) * std::pow((1.0 - ((2.0 * (val - min_)) / max_ - min_ - 1.0)), 2.0));
        }
    };

    /**
     * Sine transformation for a variable.
     */
    template<typename REAL_T>
    class SinParameterTransformation : public ParameterTransformation<REAL_T> {
    public:

        virtual REAL_T External2Internal(REAL_T val, REAL_T min_, REAL_T max_) const {
            if (val < min_ || val > max_)std::cout << "value exceeds bounds....\n";
            //            return std::asin((2.0 * val) / (max_ - min_) - min_ / (max_ - min_) - max_ / (max_ - min_));
            return std::asin((2.0 * (val - min_) / (max_ - min_)) - 1.0);
        }

        virtual REAL_T Internal2External(REAL_T val, REAL_T min_, REAL_T max_) const {
            //            return min_ + (0.5 * (max_ - min_))*(std::sin(val) + 1.);
            return min_ + (std::sin(val) + 1.0)*((max_ - min_) / 2.0);
        }

        virtual REAL_T DerivativeInternal2External(REAL_T val, REAL_T min_, REAL_T max_)const {
            return 0.5 * ((max_ - min_) * std::cos(val));
            //            return ((max_ - min_) * std::cos(val)) / 2.0;
        }
    };

    template<typename REAL_T>
    class ADMBParameterTransformation : public ParameterTransformation<REAL_T> {
    public:

        virtual REAL_T External2Internal(REAL_T val, REAL_T min_, REAL_T max_) const {
            return std::asin(2.0 * (val - min_) / (max_ - min_) - 1.0) / (M_PI / 2.0);
        }

        virtual REAL_T Internal2External(REAL_T val, REAL_T min_, REAL_T max_) const {
            return min_ + (max_ - min_)*(.5 * std::sin(val * M_PI / 2.0) + .5);
        }

        virtual REAL_T DerivativeInternal2External(REAL_T val, REAL_T min_, REAL_T max_)const {
            return (max_ - min_)*.5 * M_PI / 2.0 * std::cos(val * M_PI / 2.0);
        }
    };

    template<typename REAL_T>
    class LogitParameterTransformation : public ParameterTransformation<REAL_T> {
    public:

        virtual REAL_T External2Internal(REAL_T val, REAL_T min_, REAL_T max_)const {
            if(val == min_){
                val+=std::numeric_limits<REAL_T>::epsilon();
            }else if(val == max_){
                val-=std::numeric_limits<REAL_T>::epsilon();
            }
            
            REAL_T p = ((val)- min_) / (max_ - min_);
            return std::log(p / (1.0 - p));
        }

        virtual REAL_T Internal2External(REAL_T val, REAL_T min_, REAL_T max_) const {
            REAL_T p = std::exp(val) / (1.0 + std::exp(val));
            return p * (max_ - min_) + min_;
        }

        virtual REAL_T DerivativeInternal2External(REAL_T val, REAL_T min_, REAL_T max_)const {
//            return ((max_-min_)*std::exp(val))/(std::exp(val)+1.0)-((max_-min_)*std::exp((2.0*val)))/std::pow((std::exp(val)+1.0),2.0);
//            return ((max_−min_)*std::exp(val)/(std::exp(val)+1.0)−((max_−min_)*std::exp((2.0*val))/std::pow(std::exp(val)+1.0),2.0);
            return (std::exp(val) * std::log(M_E)*(max_ - min_)) / (std::exp(val) + 1.0)-
                    (std::exp(static_cast<REAL_T> (2.0 * val)) * std::log(M_E)*(max_ - min_)) / std::pow((std::exp(val) + 1), 2.0);
        }
    };

    
    

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
        typedef typename std::shared_ptr<VariableInfo<REAL_T> > VariableInfoPtr;
        static Tape<REAL_T> tape;
        typedef REAL_T base_type;

        VariableInfoPtr info; //(new atl::VariableInfo<REAL_T>());

        static std::shared_ptr<ParameterTransformation<REAL_T> > default_transformation;
        ParameterTransformation<REAL_T>* transformation;

        REAL_T min_boundary_m;
        REAL_T max_boundary_m;
        bool bounded_m;
        static bool show;
        std::string name;

        Variable(REAL_T value = static_cast<REAL_T> (0.0),
                REAL_T min_boundary = std::numeric_limits<REAL_T>::min(),
                REAL_T max_boundary = std::numeric_limits<REAL_T>::max()) :
        bounded_m(false),
        min_boundary_m(min_boundary),
        max_boundary_m(max_boundary),
        transformation(default_transformation.get()) {

            info = std::make_shared<VariableInfo<REAL_T> >(value);

            //            info->value = v;
        }

        Variable(int value,
                REAL_T min_boundary = std::numeric_limits<REAL_T>::min(),
                REAL_T max_boundary = std::numeric_limits<REAL_T>::max()) :
        bounded_m(false),
        min_boundary_m(min_boundary),
        max_boundary_m(max_boundary),
        transformation(default_transformation.get()) {

            info = std::make_shared<VariableInfo<REAL_T> >(static_cast<REAL_T> (value));
            //            info->value = v;
        }

        Variable(long value,
                REAL_T min_boundary = std::numeric_limits<REAL_T>::min(),
                REAL_T max_boundary = std::numeric_limits<REAL_T>::max()) :
        bounded_m(false),
        min_boundary_m(min_boundary),
        max_boundary_m(max_boundary),
        transformation(default_transformation.get()) {

            info = std::make_shared<VariableInfo<REAL_T> >(static_cast<REAL_T> (value));
            //            info->value = v;
        }

        //        Variable(const Variable<REAL_T>& other) :
        //        info(other.info), transformation(other.transformation),
        //                min_boundary_m(other.min_boundary_m), 
        //                max_boundary_m(other.max_boundary_m), 
        //                bounded_m(other.bounded_m), 
        //                name(other.name) {
        //            
        //        }

        Variable(const Variable<REAL_T>& other) :
        info(other.info),
        min_boundary_m(other.min_boundary_m),
        max_boundary_m(other.max_boundary_m),
        bounded_m(other.bounded_m),
        transformation(other.transformation) {

        }

        template<class A>
        Variable(const ExpressionBase<REAL_T, A>& exp) :
        bounded_m(false),
        min_boundary_m(std::numeric_limits<REAL_T>::min()),
        max_boundary_m(std::numeric_limits<REAL_T>::max()),
        transformation(default_transformation.get()) {

            info = std::make_shared<VariableInfo<REAL_T> >(static_cast<REAL_T> (0.0));

            if (Variable<REAL_T>::tape.recording) {

                size_t index = atl::Variable<REAL_T>::tape.NextIndex();
                this->Assign(Variable<REAL_T>::tape, exp, index);
            } else {
                this->SetValue(exp.GetValue());
            }
        }

        virtual ~Variable() {

        }

        //        operator int() const {
        //            return static_cast<int> (this->info->value);
        //        }
        //
        //        operator long() const {
        //            return static_cast<long> (this->info->value);
        //        }
        //

        operator REAL_T() {
            return (this->info->value);
        }

        operator REAL_T() const {
            return (this->info->value);
        }

        void Copy(const Variable<REAL_T>& other) {
            info = (other.info);
            min_boundary_m = (other.min_boundary_m);
            max_boundary_m = (other.max_boundary_m);
            bounded_m = (other.bounded_m);
            transformation = (other.transformation);

        }

        Variable& operator=(const REAL_T& v) {
            this->info->value = v;
            return *this;
        }

        //        Variable& operator=(const Variable<REAL_T>& other) {
        //            if (Variable<REAL_T>::tape.recording) {
        //                //                this->info = other.info;
        //                size_t index = atl::Variable<REAL_T>::tape.NextIndex();
        //                this->Assign(Variable<REAL_T>::tape, other, index);
        //            } else {
        //                this->SetValue(other.GetValue());
        //            }
        //
        //            return *this;
        //        }



        //        /**
        //         * Returns a reference to the raw value.
        //         * 
        //         * @return 
        //         */
        //        REAL_T& operator*() {
        //            return this->info->value;
        //        }
        //
        //        /**
        //         * Returns a const reference to the raw value.
        //         * 
        //         * @return 
        //         */
        //        const REAL_T& operator*() const {
        //            return this->info->value;
        //        }

        template<class A>
        inline Variable operator=(const ExpressionBase<REAL_T, A>& exp) {

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
                entry.w->count++;
                entry.w->is_nl = true;
                entry.first.resize(entry.ids.size(), static_cast<REAL_T> (0.0));
                typename atl::StackEntry<REAL_T>::vi_iterator it;
                typename atl::StackEntry<REAL_T>::vi_iterator jt;
                typename atl::StackEntry<REAL_T>::vi_iterator kt;
                size_t i = 0;
                size_t j = 0;
                size_t k = 0;
                entry.wv = exp.GetValue();
                switch (tape.derivative_trace_level) {

                    case FIRST_ORDER_REVERSE:
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            entry.first[i] = exp.EvaluateFirstDerivative((*it)->id);
                            i++;
                        }
                        break;
                        
                    case FIRST_ORDER_REVERSE_COMPLEX_STEP:
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            entry.first[i] = exp.ComplexEvaluate((*it)->id).imag()/1e-20;
                            i++;
                        }
                        break;

                    case SECOND_ORDER_REVERSE:
                        entry.w->is_nl = exp.IsNonlinear();
                        entry.is_nl = exp.IsNonlinear();

                        entry.second.resize(entry.ids.size() * entry.ids.size(), static_cast<REAL_T> (0.0));

                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {

                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            entry.first[i] = exp.EvaluateFirstDerivative((*it)->id);
                            j = 0;
                            for (jt = entry.ids.begin(); jt != entry.ids.end(); ++jt) {
                                entry.second[i * entry.ids.size() + j] = exp.EvaluateSecondDerivative((*it)->id, (*jt)->id);
                                j++;
                            }
                            i++;
                        }
                        break;

                    case THIRD_ORDER_REVERSE:
                        entry.w->is_nl = exp.IsNonlinear();
                        entry.is_nl = exp.IsNonlinear();

                        entry.second.resize(entry.ids.size() * entry.ids.size(), static_cast<REAL_T> (0.0));
                        entry.third.resize(entry.ids.size() * entry.ids.size() * entry.ids.size(), static_cast<REAL_T> (0.0));
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            (*it)->live++;
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            entry.first[i] = exp.EvaluateFirstDerivative((*it)->id);
                            j = 0;
                            for (jt = entry.ids.begin(); jt != entry.ids.end(); ++jt) {
                                entry.second[i * entry.ids.size() + j] = exp.EvaluateSecondDerivative((*it)->id, (*jt)->id);
                                k = 0;
                                for (kt = entry.ids.begin(); kt != entry.ids.end(); ++kt) {

                                    entry.third[i * entry.ids.size() * entry.ids.size() + j * entry.ids.size() + k] =
                                            exp.EvaluateThirdDerivative((*it)->id, (*jt)->id, (*kt)->id);
                                    k++;
                                }
                                j++;
                            }
                            i++;
                        }
                        break;
                    case atl::UTPM_REVERSE:

                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                            (*it)->tayor_coefficients.resize(tape.taylor_order + 1);
                            (*it)->tayor_coefficients[0] = (*it)->value;
                        }
                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            (*it)->tayor_coefficients[1] = static_cast<REAL_T> (1.0);
                            for (i = 0; i <= tape.taylor_order; i++) {
                                entry.taylor_coeff[(*it)->id].push_back(exp.Taylor(i));
                            }
                            (*it)->tayor_coefficients[1] = static_cast<REAL_T> (0.0);
                        }

                        break;

                    case DYNAMIC_RECORD:

                        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                            entry.min_id = std::min((*it)->id, entry.min_id);
                            entry.max_id = std::max((*it)->id, entry.max_id);
                        }
                        entry.exp = exp.ToDynamic();

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

        inline Variable operator-=(const REAL_T& val) {
            *this = *this-val;
            return *this;
        }

        inline Variable<REAL_T> operator-() const {
            return static_cast<REAL_T> (-1.0) * (*this);
        }

        template<class A>
        inline Variable operator-=(const ExpressionBase<REAL_T, A>& exp) {
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

        /**
         * Returns the variables name.
         * 
         * @return 
         */
        std::string GetName() const {
            return name;
        }

        /**
         * Sets the variables name. 
         * 
         * @param name
         */
        void SetName(std::string name) {
            this->name = name;
        }

        /**
         * Returns the variables transformation functor.
         * @return 
         */
        ParameterTransformation<REAL_T>& GetParameterTransformation() {
            return this->transformation;
        }
        
        /**
         * Returns the variables transformation functor.
         * @return
         */
        void SetParameterTransformation(ParameterTransformation<REAL_T>* trans) {
             this->transformation = trans;
        }


        /**
         * Returns the max boundary.
         * 
         * @return 
         */
        REAL_T GetMaxBoundary() const {
            return max_boundary_m;
        }

        /**
         * Sets the max boundary.
         * 
         * @param max_boundary
         */
        void SetMaxBoundary(REAL_T max_boundary) {
            this->bounded_m = true;
            this->max_boundary_m = max_boundary;
        }

        /**
         * Returns the min boundary.
         * 
         * @return 
         */
        REAL_T GetMinBoundary() const {
            return min_boundary_m;
        }

        /**
         * Sets the min boundary.
         */
        void SetMinBoundary(REAL_T min_boundary) {
             this->bounded_m = true;
            this->min_boundary_m = min_boundary;
        }

        bool IsBounded() const {
            return this->bounded_m;
        }
        
               /**
         * Sets the boundary values for this variable. 
         * 
         * @param min_boundary
         * @param max_boundary
         */
        inline void SetBounds(REAL_T min_boundary, REAL_T max_boundary) {
            this->bounded_m = true;
            this->SetMinBoundary(min_boundary);
            this->SetMaxBoundary(max_boundary);
            if (this->GetValue()<this->GetMinBoundary() || this->GetValue()>this->GetMaxBoundary() || this->GetValue() == 0.0) {
                this->SetValue((min_boundary + max_boundary) / 2.0);
            }
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
         * Returns the internal value from the variables transformation.
         */
        inline const REAL_T GetInternalValue() const {
            if (this->IsBounded()) {
                return this->transformation->External2Internal(this->GetValue(), this->GetMinBoundary(), this->GetMaxBoundary());
            } else {
                return this->GetValue();
            }
        }

        /**
         * Returns the scaled gradient value from the variables transformation.
         * @param x
         * @return 
         */
        REAL_T GetScaledGradient(REAL_T x) {
            if (bounded_m) {
                return this->transformation->DerivativeInternal2External(x, min_boundary_m, max_boundary_m);
            } else {
                return 1.0;
            }
        }

        /**
         * If this variable is bounded, the value of
         * v will be transformed from internal to external
         * representation and set to the value of this
         * variable. See class ParameterTransformation
         * @param v
         */
        inline void UpdateValue(REAL_T v) {

            if (this->IsBounded()) {
                this->SetValue(this->transformation->Internal2External(v, this->GetMinBoundary(), this->GetMaxBoundary()));

            } else {
                this->SetValue(v);
            }
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

        inline const REAL_T Taylor(uint32_t degree) const {


            if (degree >= this->info->tayor_coefficients.size()) {
                this->info->tayor_coefficients.resize(degree + 10);
                this->info->tayor_coefficients[0] = this->GetValue();
            }
            assert(degree < this->info->tayor_coefficients.size());
            return this->info->tayor_coefficients[degree];
        }

        virtual std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
#ifdef USE_DELEGATES
            return atl::VariableFunctions<REAL_T>::Create(this->info);
#else
            std::shared_ptr<DynamicExpressionBase<REAL_T> > var =
                    std::make_shared<atl::VariableDynamic<REAL_T> >(this->info);
            return std::shared_ptr<VariableDynamic<REAL_T> >(new VariableDynamic<REAL_T>(this->info)); //, VariableDynamic<REAL_T>::free);
#endif
        }

        inline REAL_T EvaluateFirstDerivative(uint32_t x) const {
            return info->id == x ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateSecondDerivative(uint32_t x, uint32_t y) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateThirdDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateFirstDerivativeAt(uint32_t x, size_t i, size_t j = 0) const {
            return info->id == x ? static_cast<REAL_T> (1.0) : static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateSecondDerivativeAt(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return static_cast<REAL_T> (0.0);
        }

        inline REAL_T EvaluateThirdDerivativeAt(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
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
            ss << "atl::Variable<T>[" << this->info->value << "]";
            return ss.str();
        }

        static void SetRecording(bool recording) {
            atl::Variable<REAL_T>::tape.recording = recording;
        }

        /**
         * Accumulates derivatives in a GradientStructure and puts the gradient 
         * into a std::vector. 
         * 
         * @param gs
         * @param variables
         * @param gradient
         */
        static void ComputeGradient(Tape<REAL_T>& gs, std::vector<atl::Variable<REAL_T>* >& variables, std::vector<REAL_T>& gradient) {
            gs.AccumulateFirstOrder();
            int size = variables.size();
            gradient.resize(size);
            for (int i = 0; i < size; i++) {
                gradient[i] = gs.Value(variables[i]->info->id);
            }
        }

        /**
         * Accumulates derivatives in a GradientStructure and puts the gradient 
         * into a std::valarray. 
         * 
         * @param gs
         * @param variables
         * @param gradient
         */
        static void ComputeGradient(Tape<REAL_T>& gs, std::vector<atl::Variable<REAL_T>* >& variables, std::valarray<REAL_T>& gradient) {
            gs.AccumulateFirstOrder();
            int size = variables.size();
            gradient.resize(size);
            for (int i = 0; i < size; i++) {
                gradient[i] = gs.Value(variables[i]->info->id);
            }
        }

        /**
         * Accumulates derivatives in a GradientStructure and puts the gradient 
         * and second order derivatives into std::vector's.  
         * @param gs
         * @param variables
         * @param gradient
         * @param hessian
         */
        static void ComputeGradientAndHessian(Tape<REAL_T>& gs,
                std::vector<atl::Variable<REAL_T>* >& variables,
                std::vector<REAL_T>& gradient, std::vector<std::vector<REAL_T> >& hessian) {
            gs.AccumulateSecondOrder();
            int size = variables.size();
            gradient.resize(size);
            hessian.resize(size);
            for (int i = 0; i < size; i++) {
                gradient[i] = gs.Value(variables[i]->info->id);
                hessian[i].resize(size);
                for (int j = 0; j < size; j++) {
                    hessian[i][j] = gs.Value(variables[i]->info->id, variables[j]->info->id); //hessian_row[variables[j]->info];
                }
            }
        }

        /**
         * Accumulates derivatives in a GradientStructure and puts the gradient 
         * and second order derivatives into std::valarray's.  
         * @param gs
         * @param variables
         * @param gradient
         * @param hessian
         */
        static void ComputeGradientAndHessian(Tape<REAL_T>& gs,
                std::vector<atl::Variable<REAL_T>* >& variables,
                std::valarray<REAL_T>& gradient, std::valarray<std::valarray<REAL_T> >& hessian) {
            gs.AccumulateSecondOrder();
            int size = variables.size();
            gradient.resize(size);
            hessian.resize(size);
            for (int i = 0; i < size; i++) {
                gradient[i] = gs.Value(variables[i]->info->id);
                hessian[i].resize(size);
                for (int j = 0; j < size; j++) {
                    hessian[i][j] = gs.Value(variables[i]->info->id, variables[j]->info->id);
                }
            }
        }

        /**
         * Accumulates derivatives in a GradientStructure and puts the gradient, 
         * second and third order derivatives into std::vector's.  
         * @param gs
         * @param variables
         * @param gradient
         * @param hessian
         */
        static void ComputeUpToThirdOrderMixed(Tape<REAL_T>& gs,
                std::vector<atl::Variable<REAL_T>* >& variables,
                std::vector<REAL_T>& gradient, std::vector<std::vector<REAL_T> >& hessian,
                std::vector<std::vector<std::vector<REAL_T> > >& third) {
            gs.AccumulateThirdOrder();
            int size = variables.size();
            gradient.resize(size);
            hessian.resize(size);
            for (int i = 0; i < size; i++) {
                gradient[i] = gs.Value(variables[i]->info->id); //variables[i]->info->dvalue;
                hessian[i].resize(size);
                for (int j = 0; j < size; j++) {
                    hessian[i][j] = gs.Value(variables[i]->info->id, variables[j]->info->id); //variables[i]->info->GetHessianRowValue(variables[j]->info);
                    for (int k = 0; k < size; k++) {
                        third[i][j][k] = gs.Value(variables[i]->info->id, variables[j]->info->id, variables[k]->info->id); //variables[i]->info->GetThirdOrderValue(variables[j]->info, variables[k]->info);
                    }
                }

            }
        }

        /**
         * Accumulates derivatives in a GradientStructure and puts the gradient, 
         * second and third order derivatives into std::valarray's.  
         * @param gs
         * @param variables
         * @param gradient
         * @param hessian
         */
        static void ComputeUpToThirdOrderMixed(Tape<REAL_T>& gs,
                std::vector<atl::Variable<REAL_T>* >& variables,
                std::valarray<REAL_T>& gradient, std::valarray<std::valarray<REAL_T> >& hessian,
                std::valarray<std::valarray<std::valarray<REAL_T> > >& third) {
            gs.AccumulateThirdOrder();
            int size = variables.size();
            gradient.resize(size);
            hessian.resize(size);
            for (int i = 0; i < size; i++) {
                gradient[i] = gs.Value(variables[i]->info->id);
                hessian[i].resize(size);
                for (int j = 0; j < size; j++) {
                    hessian[i][j] = gs.Value(variables[i]->info->id, variables[j]->info->id); //variables[i]->info->GetHessianRowValue(variables[j]->info);
                    for (int k = 0; k < size; k++) {
                        third[i][j][k] = gs.Value(variables[i]->info->id, variables[j]->info->id, variables[k]->info->id); //variables[i]->info->GetThirdOrderValue(variables[j]->info, variables[k]->info);
                    }
                }

            }
        }


    };

    template<typename REAL_T>
    Tape<REAL_T> Variable<REAL_T>::tape(10000);

    template<typename REAL_T>
    bool Variable<REAL_T>::show = false;

    template<typename REAL_T>
    std::shared_ptr<ParameterTransformation<REAL_T> > Variable<REAL_T>::default_transformation(new atl::LogitParameterTransformation<double>());

    template<typename REAL_T>
    std::ostream& operator<<(std::ostream& out, const Variable<REAL_T>& v) {
        out << v.GetValue();
        return out;
    }




}



#endif /* VARIABLE_HPP */


