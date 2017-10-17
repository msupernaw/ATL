/* 
 * File:   SIMD.hpp
 * Author: matthewsupernaw
 *
 * Created on July 2, 2015, 7:01 AM
 */

#ifndef SIMD_HPP
#define	SIMD_HPP

#include <iostream>

#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64)) && ! defined(__x86_64__)
#define __x86_64__ 1
#endif

// Find sse instruction set from compiler macros if SSE_INSTR_SET not defined
// Note: Not all compilers define these macros automatically
#ifndef SSE_INSTR_SET
#if defined ( __AVX2__ )
#define SSE_INSTR_SET 8
#elif defined ( __AVX__ )
#define SSE_INSTR_SET 7
#elif defined ( __SSE4_2__ )
#define SSE_INSTR_SET 6
#elif defined ( __SSE4_1__ )
#define SSE_INSTR_SET 5
#elif defined ( __SSSE3__ )
#define SSE_INSTR_SET 4
#elif defined ( __SSE3__ )
#define SSE_INSTR_SET 3
#elif defined ( __SSE2__ ) || defined ( __x86_64__ )
#define SSE_INSTR_SET 2
#elif defined ( __SSE__ )
#define SSE_INSTR_SET 1
#elif defined ( _M_IX86_FP )           // Defined in MS compiler on 32bits system. 1: SSE, 2: SSE2
#define SSE_INSTR_SET _M_IX86_FP
#else
#define SSE_INSTR_SET 0
#endif // instruction set defines
#endif // SSE_INSTR_SET

#if SSE_INSTR_SET > 7                  // AVX2 and later
#ifdef __GNUC__
#include <x86intrin.h>         // x86intrin.h includes header files for whatever instruction
// sets are specified on the compiler command line, such as:
// xopintrin.h, fma4intrin.h
#else
#include <immintrin.h>         // MS version of immintrin.h covers AVX, AVX2 and FMA3
#endif // __GNUC__
#elif SSE_INSTR_SET == 7
#include <immintrin.h>             // AVX
#elif SSE_INSTR_SET == 6
#include <nmmintrin.h>             // SSE4.2
#elif SSE_INSTR_SET == 5
#include <smmintrin.h>             // SSE4.1
#elif SSE_INSTR_SET == 4
#include <tmmintrin.h>             // SSSE3
#elif SSE_INSTR_SET == 3
#include <pmmintrin.h>             // SSE3
#elif SSE_INSTR_SET == 2
#include <emmintrin.h>             // SSE2
#elif SSE_INSTR_SET == 1
#include <xmmintrin.h>             // SSE
#endif

#if SSE_INSTR_SET > 7
#define USE_AVX
#endif

#ifndef USE_AVX

#if SSE_INSTR_SET > 0
#define USE_SSE
#endif

#endif
#include <memory>
//
//namespace util {
//
//    /**
//     * STL-compliant allocator that allocates aligned memory.
//     * @tparam T Type of the element to allocate.
//     * @tparam Alignment Alignment of the allocation, e.g. 16.
//     * @ingroup AlignedAllocator
//     */
//    template <class T, size_t Alignment>
//    struct aligned_allocator
//    : public std::allocator<T> // Inherit construct(), destruct() etc.
//    {
//#if 0
//        typedef size_t size_type;
//        typedef ptrdiff_t difference_type;
//        typedef T* pointer;
//        typedef const T* const_pointer;
//        typedef T& reference;
//        typedef const T& const_reference;
//        typedef T value_type;
//#endif
//        typedef typename std::allocator<T>::size_type size_type;
//        typedef typename std::allocator<T>::pointer pointer;
//        typedef typename std::allocator<T>::const_pointer const_pointer;
//
//        /// Defines an aligned allocator suitable for allocating elements of type
//        /// @c U.
//
//        template <class U>
//        struct rebind {
//            typedef aligned_allocator<U, Alignment> other;
//        };
//
//        /// Default-constructs an allocator.
//
//        aligned_allocator() throw () {
//        }
//
//        /// Copy-constructs an allocator.
//
//        aligned_allocator(const aligned_allocator & other) throw ()
//        : std::allocator<T > (other) {
//        }
//
//        /// Convert-constructs an allocator.
//
//        template <class U >
//        aligned_allocator(const aligned_allocator<U, Alignment>&) throw () {
//        }
//
//        /// Destroys an allocator.
//
//        ~aligned_allocator() throw () {
//        }
//
//        /// Allocates @c n elements of type @c T, aligned to a multiple of
//        /// @c Alignment.
//
//        pointer allocate(size_type n) {
//            return allocate(n, const_pointer(0));
//        }
//
//        /// Allocates @c n elements of type @c T, aligned to a multiple of
//        /// @c Alignment.
//
//        pointer allocate(size_type n, const_pointer /* hint */) {
//            void *p;
//#ifndef _WIN32
//            if (posix_memalign(&p, Alignment, n * sizeof (T)) != 0)
//                p = NULL;
//#else
//            p = _aligned_malloc(n * sizeof (T), Alignment);
//#endif
//            if (!p)
//                throw std::bad_alloc();
//            return static_cast<pointer> (p);
//        }
//
//        /// Frees the memory previously allocated by an aligned allocator.
//
//        void deallocate(pointer p, size_type /* n */) {
//#ifndef _WIN32
//            free(p);
//#else
//            _aligned_free(p);
//#endif
//        }
//    };
//
//    /**
//     * Checks whether two aligned allocators are equal. Two allocators are equal
//     * if the memory allocated using one allocator can be deallocated by the other.
//     * @returns Always @c true.
//     * @ingroup AlignedAllocator
//     */
//    template <class T1, size_t A1, class T2, size_t A2>
//    bool operator ==(const aligned_allocator<T1, A1> &, const aligned_allocator<T2, A2> &) {
//        return true;
//    }
//
//    /**
//     * Checks whether two aligned allocators are not equal. Two allocators are equal
//     * if the memory allocated using one allocator can be deallocated by the other.
//     * @returns Always @c false.
//     * @ingroup AlignedAllocator
//     */
//    template <class T1, size_t A1, class T2, size_t A2>
//    bool operator !=(const aligned_allocator<T1, A1> &, const aligned_allocator<T2, A2> &) {
//        return false;
//    }
//
//} // namespace util
//


