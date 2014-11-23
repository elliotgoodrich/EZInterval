#include "ez/interval.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(interval)

BOOST_AUTO_TEST_CASE(Constructor) {
	ez::interval<int> const A = ez::make_interval[0][0];
	BOOST_CHECK_EQUAL(A.lower(), 0);
	BOOST_CHECK_EQUAL(A.upper(), 0);
	BOOST_CHECK_EQUAL(A.left_open(), false);
	BOOST_CHECK_EQUAL(A.left_closed(), true);
	BOOST_CHECK_EQUAL(A.right_open(), false);
	BOOST_CHECK_EQUAL(A.right_closed(), true);

	ez::interval<int> const B = ez::make_interval[3](4);
	BOOST_CHECK_EQUAL(B.lower(), 3);
	BOOST_CHECK_EQUAL(B.upper(), 4);
	BOOST_CHECK_EQUAL(B.left_open(), false);
	BOOST_CHECK_EQUAL(B.left_closed(), true);
	BOOST_CHECK_EQUAL(B.right_open(), true);
	BOOST_CHECK_EQUAL(B.right_closed(), false);

	ez::interval<int> const C = ez::make_interval(-2)[7];
	BOOST_CHECK_EQUAL(C.lower(), -2);
	BOOST_CHECK_EQUAL(C.upper(), 7);
	BOOST_CHECK_EQUAL(C.left_open(), true);
	BOOST_CHECK_EQUAL(C.left_closed(), false);
	BOOST_CHECK_EQUAL(C.right_open(), false);
	BOOST_CHECK_EQUAL(C.right_closed(), true);

	ez::interval<double> const D = ez::make_interval(1.0)(5.5);
	BOOST_CHECK_EQUAL(D.lower(), 1.0);
	BOOST_CHECK_EQUAL(D.upper(), 5.5);
	BOOST_CHECK_EQUAL(D.left_open(), true);
	BOOST_CHECK_EQUAL(D.left_closed(), false);
	BOOST_CHECK_EQUAL(D.right_open(), true);
	BOOST_CHECK_EQUAL(D.right_closed(), false);

	BOOST_CHECK_THROW(ez::make_interval[0](0), ez::empty_interval);
	BOOST_CHECK_THROW(ez::make_interval(0)[0], ez::empty_interval);
	BOOST_CHECK_THROW(ez::make_interval(0)(0), ez::empty_interval);
	BOOST_CHECK_THROW(ez::make_interval[1][0], ez::empty_interval);
}

BOOST_AUTO_TEST_CASE(BeginEndTest) {
	ez::interval<int> A = ez::make_interval(0)[4];
	BOOST_CHECK_EQUAL(*A.begin(), 1);
	BOOST_CHECK_EQUAL(*A.cbegin(), 1);
	BOOST_CHECK_EQUAL(*A.end(), 5);
	BOOST_CHECK_EQUAL(*A.cend(), 5);

	ez::interval<int> B = ez::make_interval[0](4);
	BOOST_CHECK_EQUAL(*B.begin(), 0);
	BOOST_CHECK_EQUAL(*B.cbegin(), 0);
	BOOST_CHECK_EQUAL(*B.end(), 4);
	BOOST_CHECK_EQUAL(*B.cend(), 4);
}

BOOST_AUTO_TEST_CASE(EqualityTest) {
	BOOST_CHECK_EQUAL(ez::interval<int>{ez::make_interval[0][3]}, ez::interval<int>{ez::make_interval[0][3]});
	BOOST_CHECK_EQUAL(ez::make_interval[0.0][3.0], ez::make_interval[0.0][3.0]);

	BOOST_CHECK(ez::interval<int>{ez::make_interval[0][1]} != ez::interval<int>{ez::make_interval[0](1)});
	BOOST_CHECK(ez::interval<int>{ez::make_interval[0][1]} != ez::interval<int>{ez::make_interval(0)[1]});
	BOOST_CHECK(ez::interval<int>{ez::make_interval[0][1]} != ez::interval<int>{ez::make_interval(0)(1)});
	BOOST_CHECK(ez::interval<int>{ez::make_interval[0](1)} != ez::interval<int>{ez::make_interval(0)[1]});
	BOOST_CHECK(ez::interval<int>{ez::make_interval[0](1)} != ez::interval<int>{ez::make_interval(0)(1)});
	BOOST_CHECK(ez::interval<int>{ez::make_interval(0)[1]} != ez::interval<int>{ez::make_interval(0)(1)});
}

