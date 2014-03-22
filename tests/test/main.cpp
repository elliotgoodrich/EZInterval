#define BOOST_TEST_MODULE utils

#include "ez/interval.hpp"

#include <boost/test/unit_test.hpp>

#include <array>
#include <sstream>

BOOST_AUTO_TEST_CASE(ConstructorAndBasicsTest) {
	auto const test_interval = ez::interval[-3](4);
	BOOST_CHECK_EQUAL(test_interval.lower(), -3);
	BOOST_CHECK_EQUAL(test_interval.upper(), 4);

	auto const bad_interval = ez::interval[6](4);
	BOOST_CHECK_EQUAL(bad_interval.lower(), 6);
	BOOST_CHECK_EQUAL(bad_interval.upper(), 4);
}

BOOST_AUTO_TEST_CASE(EmptyTest) {
	BOOST_CHECK(!ez::interval[0][0].empty());
	BOOST_CHECK(ez::interval[0](0).empty());
	BOOST_CHECK(ez::interval(0)[0].empty());
	BOOST_CHECK(ez::interval(0)(0).empty());

	BOOST_CHECK(!ez::interval[0][1].empty());
	BOOST_CHECK(!ez::interval[0](1).empty());
	BOOST_CHECK(!ez::interval(0)[1].empty());
	BOOST_CHECK(!ez::interval(0)(1).empty());

	BOOST_CHECK(ez::interval[1][0].empty());
	BOOST_CHECK(ez::interval[1](0).empty());
	BOOST_CHECK(ez::interval(1)[0].empty());
	BOOST_CHECK(ez::interval(1)(0).empty());

	BOOST_CHECK(static_cast<bool>(ez::interval[0][0]));
	BOOST_CHECK(static_cast<bool>(ez::interval[0][1]));
	BOOST_CHECK(!static_cast<bool>(ez::interval[1][0]));
}

BOOST_AUTO_TEST_CASE(EqualityTest) {
	BOOST_CHECK_EQUAL(ez::interval[0][3], ez::interval[0][3]);
	BOOST_CHECK_EQUAL(ez::interval[0.0][3.0], ez::interval[0][3]);

	BOOST_CHECK(ez::interval[0][1] != ez::interval[0](1));
	BOOST_CHECK(ez::interval[0][1] != ez::interval(0)[1]);
	BOOST_CHECK(ez::interval[0][1] != ez::interval(0)(1));
	BOOST_CHECK(ez::interval[0](1) != ez::interval(0)[1]);
	BOOST_CHECK(ez::interval[0](1) != ez::interval(0)(1));
	BOOST_CHECK(ez::interval(0)[1] != ez::interval(0)(1));

	BOOST_CHECK_EQUAL(ez::interval[1][0], ez::interval[5](-3));
}

BOOST_AUTO_TEST_CASE(TypeDefEquality) {
	auto closed = ez::closed_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(closed, ez::interval[1][3]);

	auto ropen = ez::ropen_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(ropen, ez::interval[1](3));

	auto lopen = ez::lopen_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(lopen, ez::interval(1)[3]);

	auto open = ez::open_interval<int>{1, 3};
	BOOST_CHECK_EQUAL(open, ez::interval(1)(3));
}

BOOST_AUTO_TEST_CASE(CommonPropertiesTest) {
	auto const int_interval = ez::interval(-6)[9];
	BOOST_CHECK_EQUAL(15, int_interval.diameter());
	BOOST_CHECK_EQUAL(1, int_interval.midpoint());
	BOOST_CHECK_EQUAL(7, int_interval.radius());
	BOOST_CHECK_EQUAL(7.5, int_interval.radius<double>());

	auto const double_interval = ez::interval(-6.0)[9.0];
	BOOST_CHECK_EQUAL(15.0, double_interval.diameter());
	BOOST_CHECK_EQUAL(1.5, double_interval.midpoint());
	BOOST_CHECK_EQUAL(7.5, double_interval.radius());

	auto const hybrid_interval = ez::interval(-6)[9.0];
	BOOST_CHECK_EQUAL(15.0, hybrid_interval.diameter());
	BOOST_CHECK_EQUAL(1.5, hybrid_interval.midpoint());
	BOOST_CHECK_EQUAL(7.5, hybrid_interval.radius());

	auto const bad_interval = ez::interval(1)[0];
	BOOST_CHECK_THROW(bad_interval.diameter(), ez::empty_interval);
	BOOST_CHECK_THROW(bad_interval.midpoint(), ez::empty_interval);
	BOOST_CHECK_THROW(bad_interval.radius(),   ez::empty_interval);
}