namespace simd {


    class vector4f;
    class vector2d;

    template <class T>
    struct simd_traits {
        typedef T type;
        static const size_t size = 1;
        static const bool simd_available = false;
    };

#ifdef USE_SSE

    template <>
    struct simd_traits<float> {
        typedef vector4f type;
        static const size_t size = 4;
        static const bool simd_available = true;
    };

    template <>
    struct simd_traits<double> {
        typedef vector2d type;
        static const size_t size = 2;
        static const bool simd_available = true;
    };
#elif USE_AVX

    template <>
    struct simd_traits<float> {
        typedef vector8f type;
        static const size_t size = 8;
    };

    template <>
    struct simd_traits<double> {
        typedef vector4d type;
        static const size_t size = 4;
    };
#endif

    template <class X>
    struct simd_vector_traits {
        typedef X value_type;
    };

    template <>
    struct simd_vector_traits<vector4f> {
        typedef float value_type;
    };

    template <>
    struct simd_vector_traits<vector2d> {
        typedef double value_type;
    };

    template <class X>
    class simd_vector {
    public:

        typedef typename simd_vector_traits<X>::value_type value_type;

        // downcast operators so we can call methods in the inheriting classes

        inline X& operator()() {
            return *static_cast<X*> (this);
        }

        inline const X& operator()() const {
            return *static_cast<const X*> (this);
        }

        // Additional assignment operators

        inline X& operator+=(const X& rhs) {
            (*this)() = (*this)() + rhs;
            return (*this)();
        }

        inline X& operator+=(const value_type& rhs) {
            (*this)() = (*this)() + X(rhs);
            return (*this)();
        }

        inline X& operator-=(const X& rhs) {
            (*this)() = (*this)() - rhs;
            return (*this)();
        }

        inline X& operator-=(const value_type& rhs) {
            (*this)() = (*this)() - X(rhs);
            return (*this)();
        }

        inline X& operator*=(const X& rhs) {
            (*this)() = (*this)() * rhs;
            return (*this)();
        }

        inline X& operator*=(const value_type& rhs) {
            (*this)() = (*this)() * X(rhs);
            return (*this)();
        }

        inline X& operator/=(const X& rhs) {
            (*this)() = (*this)() / rhs;
            return (*this)();
        }

        inline X& operator/=(const value_type& rhs) {
            (*this)() = (*this)() / X(rhs);
            return (*this)();
        }

        // Increment operators

        inline X operator++(int) {
            X tmp = (*this)();
            (*this) += value_type(1);
            return tmp;
        }

        inline X& operator++() {
            (*this)() += value_type(1);
            return (*this)();
        }

        inline X operator--(int) {
            X tmp = (*this)();
            (*this) -= value_type(1);
            return tmp;
        }

        inline X& operator--() {
            (*this)() -= value_type(1);
            return (*this)();
        }


    protected:

        // Ensure only inheriting classes can instantiate / copy / assign simd_vector.
        // Avoids incomplete copy / assignment from client code.

        inline simd_vector() {
        }

        inline ~simd_vector() {
        }

        inline simd_vector(const simd_vector&) {
        }

        inline simd_vector& operator=(const simd_vector&) {
            return *this;
        }
    };

    template <class X>
    inline const simd_vector<X> operator+(const simd_vector<X>& lhs,
            const typename simd_vector_traits<X>::type& rhs) {
        return lhs() + X(rhs);
    }

    template <class X>
    inline const simd_vector<X> operator+(const typename simd_vector_traits<X>::type& lhs,
            const simd_vector<X>& rhs) {
        return X(lhs) + rhs();
    }