BOOST_AUTO_TEST_CASE(Singleton) {
	BOOST_CHECK(singleton(ez::interval<int>{ez::make_interval[0][0]}));
	BOOST_CHECK(singleton(ez::interval<int>{ez::make_interval[1][1]}));
	BOOST_CHECK(!singleton(ez::interval<int>{ez::make_interval[0][1]}));
}

BOOST_AUTO_TEST_CASE(CommonPropertiesTest) {
	ez::interval<int> const int_interval = ez::make_interval(-6)[9];
	BOOST_CHECK_EQUAL(15, diameter(int_interval));
	BOOST_CHECK_EQUAL(1, midpoint(int_interval));
	BOOST_CHECK_EQUAL(7, radius(int_interval));

	BOOST_CHECK_EQUAL(1.5, ez::midpoint<double>(int_interval));
	BOOST_CHECK_EQUAL(7.5, ez::radius<double>(int_interval));

	ez::interval<double> const double_interval = ez::make_interval(-6.0)[9.0];
	BOOST_CHECK_EQUAL(15.0, diameter(double_interval));
	BOOST_CHECK_EQUAL(1.5, midpoint(double_interval));
	BOOST_CHECK_EQUAL(7.5, radius(double_interval));
}

BOOST_AUTO_TEST_CASE(Casting) {
	ez::interval<int> const a = ez::make_interval[0][0];
	auto b = ez::interval_cast<double>(a);
	static_assert(std::is_same<decltype(b)::type, double>::value, "");
}

BOOST_AUTO_TEST_CASE(Contains) {
	ez::interval<int> const A = ez::make_interval[0][2];
	BOOST_CHECK(!contains(A, -1));
	BOOST_CHECK(contains(A, 0));
	BOOST_CHECK(contains(A, 1));
	BOOST_CHECK(contains(A, 2));
	BOOST_CHECK(!contains(A, 3));

	ez::interval<int> const B = ez::make_interval[0](2);
	BOOST_CHECK(!contains(B, -1));
	BOOST_CHECK(contains(B, 0));
	BOOST_CHECK(contains(B, 1));
	BOOST_CHECK(!contains(B, 2));
	BOOST_CHECK(!contains(B, 3));

	ez::interval<int> const C = ez::make_interval(0)[2];
	BOOST_CHECK(!contains(C, -1));
	BOOST_CHECK(!contains(C, 0));
	BOOST_CHECK(contains(C, 1));
	BOOST_CHECK(contains(C, 2));
	BOOST_CHECK(!contains(C, 3));

	ez::interval<int> const D = ez::make_interval(0)(2);
	BOOST_CHECK(!contains(D, -1));
	BOOST_CHECK(!contains(D, 0));
	BOOST_CHECK(contains(D, 1));
	BOOST_CHECK(!contains(D, 2));
	BOOST_CHECK(!contains(D, 3));
}

BOOST_AUTO_TEST_CASE(Intersection) {
	{
		ez::interval<int> const interval1 = ez::make_interval[1][3];
		ez::interval<int> const interval2 = ez::make_interval[0][2];
		BOOST_CHECK_EQUAL(intersection(interval1, interval2),
		                  ez::interval<int>{ez::make_interval[1][2]});
	}

	{
		ez::interval<int> const interval1 = ez::make_interval(1)[3];
		ez::interval<int> const interval2 = ez::make_interval(0)[2];
		BOOST_CHECK_EQUAL(intersection(interval1, interval2),
		                  ez::interval<int>{ez::make_interval(1)[2]});
	}

	{
		ez::interval<int> const interval1 = ez::make_interval[1](3);
		ez::interval<int> const interval2 = ez::make_interval[0](2);
		BOOST_CHECK_EQUAL(intersection(interval1, interval2),
		                  ez::interval<int>{ez::make_interval[1](2)});
	}

	{
		ez::interval<int> const interval1 = ez::make_interval(1)(3);
		ez::interval<int> const interval2 = ez::make_interval(0)(2);
		BOOST_CHECK_EQUAL(intersection(interval1, interval2),
		                  ez::interval<int>{ez::make_interval(1)(2)});
	}

	BOOST_CHECK_THROW(intersection(ez::interval<int>{ez::make_interval[0][1]},
	                 ez::interval<int>{ez::make_interval[3][4]}), ez::empty_interval);
}

