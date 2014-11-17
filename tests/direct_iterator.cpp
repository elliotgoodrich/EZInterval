#include "ez/direct_iterator.hpp"

#include <boost/test/unit_test.hpp>

#include <forward_list>
#include <iterator>
#include <iostream>
#include <list>
#include <type_traits>
#include <vector>

template <typename T>
using direct_category = typename ez::direct_iterator<T>::iterator_category;

template <typename T>
using difference_type = typename ez::direct_iterator<T>::difference_type;

namespace std {

std::ostream& operator<<(std::ostream& stream, std::vector<int>::iterator it) {
	return stream << reinterpret_cast<intptr_t>(&*it);
}

std::ostream& operator<<(std::ostream& stream, std::list<int>::iterator it) {
	return stream << reinterpret_cast<intptr_t>(&*it);
}

std::ostream& operator<<(std::ostream& stream, std::forward_list<int>::iterator it) {
	return stream << reinterpret_cast<intptr_t>(&*it);
}

}

BOOST_AUTO_TEST_SUITE(direct_iterator)

BOOST_AUTO_TEST_CASE(TypeChecks) {
	// int
	static_assert(std::is_same<std::random_access_iterator_tag,  direct_category<int>>::value, "");
	static_assert(!std::is_same<std::bidirectional_iterator_tag, direct_category<int>>::value, "");
	static_assert(!std::is_same<std::forward_iterator_tag,       direct_category<int>>::value, "");
	static_assert(!std::is_same<std::input_iterator_tag,         direct_category<int>>::value, "");
	static_assert(std::is_same<std::ptrdiff_t, difference_type<int>>::value, "");

	// unsigned char
	static_assert(std::is_same<std::random_access_iterator_tag,  direct_category<unsigned char>>::value, "");
	static_assert(!std::is_same<std::bidirectional_iterator_tag, direct_category<unsigned char>>::value, "");
	static_assert(!std::is_same<std::forward_iterator_tag,       direct_category<unsigned char>>::value, "");
	static_assert(!std::is_same<std::input_iterator_tag,         direct_category<unsigned char>>::value, "");
	static_assert(std::is_same<std::ptrdiff_t, difference_type<unsigned char>>::value, "");

	// unsigned long
	static_assert(std::is_same<std::random_access_iterator_tag,  direct_category<unsigned long>>::value, "");
	static_assert(!std::is_same<std::bidirectional_iterator_tag, direct_category<unsigned long>>::value, "");
	static_assert(!std::is_same<std::forward_iterator_tag,       direct_category<unsigned long>>::value, "");
	static_assert(!std::is_same<std::input_iterator_tag,         direct_category<unsigned long>>::value, "");
	static_assert(std::is_same<std::ptrdiff_t, difference_type<unsigned long>>::value, "");

	// vector iterator
	typedef direct_category<typename std::vector<int>::iterator> vector_it;
	static_assert(std::is_same<std::random_access_iterator_tag,  vector_it>::value, "");
	static_assert(!std::is_same<std::bidirectional_iterator_tag, vector_it>::value, "");
	static_assert(!std::is_same<std::forward_iterator_tag,       vector_it>::value, "");
	static_assert(!std::is_same<std::input_iterator_tag,         vector_it>::value, "");
	static_assert(std::is_same<std::ptrdiff_t, difference_type<typename std::vector<int>::iterator>>::value, "");

	// list iterator
	typedef direct_category<typename std::list<int>::iterator> list_it;
	static_assert(!std::is_same<std::random_access_iterator_tag, list_it>::value, "");
	static_assert(std::is_same<std::bidirectional_iterator_tag,  list_it>::value, "");
	static_assert(!std::is_same<std::forward_iterator_tag,       list_it>::value, "");
	static_assert(!std::is_same<std::input_iterator_tag,         list_it>::value, "");
	static_assert(std::is_same<std::ptrdiff_t, difference_type<typename std::list<int>::iterator>>::value, "");

	// forward list iterator
	typedef direct_category<typename std::forward_list<int>::iterator> forward_list_it;
	static_assert(!std::is_same<std::random_access_iterator_tag, forward_list_it>::value, "");
	static_assert(!std::is_same<std::bidirectional_iterator_tag, forward_list_it>::value, "");
	static_assert(std::is_same<std::forward_iterator_tag,        forward_list_it>::value, "");
	static_assert(!std::is_same<std::input_iterator_tag,         forward_list_it>::value, "");
	static_assert(std::is_same<std::ptrdiff_t, difference_type<typename std::forward_list<int>::iterator>>::value, "");

	// istream iterator
	typedef direct_category<typename std::istream_iterator<int>::iterator> istream_it;
	static_assert(!std::is_same<std::random_access_iterator_tag, istream_it>::value, "");
	static_assert(!std::is_same<std::bidirectional_iterator_tag, istream_it>::value, "");
	static_assert(!std::is_same<std::forward_iterator_tag,       istream_it>::value, "");
	static_assert(std::is_same<std::input_iterator_tag,          istream_it>::value, "");
	static_assert(std::is_same<std::ptrdiff_t, difference_type<typename std::istream_iterator<int>::iterator>>::value, "");

	struct A {
		typedef std::random_access_iterator_tag iterator_category;
		typedef int difference_type;
	};

	static_assert(std::is_same<std::random_access_iterator_tag,  direct_category<A>>::value, "");
	static_assert(!std::is_same<std::bidirectional_iterator_tag, direct_category<A>>::value, "");
	static_assert(!std::is_same<std::forward_iterator_tag,       direct_category<A>>::value, "");
	static_assert(!std::is_same<std::input_iterator_tag,         direct_category<A>>::value, "");
	static_assert(std::is_same<int, difference_type<A>>::value, "");
}


