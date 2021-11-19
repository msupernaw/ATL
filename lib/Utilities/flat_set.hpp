
#ifndef FLAT_SET_HPP
#define FLAT_SET_HPP

#include <vector>
#include <algorithm>
#include "aligned_allocator.hpp"
//#include "../AutoDiff/AlignedAllocator.hpp"

template<typename ForwardIterator, typename T, class P>
inline ForwardIterator fs_eastl_lower_bound(ForwardIterator first,
		ForwardIterator last, const T &value, P pred) {
	typedef typename std::iterator_traits<ForwardIterator>::difference_type DifferenceType;

	DifferenceType d = std::distance(first, last); // This will be efficient for a random access iterator such as an array.

	while (d > 0) {
		ForwardIterator i = first;
		DifferenceType d2 = d >> 1; // We use '>>1' here instead of '/2' because MSVC++ for some reason generates significantly worse code for '/2'. Go figure.

		std::advance(i, d2); // This will be efficient for a random access iterator such as an array.

		if (pred(*i, value)) {
			// Disabled because std::lower_bound doesn't specify (23.3.3.3, p3) this can be done: EASTL_VALIDATE_COMPARE(!(value < *i)); // Validate that the compare function is sane.
			first = ++i;
			d -= d2 + 1;
		} else
			d = d2;
	}
	return first;
}

template<class FwdIt, class T, class P>
inline FwdIt fs_branchless_lower_bound(FwdIt begin, FwdIt end, T const &val,
		P pred) {
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

template<class T, class Compare = std::less<T>,
		class Allocator = aligned_allocator<T> >
class flat_set {
	std::vector<T, Allocator> data_m;
	Compare cmp;
public:
	typedef typename std::vector<T, Allocator>::iterator iterator;
	typedef typename std::vector<T, Allocator>::const_iterator const_iterator;
	typedef typename std::vector<T, Allocator>::reverse_iterator reverse_iterator;
	typedef typename std::vector<T, Allocator>::const_reverse_iterator const_reverse_iterator;

	flat_set(const Compare &c = Compare()) :
			data_m(), cmp(c) {
//                                data_m.reserve(8);
	}

	template<class InputIterator>
	flat_set(InputIterator first, InputIterator last, const Compare &c =
			Compare()) :
			data_m(first, last), cmp(c) {
		std::sort(begin(), end(), cmp);
	}

	inline iterator erase(const T &t) {
		iterator i = this->find(t);
		if (i != this->end()) {
			return data_m.erase(i);
		} else {
			return this->end();
		}
	}

	inline iterator insert(const T &t) {
		iterator i = fs_eastl_lower_bound(begin(), end(), t, cmp);
		if (i == end() || cmp(t, *i))
			data_m.insert(i, std::move(t));
		return i;
	}

	inline void insert(const iterator &first, const iterator &last) {
		iterator it;
		for (it = first; it != last; ++it) {
			this->insert((*it));
		}

	}

	inline const_iterator find(const T &t) const {
		const_iterator i = fs_eastl_lower_bound(begin(), end(), t, cmp);
		return i == end() || cmp(t, *i) ? end() : i;
	}

	inline iterator find(const T &t) {
		iterator i = fs_eastl_lower_bound(begin(), end(), t, cmp);
		return i == end() || cmp(t, *i) ? end() : i;
	}

	inline iterator begin() {
		return data_m.begin();
	}

	inline iterator end() {
		return data_m.end();
	}

	inline const_iterator begin() const {
		return data_m.begin();
	}

	inline const_iterator end() const {
		return data_m.end();
	}

	inline reverse_iterator rbegin() {
		return data_m.rbegin();
	}

	inline reverse_iterator rend() {
		return data_m.rend();
	}

	inline const_reverse_iterator rbegin() const {
		return data_m.rbegin();
	}

	inline const_reverse_iterator rend() const {
		return data_m.rend();
	}

	inline size_t size() const {
		return data_m.size();
	}

	inline void reserve(size_t size) {
		data_m.reserve(size);
	}

	inline std::vector<T, Allocator>& data() {
		return data_m;
	}

	inline bool contains(const T &t) {
		return std::binary_search(this->data_m.begin(), this->data_m.end(), t);
	}

	inline void clear() {
		data_m.clear();
	}

	void clear_no_resize() {
		data_m.resize(0);
	}

};

template<typename T>
class levelorder_vector {
	typedef std::vector<T> vector;

public:
	typedef typename vector::value_type value_type;
	typedef typename vector::reference reference;
	typedef typename vector::const_reference const_reference;
	typedef typename vector::const_iterator iterator;
	typedef typename vector::const_iterator const_iterator;
	typedef typename vector::difference_type difference_type;
	typedef typename vector::size_type size_type;

	levelorder_vector() {
	}

	levelorder_vector(const levelorder_vector &x) :
			impl(x.impl) {
	}

	levelorder_vector& operator=(const levelorder_vector &x) {
		impl = x.impl;
		return *this;
	}

	template<typename InputIterator>
	levelorder_vector(InputIterator first, InputIterator last) {
		vector aux(first, last);
		std::sort(aux.begin(), aux.end());
		impl = aux;
		insert(0, aux.size(), aux.begin());
	}

	const_iterator begin() const {
		return impl.begin();
	}

	const_iterator end() const {
		return impl.end();
	}

	const_iterator cbegin() const {
		return impl.cbegin();
	}

	const_iterator cend() const {
		return impl.cend();
	}

	friend bool operator==(const levelorder_vector &x,
			const levelorder_vector &y) {
		return x.impl == y.impl;
	}

	friend bool operator!=(const levelorder_vector &x,
			const levelorder_vector &y) {
		return x.impl != y.impl;
	}

	void swap(levelorder_vector &x) {
		impl.swap(x.impl);
	}

	friend void swap(levelorder_vector &x, levelorder_vector &y) {
		x.swap(y);
	}

	size_type size() const {
		return impl.size();
	}

	size_type max_size() const {
		return impl.max_size();
	}

	bool empty() const {
		return impl.empty();
	}

	const_iterator lower_bound(const T &x) const {
		size_type n = impl.size(), i = n, j = 0;
		while (j < n) {
			if (impl[j] < x) {
				j = 2 * j + 2;
			} else {
				i = j;
				j = 2 * j + 1;
			}
		}
		return begin() + i;
	}

	void build() {
		std::sort(impl.begin(), impl.end());
		//        impl = aux_;
		insert(0, impl.size(), impl.begin());
	}

	void push(const T &t) {
		impl.push_back(t);
	}

	void clear() {
		impl.clear();
	}

	void clear_no_resize() {
		impl.resize(0);
	}

private:

	void insert(size_type i, size_type n, const_iterator first) {
		if (n) {
			size_type h = root(n);
			impl[i] = *(first + h);
			insert(2 * i + 1, h, first);
			insert(2 * i + 2, n - h - 1, first + h + 1);
		}
	}

	size_type root(size_type n) {
		if (n <= 1)
			return 0;
		size_type i = 2;
		while (i <= n)
			i *= 2;
		return std::min(i / 2 - 1, n - i / 4);
	}

	vector impl;

};

#endif /* FLAT_SET_HPP */