    template <class X>
    inline const simd_vector<X> operator-(const simd_vector<X>& lhs,
            const typename simd_vector_traits<X>::type& rhs) {
        return lhs() - X(rhs);
    }

    template <class X>
    inline const simd_vector<X> operator-(const typename simd_vector_traits<X>::type& lhs,
            const simd_vector<X>& rhs) {
        return X(lhs) - rhs();
    }

    template <class X>
    inline const simd_vector<X> operator*(const simd_vector<X>& lhs,
            const typename simd_vector_traits<X>::type& rhs) {
        return lhs() * X(rhs);
    }

    template <class X>
    inline const simd_vector<X> operator*(const typename simd_vector_traits<X>::type& lhs,
            const simd_vector<X>& rhs) {
        return X(lhs) * rhs();
    }

    template <class X>
    inline const simd_vector<X> operator/(const simd_vector<X>& lhs,
            const typename simd_vector_traits<X>::type& rhs) {
        return lhs() / X(rhs);
    }

    template <class X>
    inline const simd_vector<X> operator/(const typename simd_vector_traits<X>::type& lhs,
            const simd_vector<X>& rhs) {
        return X(lhs) / rhs();
    }

    class vector4f : public simd_vector<vector4f> {
    public:

        inline vector4f() {
        }

        inline vector4f(float f) : m_value(_mm_set1_ps(f)) {
        }

        inline vector4f(float f0, float f1, float f2, float f3) : m_value(_mm_setr_ps(f0, f1, f2, f3)) {
        }

        inline vector4f(const __m128& rhs) : m_value(rhs) {
        }

        inline vector4f& operator=(const __m128& rhs) {
            m_value = rhs;
            return *this;
        }

        inline operator __m128() const {
            return m_value;
        }

        inline vector4f& load_a(const float* src) {
            m_value = _mm_load_ps(src);
            return *this;
        }

        inline vector4f& load_u(const float* src) {
            m_value = _mm_loadu_ps(src);
            return *this;
        }

        inline void store_a(float* dst) const {
            _mm_store_ps(dst, m_value);
        }

        inline void store_u(float* dst) const {
            _mm_storeu_ps(dst, m_value);
        }

    private:

        __m128 m_value;
    };

    inline const vector4f operator+(const vector4f& lhs, const vector4f& rhs) {
        return _mm_add_ps(lhs, rhs);
    }

    inline const vector4f operator-(const vector4f& lhs, const vector4f& rhs) {
        return _mm_sub_ps(lhs, rhs);
    }

    inline const vector4f operator*(const vector4f& lhs, const vector4f& rhs) {
        return _mm_mul_ps(lhs, rhs);
    }

    inline const vector4f operator/(const vector4f& lhs, const vector4f& rhs) {
        return _mm_div_ps(lhs, rhs);
    }

    std::ostream& operator<<(std::ostream& out, vector4f& v) {
        float result [4];
        _mm_store_ps(result, v);
        out << "{" << result[0] << "," << result[1] << "," << result[2] << "," << result[3] << "}";
        return out;
    }

    class vector2d : public simd_vector<vector2d> {
    public:

        inline vector2d() {
        }

        inline vector2d(double f) : m_value(_mm_set1_pd(f)) {
        }

        inline vector2d(double f0, double f1) : m_value(_mm_setr_pd(f0, f1)) {
        }

        inline vector2d(const __m128& rhs) : m_value(rhs) {
        }

        inline vector2d& operator=(const __m128& rhs) {
            m_value = rhs;
            return *this;
        }

        inline operator __m128() const {
            return m_value;
        }

        inline vector2d& load_a(const double* src) {
            m_value = _mm_load_pd(src);
            return *this;
        }

        inline vector2d& load_u(const double* src) {
            m_value = _mm_loadu_pd(src);
            return *this;
        }

        inline void store_a(double* dst) const {
            _mm_store_pd(dst, m_value);
        }

        inline void store_u(double* dst) const {
            _mm_storeu_pd(dst, m_value);
        }

    private:

        __m128d m_value;
    };

    inline const vector2d operator+(const vector2d& lhs, const vector2d& rhs) {
        return _mm_add_pd(lhs, rhs);
    }

    inline const vector2d operator-(const vector2d& lhs, const vector2d& rhs) {
        return _mm_sub_pd(lhs, rhs);
    }

    inline const vector2d operator*(const vector2d& lhs, const vector2d& rhs) {
        return _mm_mul_pd(lhs, rhs);
    }

    inline const vector2d operator/(const vector2d& lhs, const vector2d& rhs) {
        return _mm_div_pd(lhs, rhs);
    }

    std::ostream& operator<<(std::ostream& out, vector2d& v) {
        double result [2];
        _mm_store_pd(result, v);
        out << "{" << result[0] << "," << result[1] << "}";
        return out;
    }
}

#endif	/* SIMD_HPP */