template <typename T>
struct iterator_test;

#define EZ_INPUT_ITERATOR_TEST(EZ_TYPE, EZ_NAME) \
BOOST_AUTO_TEST_CASE(EZ_NAME ## InputIteratorTest) { \
	static_assert(std::is_move_constructible<ez::direct_iterator<EZ_TYPE>>::value, ""); \
	static_assert(std::is_copy_constructible<ez::direct_iterator<EZ_TYPE>>::value, ""); \
	static_assert(std::is_move_assignable<ez::direct_iterator<EZ_TYPE>>::value, ""); \
	static_assert(std::is_copy_assignable<ez::direct_iterator<EZ_TYPE>>::value, ""); \
	static_assert(std::is_destructible<ez::direct_iterator<EZ_TYPE>>::value, ""); \
 \
	iterator_test<EZ_TYPE> s; \
	EZ_TYPE value = s.starting_value(); \
	auto i = ez::make_direct_iterator(value); \
 \
	BOOST_CHECK_EQUAL(*i, value); \
 \
	auto j = i; \
	BOOST_CHECK_EQUAL(*i, *j); \
	BOOST_CHECK_EQUAL(i, j); \
 \
	BOOST_CHECK_EQUAL(++i, ++j); \
	BOOST_CHECK_EQUAL(*i, *j); \
 \
	BOOST_CHECK_EQUAL(i++, j++); \
	BOOST_CHECK_EQUAL(*i, *j); \
 \
	BOOST_CHECK_EQUAL(i++, j); \
	BOOST_CHECK(i != j); \
 \
	++j; \
	BOOST_CHECK(++i != j); \
	BOOST_CHECK(i != j); \
}

#define EZ_FORWARD_ITERATOR_TEST(EZ_TYPE, EZ_NAME) \
BOOST_AUTO_TEST_CASE(EZ_NAME ## ForwardIteratorTest) { \
	static_assert(std::is_default_constructible<ez::direct_iterator<EZ_TYPE>>::value, ""); \
\
	ez::direct_iterator<EZ_TYPE> i, j; \
	BOOST_CHECK_EQUAL(i, j); \
}

#define EZ_BIDIRECTIONAL_ITERATOR_TEST(EZ_TYPE, EZ_NAME) \
BOOST_AUTO_TEST_CASE(EZ_NAME ## BidirectionalIteratorTest) { \
	iterator_test<EZ_TYPE> s; \
	auto i = ez::make_direct_iterator(s.starting_value()); \
 \
	auto j = i; \
 \
	std::advance(i, 3); \
	std::advance(j, 3); \
 \
	BOOST_CHECK_EQUAL(*i, *j); \
	BOOST_CHECK_EQUAL(i, j); \
 \
	BOOST_CHECK_EQUAL(--i, --j); \
	BOOST_CHECK_EQUAL(*i, *j); \
 \
	BOOST_CHECK_EQUAL(i--, j--); \
	BOOST_CHECK_EQUAL(*i, *j); \
 \
	BOOST_CHECK_EQUAL(i--, j); \
	BOOST_CHECK(i != j); \
}

