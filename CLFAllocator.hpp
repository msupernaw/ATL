/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CLFAllocator.hpp
 * Author: matthewsupernaw
 *
 * Created on August 24, 2016, 9:39 AM
 */

#ifndef CLFALLOCATOR_HPP
#define CLFALLOCATOR_HPP


#include <stdlib.h>
#include <new>

#include <limits>

#include "Platform.hpp"

#ifndef ATL_WINDOWS
#include "clfmalloc.h"
#endif


namespace atl {
template <class T> 
struct clfallocator {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U> struct rebind { typedef clfallocator<U> other; };
    clfallocator() throw() {}
    clfallocator(const clfallocator&) throw() {}

    template <class U> clfallocator(const clfallocator<U>&) throw(){}

    ~clfallocator() throw() {}

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type s, void const * = 0) {
        if (0 == s)
            return NULL;
        pointer temp = (pointer)malloc(s * sizeof(T)); 
        if (temp == NULL)
            throw std::bad_alloc();
        return temp;
    }

    void deallocate(pointer p, size_type) {
        free(p);
    }

    size_type max_size() const throw() { 
        return std::numeric_limits<size_t>::max() / sizeof(T); 
    }

    void construct(pointer p, const T& val) {
        new((void *)p) T(val);
    }

    void destroy(pointer p) {
        p->~T();
    }
};
}


#endif /* CLFALLOCATOR_HPP */

