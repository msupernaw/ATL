/* 
 * File:   BigFloat.hpp
 * Author: matthewsupernaw
 *
 * Created on January 22, 2014, 3:20 PM
 */

#ifndef BIGFLOAT_HPP
#define	BIGFLOAT_HPP

#include <limits>

#include "../third_party/ttmath-0.9.3/ttmath/ttmathbig.h"
#include "../third_party/ttmath-0.9.3/ttmath/ttmath.h"


#ifndef BIGFLOAT_MANTISSA_BITS
#define BIGFLOAT_MANTISSA_BITS 64
#endif

#ifndef BIGFLOAT_EXPONENT_BITS
#define BIGFLOAT_EXPONENT_BITS 128
#endif


namespace atl {

    /*!
     * A base class for all ATL  types.
     */
    template<class T>
    class BigFloat {
    public:
        ttmath::Big<TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > value_m;

        /**
         * Constructor
         */
        BigFloat() : value_m(0) {

        }

        /**
         * Constructor
         */
        BigFloat(T value) : value_m(value) {

        }

        /**
         * Constructor
         */
        BigFloat(std::string value) : value_m(value) {

        }

        /**
         * Copy constructor.
         * 
         * @param orig
         */
        BigFloat(const BigFloat& orig) : value_m(orig.value_m) {

        }

        operator T() {
            return this->ToValue(this->ToString());
        }

        /*member functions*/

        /**
         * Assignment operator.
         * 
         * @param val
         * @return 
         */
        BigFloat<T>& operator=(const T& val) {
            this->value_m = val;
            return *this;
        }

        /**
         * Addition operator.
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator+(const BigFloat<T>& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m + rhs.value_m;
            return ret;
        }

        /**
         * Addition operator.
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator+(const T& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m + rhs;
            return ret;
        }

        /**
         * Subtraction operator.
         * 
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator-(const BigFloat<T>& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m - rhs.value_m;
            return ret;
        }

        /**
         * Subtraction operator.
         * 
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator-(const T& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m - rhs;
            return ret;
        }

        /**
         * Multiplication operator.
         * 
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator*(const BigFloat<T>& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m * rhs.value_m;
            return ret;
        }

        /**
         * Multiplication operator.
         * 
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator*(const T& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m * rhs;
            return ret;
        }

        /**
         * Division operator.
         * 
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator/(const BigFloat<T>& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m / rhs.value_m;
            return ret;
        }

        /**
         * Division operator.
         * 
         * @param rhs
         * @return 
         */
        const BigFloat<T> operator/(const T& rhs) {
            BigFloat<T> ret;
            ret.value_m = this->value_m / rhs;
            return ret;
        }

        /**
         * Addition assignment operator.
         * 
         * @param val
         * @return 
         */
        BigFloat<T>& operator+=(const T& val) {
            this->value_m += val;
            return *this;
        }

        /**
         * Subtraction assignment operator.
         * @param val
         * @return 
         */
        BigFloat<T>& operator-=(const T& val) {
            this->value_m -= val;
            return *this;
        }

        /**
         * Multiplication assignment operator.
         * 
         * @param val
         * @return 
         */
        BigFloat<T>& operator*=(const T& val) {
            this->value_m *= val;
            return *this;
        }

        /**
         * Division assignment operator.
         * 
         * @param val
         * @return 
         */
        BigFloat<T>& operator/=(const T& val) {
            this->value_m /= val;
            return *this;
        }

        /**
         * Assignment operator.
         * 
         * @param rhs
         * @return 
         */
        BigFloat<T>& operator=(const BigFloat<T>& rhs) {
            this->value_m = rhs.value_m;
            return *this;
        }

        /**
         * Addition assignment operator.
         * 
         * @param rhs
         * @return 
         */
        BigFloat<T>& operator+=(const BigFloat<T>& rhs) {
            this->value_m += rhs.value_m;
            return *this;
        }