#define EZ_RANDOM_ACCESS_ITERATOR_TEST(EZ_TYPE, EZ_NAME) \
BOOST_AUTO_TEST_CASE(EZ_NAME ## RandomAccessIteratorTest) { \
	iterator_test<EZ_TYPE> s; \
	const auto i = ez::make_direct_iterator(s.starting_value()); \
	auto j = i; \
	auto k = i; \
 \
	++k, ++k; \
	BOOST_CHECK_EQUAL(j += 2, k); \
	BOOST_CHECK_EQUAL(j, k); \
	BOOST_CHECK_EQUAL(i + 2, j); \
	BOOST_CHECK_EQUAL(2 + i, j); \
	BOOST_CHECK_EQUAL(j - i, 2); \
 \
	--k; \
	BOOST_CHECK_EQUAL(j -= 1, k); \
	BOOST_CHECK_EQUAL(j, k); \
	BOOST_CHECK_EQUAL(j - 1, i); \
	BOOST_CHECK_EQUAL(i - j, -1); \
 \
	BOOST_CHECK_EQUAL(i + 3, i[3]); \
 \
	auto m = i + 1; \
	BOOST_CHECK(i < m); \
	BOOST_CHECK(i <= m); \
	BOOST_CHECK(m > i); \
	BOOST_CHECK(m >= i); \
 \
	BOOST_CHECK(i <= i); \
	BOOST_CHECK(i >= i); \
	BOOST_CHECK(!(i < i)); \
	BOOST_CHECK(!(i < i)); \
}

template <>
struct iterator_test<int> {
	int starting_value() const {
		return 0;
	}
};

EZ_INPUT_ITERATOR_TEST(int, Int)
EZ_FORWARD_ITERATOR_TEST(int, Int)
EZ_BIDIRECTIONAL_ITERATOR_TEST(int, Int)
EZ_RANDOM_ACCESS_ITERATOR_TEST(int, Int)

template <>
struct iterator_test<double> {
	double starting_value() const {
		return 0.0;
	}
};

EZ_INPUT_ITERATOR_TEST(double, Double)
EZ_FORWARD_ITERATOR_TEST(double, Double)
EZ_BIDIRECTIONAL_ITERATOR_TEST(double, Double)
EZ_RANDOM_ACCESS_ITERATOR_TEST(double, Double)

template <>
struct iterator_test<std::vector<int>::iterator> {
	iterator_test() {
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);
	}

	std::vector<int>::iterator starting_value() {
		return v.begin();
	}

	std::vector<int> v;
};

EZ_INPUT_ITERATOR_TEST(std::vector<int>::iterator, Vector)
EZ_FORWARD_ITERATOR_TEST(std::vector<int>::iterator, Vector)
EZ_BIDIRECTIONAL_ITERATOR_TEST(std::vector<int>::iterator, Vector)
EZ_RANDOM_ACCESS_ITERATOR_TEST(std::vector<int>::iterator, Vector)

template <>
struct iterator_test<std::list<int>::iterator> {
	iterator_test() {
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);
	}

	std::list<int>::iterator starting_value() {
		return v.begin();
	}

	std::list<int> v;
};

EZ_INPUT_ITERATOR_TEST(std::list<int>::iterator, List)
EZ_FORWARD_ITERATOR_TEST(std::list<int>::iterator, List)
EZ_BIDIRECTIONAL_ITERATOR_TEST(std::list<int>::iterator, List)

template <>
struct iterator_test<std::forward_list<int>::iterator> {
	iterator_test() {
		v.push_front(1);
		v.push_front(2);
		v.push_front(3);
		v.push_front(4);
		v.push_front(5);
	}

	std::forward_list<int>::iterator starting_value() {
		return v.begin();
	}

	std::forward_list<int> v;
};

EZ_INPUT_ITERATOR_TEST(std::forward_list<int>::iterator, ForwardList)
EZ_FORWARD_ITERATOR_TEST(std::forward_list<int>::iterator, ForwardList)

BOOST_AUTO_TEST_SUITE_END()
