/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Transformations.h
 * Author: matthewsupernaw
 *
 * Created on March 8, 2017, 2:27 PM
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <cmath>

namespace atl{
    
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
            if (val <= min_ || val >= max_)std::cout << "value exceeds bounds....\n";
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
            REAL_T p = (val - min_) / (max_ - min_);
            return std::log(p / (1.0 - p));
        }

        virtual REAL_T Internal2External(REAL_T val, REAL_T min_, REAL_T max_) const {
            REAL_T p = std::exp(val) / (1.0 + std::exp(val));
            return p * (max_ - min_) + min_;
        }

        virtual REAL_T DerivativeInternal2External(REAL_T val, REAL_T min_, REAL_T max_)const {
            return (std::exp(val) * std::log(M_E)*(max_ - min_)) / (std::exp(val) + 1.0)-
                    (std::exp(static_cast<REAL_T> (2.0 * val)) * std::log(M_E)*(max_ - min_)) / std::pow((std::exp(val) + 1), 2.0);
        }
    };

    
}

#endif /* TRANSFORMATIONS_H */