        /**
         * Subtraction assignment operator.
         * 
         * @param rhs
         * @return 
         */
        BigFloat<T>& operator-=(const BigFloat<T>& rhs) {
            this->value_m -= rhs.value_m;
            return *this;
        }

        /**
         * Multiplication assignment operator.
         * @param rhs
         * @return 
         */
        BigFloat<T>& operator*=(const BigFloat<T>& rhs) {
            this->value_m *= rhs.value_m;
            return *this;
        }

        /**
         * Division assignment operator.
         * 
         * @param rhs
         * @return 
         */
        BigFloat<T>& operator/=(const BigFloat<T>& rhs) {
            this->value_m /= rhs.value_m;
            return *this;
        }

//        /**
//         * Division assignment operator.
//         * 
//         * @param rhs
//         * @return 
//         */
//        unsigned long operator %(const BigFloat<T>& rhs)const {
//            return this->value_m.Mod(rhs);
//        }
//
//        /**
//         * Division assignment operator.
//         * 
//         * @param rhs
//         * @return 
//         */
//        unsigned long operator %(const T& rhs) {
//            ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > temp = rhs;
//            unsigned long ret = this->value_m.Mod(temp);
//            return ret;
//        }

        /**
         * Prefix increment.
         * 
         * @return 
         */
        BigFloat<T>& operator++() {
            this->value_m++;
            return *this;
        }

        /**
         * Suffix increment.
         * @param i
         * @return 
         */
        BigFloat<T>& operator++(int i) {
            ++this->value_m;
            return *this;
        }

        /**
         * Prefix decrement.
         * 
         * @return 
         */
        BigFloat<T>& operator--() {
            this->value_m--;
            return *this;
        }

        /**
         * Suffix decrement.
         * 
         * @param i
         * @return 
         */
        BigFloat<T>& operator--(int i) {
            --this->value_m;
            return *this;
        }

        std::string ToString() const {
            std::stringstream ss;
            ss << this->value_m;
            return ss.str();
        }

        T ToValue() {
            std::istringstream ss;
            ss << this->value_m;
            T result;
            return ss >> result ? result : 0;
        }