BOOST_AUTO_TEST_CASE(Intersection) {
	{
		auto const interval1 = ez::interval[1][3];
		auto const interval2 = ez::interval[0][2];
		BOOST_CHECK_EQUAL(interval1 | interval2, ez::interval[1][2]);
	}

	{
		auto const interval1 = ez::interval(1)[3];
		auto const interval2 = ez::interval(0)[2];
		BOOST_CHECK_EQUAL(interval1 | interval2, ez::interval(1)[2]);
	}

	{
		auto const interval1 = ez::interval[1](3);
		auto const interval2 = ez::interval[0](2);
		BOOST_CHECK_EQUAL(interval1 | interval2, ez::interval[1](2));
	}

	{
		auto const interval1 = ez::interval(1)(3);
		auto const interval2 = ez::interval(0)(2);
		BOOST_CHECK_EQUAL(interval1 | interval2, ez::interval(1)(2));
	}

	auto const empty_interval = ez::interval[0][1] | ez::interval[3][4];
	BOOST_CHECK(empty_interval.empty());
}

BOOST_AUTO_TEST_CASE(TypeAliases) {
	{
		ez::open_interval<int> interval{0, 1};
		BOOST_CHECK_EQUAL(interval, ez::interval(0)(1));
	}

	{
		ez::lopen_interval<int> interval{0, 1};
		BOOST_CHECK_EQUAL(interval, ez::interval(0)[1]);
	}

	{
		ez::ropen_interval<int> interval{0, 1};
		BOOST_CHECK_EQUAL(interval, ez::interval[0](1));
	}

	{
		ez::closed_interval<int> interval{0, 1};
		BOOST_CHECK_EQUAL(interval, ez::interval[0][1]);
	}
}

BOOST_AUTO_TEST_CASE(Iterators) {
	{
		auto const interval = ez::interval[1][2];
		BOOST_CHECK_EQUAL(*interval.begin(), 1);
		BOOST_CHECK_EQUAL(*interval.end(),   3);

		BOOST_CHECK_EQUAL(*interval.step(2).begin(), 1);
		BOOST_CHECK_EQUAL(*interval.step(2).end(),   3);

		BOOST_CHECK_EQUAL(*interval.step(3).begin(), 1);
		BOOST_CHECK_EQUAL(*interval.step(3).end(),   4);
	}

	{
		auto const interval = ez::interval[1](2);
		BOOST_CHECK_EQUAL(*interval.begin(), 1);
		BOOST_CHECK_EQUAL(*interval.end(),   2);

		BOOST_CHECK_EQUAL(*interval.step(2).begin(), 1);
		BOOST_CHECK_EQUAL(*interval.step(2).end(),   3);

		BOOST_CHECK_EQUAL(*interval.step(3).begin(), 1);
		BOOST_CHECK_EQUAL(*interval.step(3).end(),   4);
	}

	{
		auto const interval = ez::interval[-2][9];
		BOOST_CHECK_EQUAL(*interval.begin(), -2);
		BOOST_CHECK_EQUAL(*interval.end(),   10);

		BOOST_CHECK_EQUAL(*interval.step(2).begin(), -2);
		BOOST_CHECK_EQUAL(*interval.step(2).end(),   10);

		BOOST_CHECK_EQUAL(*interval.step(3).begin(), -2);
		BOOST_CHECK_EQUAL(*interval.step(3).end(),   10);

		BOOST_CHECK_EQUAL(*interval.step(4).begin(), -2);
		BOOST_CHECK_EQUAL(*interval.step(4).end(),   10);

		BOOST_CHECK_EQUAL(*interval.step(5).begin(), -2);
		BOOST_CHECK_EQUAL(*interval.step(5).end(),   13);

		BOOST_CHECK_EQUAL(*interval.step(6).begin(), -2);
		BOOST_CHECK_EQUAL(*interval.step(6).end(),   10);

		BOOST_CHECK_EQUAL(*interval.step(107).begin(), -2);
		BOOST_CHECK_EQUAL(*interval.step(107).end(),   105);
	}

	{
		auto const interval = ez::interval[-6](-1);
		BOOST_CHECK_EQUAL(*interval.begin(), -6);
		BOOST_CHECK_EQUAL(*interval.end(),   -1);

		BOOST_CHECK_EQUAL(*interval.step(2).begin(), -6);
		BOOST_CHECK_EQUAL(*interval.step(2).end(),    0);

		BOOST_CHECK_EQUAL(*interval.step(3).begin(), -6);
		BOOST_CHECK_EQUAL(*interval.step(3).end(),    0);

		BOOST_CHECK_EQUAL(*interval.step(4).begin(), -6);
		BOOST_CHECK_EQUAL(*interval.step(4).end(),    2);

		BOOST_CHECK_EQUAL(*interval.step(5).begin(), -6);
		BOOST_CHECK_EQUAL(*interval.step(5).end(),   -1);

		BOOST_CHECK_EQUAL(*interval.step(6).begin(), -6);
		BOOST_CHECK_EQUAL(*interval.step(6).end(),    0);
	}
}