BOOST_AUTO_TEST_CASE(Subsets) {
	{
		ez::interval<int> const A = ez::make_interval[1][2];
		ez::interval<int> const B = ez::make_interval[0][2];

		BOOST_CHECK(subset(A,  B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(!proper_subset(A, B));
		BOOST_CHECK(!proper_superset(B, A));
	}

	{
		ez::interval<int> const A = ez::make_interval[1][2];
		ez::interval<int> const B = ez::make_interval[0][3];

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(proper_subset(A, B));
		BOOST_CHECK(proper_superset(B, A));
	}

	{
		ez::interval<int> const A = ez::make_interval[1](2);
		ez::interval<int> const B = ez::make_interval[0][2];

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(proper_subset(A, B));
		BOOST_CHECK(proper_superset(B, A));
	}

	{
		ez::interval<int> const A = ez::make_interval(1)(2);
		ez::interval<int> const B = ez::make_interval(0)(2);

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(!(proper_subset(A, B)));
		BOOST_CHECK(!(proper_superset(B, A)));
	}

	{
		ez::interval<int> const A = ez::make_interval(1)(2);
		ez::interval<int> const B = ez::make_interval[1][2];

		BOOST_CHECK(subset(A, B));
		BOOST_CHECK(superset(B, A));
		BOOST_CHECK(proper_subset(A, B));
		BOOST_CHECK(proper_superset(B, A));
	}
}

BOOST_AUTO_TEST_CASE(Arithmetic) {
	ez::interval<int> const A = ez::make_interval[0][4];
	ez::interval<int> const B = ez::make_interval(2)(3);
	ez::interval<int> const C = ez::make_interval[-2](2);
	ez::interval<int> const D = ez::make_interval(0)[3];

	// Addition
	BOOST_CHECK_EQUAL(A + A, ez::interval<int>{ez::make_interval[0][8]});
	BOOST_CHECK_EQUAL(A + B, ez::interval<int>{ez::make_interval(2)(7)});
	BOOST_CHECK_EQUAL(A + C, ez::interval<int>{ez::make_interval[-2](6)});
	BOOST_CHECK_EQUAL(A + D, ez::interval<int>{ez::make_interval(0)[7]});

	BOOST_CHECK_EQUAL(B + B, ez::interval<int>{ez::make_interval(4)(6)});
	BOOST_CHECK_EQUAL(B + C, ez::interval<int>{ez::make_interval(0)(5)});
	BOOST_CHECK_EQUAL(B + D, ez::interval<int>{ez::make_interval(2)(6)});

	BOOST_CHECK_EQUAL(C + C, ez::interval<int>{ez::make_interval[-4](4)});
	BOOST_CHECK_EQUAL(C + D, ez::interval<int>{ez::make_interval(-2)(5)});

	BOOST_CHECK_EQUAL(D + D, ez::interval<int>{ez::make_interval(0)[6]});

	BOOST_CHECK_EQUAL(A + B, B + A);
	BOOST_CHECK_EQUAL(A + C, C + A);
	BOOST_CHECK_EQUAL(A + D, D + A);
	BOOST_CHECK_EQUAL(B + C, C + B);
	BOOST_CHECK_EQUAL(B + D, D + B);
	BOOST_CHECK_EQUAL(C + D, D + C);

	// Subtraction
	BOOST_CHECK_EQUAL(A - A, ez::interval<int>{ez::make_interval[-4][4]});
	BOOST_CHECK_EQUAL(A - B, ez::interval<int>{ez::make_interval(-3)(2)});
	BOOST_CHECK_EQUAL(A - C, ez::interval<int>{ez::make_interval(-2)[6]});
	BOOST_CHECK_EQUAL(A - D, ez::interval<int>{ez::make_interval[-3](4)});

	BOOST_CHECK_EQUAL(B - A, ez::interval<int>{ez::make_interval(-2)(3)});
	BOOST_CHECK_EQUAL(B - B, ez::interval<int>{ez::make_interval(-1)(1)});
	BOOST_CHECK_EQUAL(B - C, ez::interval<int>{ez::make_interval(0)(5)});
	BOOST_CHECK_EQUAL(B - D, ez::interval<int>{ez::make_interval(-1)(3)});

	BOOST_CHECK_EQUAL(C - A, ez::interval<int>{ez::make_interval[-6](2)});
	BOOST_CHECK_EQUAL(C - B, ez::interval<int>{ez::make_interval(-5)(0)});
	BOOST_CHECK_EQUAL(C - C, ez::interval<int>{ez::make_interval(-4)(4)});
	BOOST_CHECK_EQUAL(C - D, ez::interval<int>{ez::make_interval[-5](2)});

	BOOST_CHECK_EQUAL(D - A, ez::interval<int>{ez::make_interval(-4)[3]});
	BOOST_CHECK_EQUAL(D - B, ez::interval<int>{ez::make_interval(-3)(1)});
	BOOST_CHECK_EQUAL(D - C, ez::interval<int>{ez::make_interval(-2)[5]});
	BOOST_CHECK_EQUAL(D - D, ez::interval<int>{ez::make_interval(-3)(3)});

	// Division
	BOOST_CHECK_THROW(A / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(A / B, ez::interval<int>{ez::make_interval[0](2)});
	BOOST_CHECK_THROW(A / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(A / D, ez::divide_by_zero);

	BOOST_CHECK_THROW(B / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(B / B, ez::interval<int>{ez::make_interval(0)(1)});
	BOOST_CHECK_THROW(B / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(B / D, ez::divide_by_zero);

	BOOST_CHECK_THROW(C / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(C / B, ez::interval<int>{ez::make_interval(-1)(1)});
	BOOST_CHECK_THROW(C / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(C / D, ez::divide_by_zero);

	BOOST_CHECK_THROW(D / A, ez::divide_by_zero);
	BOOST_CHECK_EQUAL(D / B, ez::interval<int>{ez::make_interval[0](1)});
	BOOST_CHECK_THROW(D / C, ez::divide_by_zero);
	BOOST_CHECK_THROW(D / D, ez::divide_by_zero);

	// Multiplication
	BOOST_CHECK_EQUAL(A * A, ez::interval<int>{ez::make_interval[0][16]});
	BOOST_CHECK_EQUAL(A * B, ez::interval<int>{ez::make_interval[0](12)});
	BOOST_CHECK_EQUAL(A * C, ez::interval<int>{ez::make_interval[-8](8)});
	BOOST_CHECK_EQUAL(A * D, ez::interval<int>{ez::make_interval[0][12]});

	BOOST_CHECK_EQUAL(B * B, ez::interval<int>{ez::make_interval(4)(9)});
	BOOST_CHECK_EQUAL(B * C, ez::interval<int>{ez::make_interval(-6)(6)});
	BOOST_CHECK_EQUAL(B * D, ez::interval<int>{ez::make_interval(0)(9)});

	BOOST_CHECK_EQUAL(C * C, ez::interval<int>{ez::make_interval(-4)[4]});
	BOOST_CHECK_EQUAL(C * D, ez::interval<int>{ez::make_interval[-6](6)});

	BOOST_CHECK_EQUAL(D * D, ez::interval<int>{ez::make_interval(0)[9]});

	BOOST_CHECK_EQUAL(A * B, B * A);
	BOOST_CHECK_EQUAL(A * C, C * A);
	BOOST_CHECK_EQUAL(A * D, D * A);
	BOOST_CHECK_EQUAL(B * C, C * B);
	BOOST_CHECK_EQUAL(B * D, D * B);
	BOOST_CHECK_EQUAL(C * D, D * C);
}

BOOST_AUTO_TEST_SUITE_END()
