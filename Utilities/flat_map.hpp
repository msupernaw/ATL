/* 
 * File:   flat_map.hpp
 * Author: matthewsupernaw
 *
 * Created on June 27, 2015, 3:21 PM
 */

#ifndef FLAT_MAP_HPP
#define FLAT_MAP_HPP

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <iterator>

/**
 * Port of eastl lower_bound
 * License: https://github.com/electronicarts/EASTL/blob/master/LICENSE
 * @param first
 * @param last
 * @param value
 * @param compare
 * @return 
 */
template <typename ForwardIterator, typename T, typename Compare >
inline ForwardIterator
eastl_lower_bound(ForwardIterator first, ForwardIterator last, const T& value, const Compare& compare) {
    typedef typename std::iterator_traits<ForwardIterator>::difference_type DifferenceType;

    DifferenceType d = std::distance(first, last); // This will be efficient for a random access iterator such as an array.

    while (d > 0) {
        ForwardIterator i = first;
        DifferenceType d2 = d >> 1; // We use '>>1' here instead of '/2' because MSVC++ for some reason generates significantly worse code for '/2'. Go figure.

        std::advance(i, d2); // This will be efficient for a random access iterator such as an array.

        if (compare(*i, value)) {
            // Disabled because std::lower_bound doesn't specify (23.3.3.3, p3) this can be done: EASTL_VALIDATE_COMPARE(!(value < *i)); // Validate that the compare function is sane.
            first = ++i;
            d -= d2 + 1;
        } else
            d = d2;
    }
    return first;
}

template<class FwdIt, class T, class P>
inline FwdIt branchless_lower_bound(FwdIt begin, FwdIt end, T const &val, P pred) {
    while (begin != end) {
        FwdIt middle(begin);
        std::advance(middle, std::distance(begin, end) >> 1);
        FwdIt middle_plus_one(middle);
        ++middle_plus_one;
        bool const b = pred(*middle, val);
        begin = b ? middle_plus_one : begin;
        end = b ? end : middle;
    }
    return begin;
}

template<class Key, class T>
class flat_map {
public:

    typedef std::pair<Key, T> value_type;
    typedef Key key_type;
    typedef T mapped_type;

private:
    std::vector<value_type > data_m;

    struct compare_elems {

        inline const bool operator()(const value_type& x, const value_type& y) const {
            return x.first < y.first;
        }

        inline const bool operator()(const value_type& x, const key_type& y) const {
            return x.first < y;
        }

        inline const bool operator()(const key_type& x, const value_type& y) const {
            return x < y.first;
        }
    };

public:
    typedef typename std::vector<value_type>::iterator iterator;
    typedef typename std::vector<value_type>::const_iterator const_iterator;
    typedef typename std::vector<value_type>::reverse_iterator reverse_iterator;
    typedef typename std::vector<value_type>::const_reverse_iterator const_reverse_iterator;

    flat_map(std::size_t n = 0) {
        data_m.reserve(n);
    }

    inline std::pair<iterator, bool> insert(const value_type& value) {
        iterator it = eastl_lower_bound(data_m.begin(), data_m.end(), value.first, compare_elems());
        if (it == data_m.end() || it->first != value.first) {
            iterator it2 = data_m.insert(it, value);
            return std::make_pair(it2, true);
        } else {
            return std::make_pair(it, false);
        }
    }

    inline T& operator[](const Key& key) {
        iterator it = eastl_lower_bound(data_m.begin(), data_m.end(), key, compare_elems());
        if (it == data_m.end() || it->first != key) {
            iterator it2 = data_m.insert(it, value_type(key, T()));
            return it2->second;
        } else {
            return it->second;
        }
    }

    void swap(flat_map& m) {
        data_m.swap(m.data_m);
    }

    iterator find(const Key& key) {
        iterator it = eastl_lower_bound(data_m.begin(), data_m.end(), key, compare_elems());
        if (it == data_m.end() || it->first != key) {
            return data_m.end();
        } else {
            return it;
        }
    }

    const_iterator find(const Key& key) const {
        const_iterator it = eastl_lower_bound(data_m.begin(), data_m.end(), key, compare_elems());
        if (it == data_m.end() || it->first != key) {
            return data_m.end();
        } else {
            return it;
        }
    }

    void erase(const Key& key) {
        iterator it = eastl_lower_bound(data_m.begin(), data_m.end(), key, compare_elems());
        if (it != data_m.end() && it->first == key) {
            data_m.erase(it);
        }
    }

    void clear() {
        data_m.clear();
    }

    void reset() {
        data_m.resize(0);
    }

    bool empty() const {
        return data_m.empty();
    }

    std::size_t size() const {
        return data_m.size();
    }

    iterator begin() {
        return data_m.begin();
    }

    iterator end() {
        return data_m.end();
    }

    const_iterator begin() const {
        return data_m.begin();
    }

    const_iterator end() const {
        return data_m.end();
    }

    reverse_iterator rbegin() {
        return data_m.rbegin();
    }

    reverse_iterator rend() {
        return data_m.rend();
    }

    const_reverse_iterator rbegin() const {
        return data_m.rbegin();
    }

    const_reverse_iterator rend() const {
        return data_m.rend();
    }

    std::vector<value_type >& data() {
        return data_m;
    }

    const std::vector<value_type >& data()const {
        return data_m;
    }
};



#endif /* FLAT_MAP_HPP */

