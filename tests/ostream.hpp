#ifndef INCLUDE_GUARD_C9E151ED_8079_433A_8F33_BF79B59D6AC8
#define INCLUDE_GUARD_C9E151ED_8079_433A_8F33_BF79B59D6AC8

#include "ez/direct_iterator.hpp"
#include "ez/interval.hpp"

#include <forward_list>
#include <iostream>
#include <list>
#include <vector>

namespace std {

inline ostream& operator<<(ostream& stream, vector<int>::iterator it) {
	return stream << reinterpret_cast<intptr_t>(&*it);
}

inline ostream& operator<<(ostream& stream, list<int>::iterator it) {
	return stream << reinterpret_cast<intptr_t>(&*it);
}

inline ostream& operator<<(ostream& stream, list<int>::const_iterator it) {
	return stream << reinterpret_cast<intptr_t>(&*it);
}

inline ostream& operator<<(ostream& stream, forward_list<int>::iterator it) {
	return stream << reinterpret_cast<intptr_t>(&*it);
}

template <typename T>
ostream& operator<<(ostream& stream, ez::direct_iterator<T> it) {
	return stream << *it;
}

}


#endif
