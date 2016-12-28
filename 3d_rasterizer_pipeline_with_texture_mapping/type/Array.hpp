/* Start Header -------------------------------------------------------
File Name: Array.hpp
Purpose: Contains definition for Array
Language: C++, VC++11 (cl.exe)
Platform: Windows, Visual Studio 2012
Project: jimi.e_CS250_2
Author: Jimi Huard, jimi.e 180003312
Creation date: 9/20/2013
- End Header --------------------------------------------------------*/
#pragma once

#include "Type.hpp"

template < typename T >
class Array {
	enum { FACTOR = 2 };

	T *_data ;
	uint32 _size ;
	uint32 _cap ;

	void Realloc() {
		if (!_cap)
			_cap = FACTOR ;
		else _cap = _cap * FACTOR ;

		T *temp = _data ;
		_data = new T[_cap] ;
		for (uint32 i = 0; i < _size; ++i)
			_data[i] = temp[i] ;
		delete []temp ;
	}

public:
	typedef T* iterator ;
	typedef T const *const_iterator ;

	Array() : _data(nullptr), _size(0), _cap(0) {}
	~Array() { Cleanup(); }

	Array(const Array<T> &other) {
		_size = other._size ;
		_cap = other._cap ;
		_data = new T[_cap] ;
		for (uint32 i = 0; i < _size; ++i)
			_data[i] = other._data[i] ;
	}

	Array<T> &operator=(const Array<T> &other) {
		if (this != &other) {
			Cleanup() ;
			_size = other._size ;
			_cap = other._cap ;
			_data = new T[_cap] ;
			for (uint32 i = 0; i < _size; ++i)
				_data[i] = other._data[i] ;
		}
		return *this ;
	}

	T *Data() const {
		return _data ;
	}

	void Cleanup() {
		_size = 0U ;
		_cap = 0U ;
		delete []_data ;
	}

	void push_back(const T &t) {
		if (_size + 1 >= _cap)
			Realloc() ;
		_data[_size] = t ;
		_size ++ ;
	}

	void pop_back() {
		if (_size)
			_size -- ;
	}

	void erase(iterator pos) {
		if (pos == end()) return ;
		for (iterator n = pos + 1; n != end(); ++n) {
			*pos = *n ;
			pos = n ;
		}
		--_size ;
	}

	T &operator[](uint32 ind) {
		return _data[ind] ;
	}

	T const &operator[](uint32 ind) const {
		return _data[ind] ;
	}

	uint32 Size() const {
		return _size ;
	}

	uint32 Capacity() const {
		return _cap ;
	}

	void Clear() {
		_size = 0 ;
	}

	iterator at(uint32 index) {
		return _data + index ;
	}
	
	iterator begin() {
		return _data ;
	}

	iterator end() {
		return _data + _size ;
	}

	const_iterator begin() const {
		return _data ;
	}

	const_iterator end() const {
		return _data + _size ;
	}
};

template < typename T1, typename T2 >
struct Pair {
	T1 first ;
	T2 second ;

	Pair() : first(), second() {}

	Pair(T1 f, T2 s) 
		: first(f), second(s) {}
}; 

template < typename T1, typename T2 >
Pair<T1, T2> MakePair(T1 a, T2 b) {
	return Pair<T1, T2>(a, b) ;
}