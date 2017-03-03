// -*- C++ -*-
/* 
 * File:   compressed_vector.hpp
 * Author: matthewsupernaw
 *
 * Created on November 24, 2015, 1:40 PM
 */

#ifndef COMPRESSED_VECTOR_HPP
#define	COMPRESSED_VECTOR_HPP

#include <vector>
#include <algorithm>
#include <utility>
#include "flat_map.hpp"

namespace util {

    template<typename T>
    class compressed_vector {
        std::vector<std::pair<size_t, T> > data_m;
        size_t size_m;
        size_t allocated_size_m;
        size_t max_held;

        inline const size_t lower_bound(const size_t& index) {
            size_t start = 0;
            size_t end = size_m;
            size_t guess;
            typename std::vector<std::pair<size_t, T> >::iterator it;
            while (end > start) {
                guess = (end + start) >> 1;
                if (data_m[guess].first < index) {
                    start = guess + 1;
                } else {
                    end = guess;
                }
            }
            return (start);
        }

        T value(const size_t& index) {
         
            size_t id = lower_bound(index);
            if (id >= size_m || data_m[id].first != index) {
                return T();
            }

            return data_m[id].second;
        }

        T& ref(size_t index) {
            size_t id = lower_bound(index);
            if (id >= size_m || data_m[id].first != index) {
                resize(size_m + 1, 1);
                for (size_t j = size_m - 1; j > id; --j) {
                    data_m[j] = data_m[j - 1];
                }
                data_m[id] = std::pair<size_t, T>(index, T());
            }
            return data_m[id].second;
        }

        inline void reallocate(size_t size) {
            data_m.resize(size);
            allocated_size_m = size;
        }

    public:
        typedef typename std::vector<std::pair<size_t, T> >::iterator iterator;
        typedef typename std::vector<std::pair<size_t, T> >::const_iterator const_iterator;
        typedef typename std::vector<std::pair<size_t, T> >::reverse_iterator reverse_iterator;
        typedef typename std::vector<std::pair<size_t, T> >::const_reverse_iterator const_reverse_iterator;

        compressed_vector(size_t size = 0) : size_m(0), allocated_size_m(0), max_held(0) {
//                        this->reserve(32);
            if (size)
                this->resize(size);
        }

        /**
         * Returns a reference to a stored variable at given index.
         * If the entry doesn't exist, it is created.
         * 
         * @param index
         * @return 
         */
        inline T& operator[](size_t index) {
            return ref(index);
        }

        /**
         * Returns a reference to a stored variable at given index.
         * If the entry doesn't exist, it is created.
         * 
         * @param index
         * @return 
         */
        inline T& at(size_t index) {
            return ref(index);
        }

        /**
         * Returns a value to a stored variable at given index.
         * If the entry doesn't exist, the types default value is returned and 
         * no new entries are stored.
         * 
         * @param index
         * @return 
         */
        inline T get(size_t index) {
            return value(index);
        }

        /**
         * Returns a value to a stored variable at given index.
         * If the entry doesn't exist, the types default value is returned and 
         * no new entries are stored.
         * 
         * @param index
         * @return 
         */
        inline T operator()(size_t index) {
            return value(index);
        }

        inline void clear() {
            this->allocated_size_m = 0;
            this->size_m = 0;
            this->data_m.clear();
        }

        size_t size() {
            return size_m;
        }

        void reserve(size_t size) {
            data_m.reserve(size);
        }

        void resize(size_t size, double factor = 1) {
            if (allocated_size_m < size) {
                reallocate(size + size_t(factor * double(size)));
            }
            size_m = size;
        }

        iterator find(size_t index) {
            size_t id = lower_bound(index);
            if (id >= size_m || data_m[id].first != index) {
                return end();
            }
            return data_m.begin() + id;
        }

        iterator begin() {
            return data_m.begin();
        }

        const_iterator begin() const {
            return data_m.begin();
        }

        iterator end() {
            return data_m.end();
        }

        const_iterator end() const {
            return data_m.end();
        }

        reverse_iterator rbegin() {
            return data_m.rbegin();
        }

        const_reverse_iterator rbegin() const {
            return data_m.rbegin();
        }

        reverse_iterator rend() {
            return data_m.rend();
        }

        const_reverse_iterator rend() const {
            return data_m.rend();
        }

    };
}


#endif	/* COMPRESSED_VECTOR_HPP */