        //Friends
        // relational operators
        template<class TT> friend const int operator==(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator!=(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator<(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator>(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator<=(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator>=(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const long operator %(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);

        template<class TT> friend const int operator==(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator!=(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator<(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator>(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator<=(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const int operator>=(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const long operator%(const TT &lhs, const BigFloat<TT>& rhs);

        template<class TT> friend const int operator==(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const int operator!=(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const int operator<(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const int operator>(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const int operator<=(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const int operator>=(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const int operator%(const BigFloat<TT>& lhs, const TT &rhs);


        // binary
        template<class TT> friend const BigFloat<TT> operator-(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const BigFloat<TT> operator/(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const BigFloat<TT> operator+(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const BigFloat<TT> operator*(const BigFloat<TT>& lhs, const BigFloat<TT>& rhs);


        template<class TT> friend const BigFloat<TT> operator-(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const BigFloat<TT> operator/(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const BigFloat<TT> operator+(TT lhs, const BigFloat<TT>& rhs);
        template<class TT> friend const BigFloat<TT> operator*(TT lhs, const BigFloat<TT>& rhs);


        template<class TT> friend const BigFloat<TT> operator-(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const BigFloat<TT> operator/(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const BigFloat<TT> operator+(const BigFloat<TT>& lhs, TT rhs);
        template<class TT> friend const BigFloat<TT> operator*(const BigFloat<TT>& lhs, TT rhs);

    protected:

        ttmath::Big < 1, 2 > GetValue() const {
            return value_m;
        }



    private:
        //          template<class T>

        T ToValue(const std::string &val) const {
            std::istringstream ss(val);
            //   ss<<val;
            T result;
            return ss >> result ? result : 0;
        }

    };

    /*!
     * Equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator==(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {

        return (lhs.GetValue() == rhs.GetValue());
    }

    /*!
     * Not equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator!=(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {

        return (lhs.GetValue() != rhs.GetValue());
    }

    /*!
     * Less than comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator<(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {

        return (lhs.GetValue() < rhs.GetValue());
    }

    /*!
     * Greater than comparison operator.
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator>(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {

        return (lhs.GetValue() > rhs.GetValue());
    }

    /*!
     * Less than equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator<=(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {

        return (lhs.GetValue() <= rhs.GetValue());
    }

    /*!
     * Greater than equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator>=(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {

        return (lhs.GetValue() >= rhs.GetValue());
    }

    template<class T> const long operator %(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {
        return lhs.value_m.Mod(rhs.value_m);
    }

    /*!
     * Equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator==(T lhs, const BigFloat<T>& rhs) {

        return (lhs == rhs.GetValue());
    }

    /*!
     * Not equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator!=(T lhs, const BigFloat<T>& rhs) {

        return (lhs != rhs.GetValue());
    }

    /*!
     * Less than comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator<(T lhs, const BigFloat<T>& rhs) {

        return (lhs < rhs.GetValue());
    }

    /*!
     * Greater than comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator>(T lhs, const BigFloat<T>& rhs) {

        return (lhs > rhs.GetValue());
    }

    /*!
     * Less than equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator<=(T lhs, const BigFloat<T>& rhs) {

        return (lhs <= rhs.GetValue());
    }

    /*!
     * Greater than equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator>=(T lhs, const BigFloat<T>& rhs) {

        return (lhs >= rhs.GetValue());
    }

    template<class T> const long operator %(const T& lhs, const BigFloat<T>& rhs) {
        return lhs % rhs.ToValue();
    }

    /*!
     * Equal to comparison operator.
     *
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator==(const BigFloat<T>& lhs, T rhs) {

        return (lhs.GetValue() == rhs);
    }

    /*!
     * Not equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator!=(const BigFloat<T>& lhs, T rhs) {

        return (lhs.GetValue() != rhs);
    }

    /*!
     * Less than comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator<(const BigFloat<T>& lhs, T rhs) {

        return (lhs.GetValue() < rhs);
    }

    /*!
     * Greater than comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator>(const BigFloat<T>& lhs, T rhs) {

        return (lhs.GetValue() > rhs);
    }

    /*!
     * Less than equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator<=(const BigFloat<T>& lhs, T rhs) {

        return (lhs.GetValue() <= rhs);
    }

    /*!
     * Greater than equal to comparison operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> inline const int operator>=(const BigFloat<T>& lhs, T rhs) {

        return (lhs.GetValue() >= rhs);
    }

    
    template<class T> const int operator %(const BigFloat<T>& lhs, const T& rhs) {
        ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > temp(rhs);
        ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > ltemp(lhs.value_m);
        return ltemp.Mod(temp);
    }


    // binary

    /*!
     * Outside class subtraction operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator-(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m - rhs.value_m;
        return ret;
    }

    /*!
     * Outside class addition operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator+(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m + rhs.value_m;
        return ret;
    }

    /*!
     * Outside class division operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator/(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m / rhs.value_m;
        return ret;
    }

    /*!
     * Outside class multiplication operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator*(const BigFloat<T>& lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m * rhs.value_m;
        return ret;
    }

    /*!
     * Outside class subtraction operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator-(T lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (lhs) - rhs.value_m;
        return ret;
    }

    /*!
     * Outside class addition operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator+(T lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (lhs) + rhs.value_m;
        return ret;
    }

    /*!
     * Outside class division operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator/(T lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (lhs) / rhs.value_m;
        return ret;
    }

    /*!
     * Outside class multiplication operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator*(T lhs, const BigFloat<T>& rhs) {
        BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (lhs) * rhs.value_m;
        return ret;
    }

    /*!
     * Outside class subtraction operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator-(const BigFloat<T>& lhs, T rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m - ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (rhs);
        return ret;
    }

    /*!
     * Outside class addition operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator+(const BigFloat<T>& lhs, T rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m + ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (rhs);
        return ret;
    }

    /*!
     * Outside class division operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator/(const BigFloat<T>& lhs, T rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m / ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (rhs);
        return ret;
    }

    /*!
     * Outside class multiplication operator.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const BigFloat<T> operator*(const BigFloat<T>& lhs, T rhs) {
        BigFloat<T> ret;
        ret.value_m = lhs.value_m * ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (rhs);
        return ret;
    }

    template<class T >
    std::ostream & operator<<(std::ostream &out, BigFloat<T> const &t) {
        out << t.value_m;
        return out;
    }

}



namespace std {

    //Math Overloads

    /*!
     * Returns the arc tangent of the BigFloat number val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> atan(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::ATan(val.value_m);
        return ret;
    }

    /*!
     * Compute arc tangent with two parameters. 
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> atan2(const atl::BigFloat<T> &lhs, const atl::BigFloat<T> &rhs) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (2) *
                ttmath::ATan(lhs.value_m / ((rhs.value_m^ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (2) +
                lhs.value_m^ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (2))^ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (.5) + rhs.value_m));
        return ret;
    }

//    /*!
//     * Compute arc tangent with two parameters.
//     * 
//     * @param lhs
//     * @param rhs
//     * @return 
//     */
//    template<class T> const atl::BigFloat<T> atan2(T lhs, const atl::BigFloat<T> &rhs) {
//        atl::BigFloat<T> ret;
//        ret.value_m = ttmath::ATan2(ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (lhs), rhs.value_m);
//        return ret;
//    }

//    /*!
//     * Compute arc tangent with two parameters.
//     *  
//     * @param lhs
//     * @param rhs
//     * @return 
//     */
//    template<class T> const atl::BigFloat<T> atan2(const atl::BigFloat<T> &lhs, T rhs) {
//        atl::BigFloat<T> ret;
//        ret.value_m = ATan2(lhs.value_m, ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (rhs));
//        return ret;
//    }

    /*!
     * Returns the cosine of the BigFloat number val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> cos(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Cos(val.value_m);
        return ret;
    }

    /*!
     * Compute exponential function for val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> exp(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Exp(val.value_m);
        return ret;
    }

    template<class T> const atl::BigFloat<T> mfexp(const atl::BigFloat<T> & x) {
        T b = T(60);
        if (x <= b && x >= T(-1) * b) {
            return std::exp(x);
        } else if (x > b) {
            return std::exp(b)*(T(1.) + T(2.) * (x - b)) / (T(1.) + x - b);
        } else {
            return std::exp(T(-1) * b)*(T(1.) - x - b) / (T(1.) + T(2.) * (T(-1) * x - b));
        }
    }

    /*!
     * Compute natural logarithm of val.
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> log(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Ln(val.value_m);
        return ret;
    }

    /*!
     * Compute natural common logarithm of val.
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> log10(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Log(val.value_m, ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (10));
        return ret;
    }



    /*
     
     float power(float x, int y)
{
    float temp;
    if( y == 0)
       return 1;
    temp = power(x, y/2);       
    if (y%2 == 0)
        return temp*temp;
    else
    {
        if(y > 0)
            return x*temp*temp;
        else
            return (temp*temp)/x;
    }
} 
     */

    /*!
     * Raise to power.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const atl::BigFloat<T> pow(const atl::BigFloat<T> &x, const atl::BigFloat<T> &y) {
        atl::BigFloat<T> temp;
        temp.value_m = x.value_m.PowFrac(y.value_m);
        return temp;
        
//        if (y == T(0))
//            return 1;
//        atl::BigFloat<T> temp = (pow(x, y/2.0));
//        if (y % T(2.0) == 0)
//            return temp * temp;
//        else {
//            if (y > T(0))
//                return x * temp * temp;
//            else
//                return (temp * temp) / x;
//        }
        //        atl::BigFloat<T> ret;
        //        // atl::BigFloat<T> rt = rhs;
        //        //
        //        //        if (rhs.value_m == ttmath::Big<TTMATH_BITS(BIGFLOAT_MANTISSA),TTMATH_BITS(BIGFLOAT_EXPONENT) > (0.0)) {
        //        //            ret.value_m = ttmath::Big<TTMATH_BITS(BIGFLOAT_MANTISSA),TTMATH_BITS(BIGFLOAT_EXPONENT) > (1);
        //        //            return ret;
        //        //        }
        //        //        ret = std::pow(lhs, rhs / atl::BigFloat<T>  (2.0));
        //        //
        //        //        if ((rt.value_m.Mod(ttmath::Big<TTMATH_BITS(BIGFLOAT_MANTISSA),TTMATH_BITS(BIGFLOAT_EXPONENT) > (2.0))) == 0) {
        //        //                return ret *ret;
        //        //            } else {
        //        //
        //        //            if (rhs > atl::BigFloat<T> (0)) {
        //        //                    return lhs * ret*ret;
        //        //                } else {
        //        //
        //        //                return (ret * ret) / rhs;
        //        //            }
        //        //        }
        //
        //
        //        ret.value_m = lhs.value_m;
        //        ret.value_m.PowFrac(rhs.value_m);
        //        return ret;
    }

    /*!
     * Raise to power.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const atl::BigFloat<T> pow(T lhs, const atl::BigFloat<T> & rhs) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (lhs);
        ret.value_m.PowFrac(rhs.value_m);

        return ret;
    }

    /*!
     * Raise to power.
     * 
     * @param lhs
     * @param rhs
     * @return 
     */
    template<class T> const atl::BigFloat<T> pow(const atl::BigFloat<T> &lhs, T rhs) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (lhs.value_m);
        ret.value_m.PowFrac(ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (rhs));

        return ret;
    }

    /*!
     * Returns the sine of val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> sin(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Sin(val.value_m);

        return ret;
    }

    /*!
     * Compute square root val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> sqrt(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (val.value_m);
        ret.value_m.Sqrt();
        return ret;
    }

    /*!
     * Returns the tangent of val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> tan(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Tan(val.value_m);

        return ret;
    }

    /*!
     * Returns the arc cosine of val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> acos(const atl::BigFloat<T> & val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::ACos(val.value_m);

        return ret;
    }

    /*!
     * Returns the arc sine of val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> asin(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::ASin(val.value_m);

        return ret;
    }

    /*!
     * Returns the hyperbolic sin of val.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> sinh(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Sinh(val.value_m);

        return ret;
    }

    /*!
     * Returns the hyperbolic cosine of val.
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> cosh(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Cosh(val.value_m);

        return ret;
    }

    /*!
     * Returns the hyperbolic tangent of val.
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> tanh(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Tanh(val.value_m);

        return ret;
    }

    /*!
     * Compute absolute value of val.
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> fabs(const atl::BigFloat<T> &val) {
        atl::BigFloat<T> ret;
        ret.value_m = ttmath::Big < TTMATH_BITS(BIGFLOAT_MANTISSA_BITS), TTMATH_BITS(BIGFLOAT_EXPONENT_BITS) > (val.value_m);
        ret.value_m.Abs();

        return ret;
    }

    /*!
     * Round down value.
     * 
     * @param val
     * @return 
     */
    template<class T> const atl::BigFloat<T> floor(const atl::BigFloat<T> &val) {

    }

    template<class T>const atl::BigFloat<T>& max(const atl::BigFloat<T> &a, const atl::BigFloat<T> &b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    template<class T> const atl::BigFloat<T>& min(const atl::BigFloat<T> &a, const atl::BigFloat<T> &b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }


    //namespace std {

    template<class T>
    class numeric_limits<atl::BigFloat<T> > : public numeric_limits<T> {
    };





}


#endif	/* BIGFLOAT_HPP */