BOOST_AUTO_TEST_CASE(RangeLoops) {
	{
		std::stringstream sstream;
		auto numbers = ez::interval['0']['9'];
		for(auto letter : numbers) {
			sstream << letter;
		}
		BOOST_CHECK_EQUAL(sstream.str(), "0123456789");
	}

	{
		std::stringstream sstream;
		auto counting_is_easy = ez::interval[1](10);
		for(auto number : counting_is_easy) {
			sstream << number;
		}
		BOOST_CHECK_EQUAL(sstream.str(), "123456789");
	}

	{
		std::stringstream sstream;
		auto step_interval = ez::interval[0][9];
		for(auto number : step_interval.step(3)) {
			sstream << number;
		}
		BOOST_CHECK_EQUAL(sstream.str(), "0369");
	}

	{
		std::stringstream sstream;
		auto step_interval = ez::interval[0](10);
		for(auto number : step_interval.step(3)) {
			sstream << number;
		}
		BOOST_CHECK_EQUAL(sstream.str(), "0369");
	}
}

BOOST_AUTO_TEST_CASE(ReverseIterations) {
	{
		std::stringstream sstream;
		auto numbers = ez::interval[0][9];
		for(auto letter : numbers.reverse()) {
			sstream << letter;
		}
		BOOST_CHECK_EQUAL(sstream.str(), "9876543210");
	}

	{
		std::stringstream sstream;
		auto numbers = ez::interval(1)[8];
		for(auto number : numbers.reverse()) {
			sstream << number;
		}
		BOOST_CHECK_EQUAL(sstream.str(), "8765432");
	}
}

BOOST_AUTO_TEST_CASE(Contains) {
	auto A = ez::interval[0][2];
	BOOST_CHECK(!A.contains(-1));
	BOOST_CHECK(A.contains(0));
	BOOST_CHECK(A.contains(1));
	BOOST_CHECK(A.contains(2));
	BOOST_CHECK(!A.contains(3));

	auto B = ez::interval[0](2);
	BOOST_CHECK(!B.contains(-1));
	BOOST_CHECK(B.contains(0));
	BOOST_CHECK(B.contains(1));
	BOOST_CHECK(!B.contains(2));
	BOOST_CHECK(!B.contains(3));

	auto C = ez::interval(0)[2];
	BOOST_CHECK(!C.contains(-1));
	BOOST_CHECK(!C.contains(0));
	BOOST_CHECK(C.contains(1));
	BOOST_CHECK(C.contains(2));
	BOOST_CHECK(!C.contains(3));

	auto D = ez::interval(0)(2);
	BOOST_CHECK(!D.contains(-1));
	BOOST_CHECK(!D.contains(0));
	BOOST_CHECK(D.contains(1));
	BOOST_CHECK(!D.contains(2));
	BOOST_CHECK(!D.contains(3));
}

BOOST_AUTO_TEST_CASE(Subsets) {
	{
		auto A = ez::interval[1][2];
		auto B = ez::interval[0][2];

		BOOST_CHECK(A <= B);
		BOOST_CHECK(B >= A);
		BOOST_CHECK(!(A < B));
		BOOST_CHECK(!(B > A));
	}

	{
		auto A = ez::interval[1][2];
		auto B = ez::interval[0][3];

		BOOST_CHECK(A <= B);
		BOOST_CHECK(B >= A);
		BOOST_CHECK(A < B);
		BOOST_CHECK(B > A);
	}

	{
		auto A = ez::interval[1](2);
		auto B = ez::interval[0][2];

		BOOST_CHECK(A <= B);
		BOOST_CHECK(B >= A);
		BOOST_CHECK(A < B);
		BOOST_CHECK(B > A);
	}

	{
		auto A = ez::interval(1)(2);
		auto B = ez::interval(0)(2);

		BOOST_CHECK(A <= B);
		BOOST_CHECK(B >= A);
		BOOST_CHECK(!(A < B));
		BOOST_CHECK(!(B > A));
	}

	{
		auto A = ez::interval(1)(2);
		auto B = ez::interval[1][2];

		BOOST_CHECK(A <= B);
		BOOST_CHECK(B >= A);
		BOOST_CHECK(A < B);
		BOOST_CHECK(B > A);
	}

	{
		auto A = ez::interval(0)(0);
		auto B = ez::interval[1](2);

		BOOST_CHECK(A <= B);
		BOOST_CHECK(B >= A);
		BOOST_CHECK(A < B);
		BOOST_CHECK(B > A);
	}

	{
		auto A = ez::interval(0)[1];
		auto B = ez::interval(2)(2);

		BOOST_CHECK(!(A <= B));
		BOOST_CHECK(!(B >= A));
		BOOST_CHECK(!(A < B));
		BOOST_CHECK(!(B > A));
	}

	{
		auto A = ez::interval(0)(0);
		auto B = ez::interval(2)(2);

		BOOST_CHECK(A <= B);
		BOOST_CHECK(B >= A);
		BOOST_CHECK(!(A < B));
		BOOST_CHECK(!(B > A));
	}
}

BOOST_AUTO_TEST_CASE(SieveOfEratosthenes) {
	std::size_t const limit = 100;
	std::array<bool, limit> is_prime;
	is_prime.fill(true);

	is_prime[0] = false;
	is_prime[1] = false;

	for(auto i : ez::interval[0](limit)) {
		if(is_prime[i]) {
			for(auto multiple_of_i : ez::interval[2 * i](limit).step(i)) {
				is_prime[multiple_of_i] = false;
			}
			//std::cout << i << " ";
		}
	}
}
