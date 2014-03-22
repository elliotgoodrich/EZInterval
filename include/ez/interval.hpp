/******************************************************************//**
 *
 *
 *
 *********************************************************************/

#ifndef INCLUDE_GUARD_97581AFD_D7FC_4A4A_9DAD_D0C5E1A7111C
#define INCLUDE_GUARD_97581AFD_D7FC_4A4A_9DAD_D0C5E1A7111C

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ostream>
#include <type_traits>
#include <stdexcept>
#include <utility>

namespace ez {
namespace detail {

/* Finds the first number in the series a_n = lowerbound + step * n such that a_n is not between
   the interval lowerbound to upperbound. */
template <bool IsStrict>
struct sentinel_helper;

template <>
struct sentinel_helper<false> {
	template <typename T>
	static T f(T const& lowerbound, T const& upperbound, T const& step) {
		return static_cast<std::ptrdiff_t>((upperbound - lowerbound + step - 1) / step) * step
		                                   + lowerbound;
	}
};

template <>
struct sentinel_helper<true> {
	template <typename T>
	static T f(T const& lowerbound, T const& upperbound, T const& step) {
		return static_cast<std::ptrdiff_t>((upperbound - lowerbound + step) / step) * step + lowerbound;
	}
};

/* Helper function to compare two numbers. */
template<bool StrictComparison>
struct less_than_helper;

template <>
struct less_than_helper<false> {
	template <typename N1, typename N2>
	static bool f(N1 const& lhs, N2 const& rhs) {
		return lhs < rhs;
	}
};

template<>
struct less_than_helper<true> {
	template <typename N1, typename N2>
	static bool f(N1 const& lhs, N2 const& rhs) {
		return lhs <= rhs;
	}
};

/* Helper class to determine if one interval is a proper subset of another. */
template <bool LhsLOpen, bool LhsROpen, bool RhsLOpen, bool RhsROpen>
struct proper_subset_helper {
	static bool const lowerbound_strict_comp = LhsLOpen && !RhsLOpen;
	static bool const upperbound_strict_comp = LhsROpen && !RhsROpen;

	/* Returns true if \a N1 is a proper subset of \a N2. */
	template <typename N1, typename N2>
	static bool f(N1 const& lhs, N2 const& rhs) {
		return (lhs.empty() && !rhs.empty()) ||
		       (less_than_helper<lowerbound_strict_comp>::f(rhs.lower(), lhs.lower()) &&
		        less_than_helper<upperbound_strict_comp>::f(lhs.upper(), rhs.upper()));
	}
};

/* Helper class to determine if one interval is a proper superset of another. */
template <bool LhsLOpen, bool LhsROpen, bool RhsLOpen, bool RhsROpen>
struct proper_superset_helper {
	template <typename N1, typename N2>

	/* Returns true if \a N1 is a proper superset of \a N2. */
	static bool f(N1 const& lhs, N2 const& rhs) {
		return proper_subset_helper<RhsLOpen, RhsROpen, LhsLOpen, LhsROpen>::f(rhs, lhs);
	}
};

/* Helper class to determine if one interval is a subset of another. */
template <bool LhsLOpen, bool LhsROpen, bool RhsLOpen, bool RhsROpen>
struct subset_helper {
	static bool const lowerbound_strict_comp = LhsLOpen || !RhsLOpen;
	static bool const upperbound_strict_comp = LhsROpen || !RhsROpen;

	/* Returns true if \a N1 is a subset of \a N2. */
	template <typename N1, typename N2>
	static bool f(N1 const& lhs, N2 const& rhs) {
		return lhs.empty() ||
		       (less_than_helper<lowerbound_strict_comp>::f(rhs.lower(), lhs.lower()) &&
		        less_than_helper<upperbound_strict_comp>::f(lhs.upper(), rhs.upper()));
	}
};

/* Helper class to determine if one interval is a superset of another. */
template <bool LhsLOpen, bool LhsROpen, bool RhsLOpen, bool RhsROpen>
struct superset_helper {
	template <typename N1, typename N2>

	/* Returns true if \a N1 is a superset of \a N2. */
	static bool f(N1 const& lhs, N2 const& rhs) {
		return subset_helper<RhsLOpen, RhsROpen, LhsLOpen, LhsROpen>::f(rhs, lhs);
	}
};

/* Helper class to determine equality. */
template <bool LhsLOpen, bool LhsUOpen, bool RhsLOpen, bool RhsUOpen>
struct equality_helper {
	/* Returns true if \a N1 equals \a N2. */
	template <typename N1, typename N2>
	static bool f(N1 const& lhs, N2 const& rhs) {
		return (lhs.empty() && rhs.empty()) ||
		       (LhsLOpen == RhsLOpen &&
		        LhsUOpen == RhsUOpen &&
		        lhs.lower() == rhs.lower() &&
		        lhs.upper() == rhs.upper());
	}
};

}

/** An empty_interval exception will be thrown when trying to call a method of \ref basic_interval
    which is undefined on an empty interval. */
class empty_interval : public std::logic_error {
public:
	using std::logic_error::logic_error;
};

/** An iterator across a specified numerical range. */
template <typename T>
class const_interval_iterator : public std::random_access_iterator_tag {
public:
	/** Constructs a default iterator. */
	const_interval_iterator()
	: m_value{}
	, m_step{} {
	}

	/** Creates an iterator with \a value jumping by \a step. */
	const_interval_iterator(T const& value, T const& step)
	: m_value{value}
	, m_step{step} {
	}

	~const_interval_iterator() noexcept = default;

	const_interval_iterator(const_interval_iterator<T> const&) noexcept = default;

	const_interval_iterator<T>& operator=(const_interval_iterator<T> const&) noexcept = default;

	T const& operator*() const {
		return m_value;
	}
	T const* operator->() const {
		return &m_value;
	}

	const_interval_iterator<T>& operator++() {
		m_value += m_step;
		return *this;
	}

	const_interval_iterator<T> operator++(int) {
		auto const copy = *this;
		++(*this);
		return copy;
	}

	const_interval_iterator<T>& operator--() {
		m_value -= m_step;
		return *this;
	}

	const_interval_iterator<T> operator--(int) {
		auto const copy = *this;
		--(*this);
		return copy;
	}

	friend bool operator==(const_interval_iterator<T> const& lhs,
	                       const_interval_iterator<T> const& rhs) {
		return lhs.m_value == rhs.m_value;
	}

	friend bool operator!=(const_interval_iterator<T> const& lhs,
	                       const_interval_iterator<T> const& rhs) {
		return !(lhs == rhs);
	}

	const_interval_iterator<T>& operator+=(std::ptrdiff_t n) {
		m_value += (n * m_step);
		return *this;
	}

	const_interval_iterator<T> operator+(std::ptrdiff_t n) const {
		auto copy = *this;
		copy += n;
		return copy;
	}

	friend const_interval_iterator<T> operator+(std::ptrdiff_t n,
	                                            const_interval_iterator<T> const& it) {
		auto copy = it;
		copy += n;
		return copy;
	}

	const_interval_iterator<T>& operator-=(std::ptrdiff_t n) {
		m_value -= (n * m_step);
		return *this;
	}

	const_interval_iterator<T> operator-(std::ptrdiff_t n) const {
		auto copy = *this;
		copy -= n;
		return copy;
	}

	const_interval_iterator<T> operator[](std::ptrdiff_t n) const {
		return *(*this + n);
	}

	friend bool operator<(const_interval_iterator<T> const& lhs,
	                      const_interval_iterator<T> const& rhs) {
		return lhs.m_value < rhs.m_value;
	}

	friend bool operator<=(const_interval_iterator<T> const& lhs,
	                       const_interval_iterator<T> const& rhs) {
		return lhs.m_value <= rhs.m_value;
	}

	friend bool operator>(const_interval_iterator<T> const& lhs,
	                      const_interval_iterator<T> const& rhs) {
		return lhs.m_value > rhs.m_value;
	}

	friend bool operator>=(const_interval_iterator<T> const& lhs,
	                       const_interval_iterator<T> const& rhs) {
		return lhs.m_value >= rhs.m_value;
	}

private:
	T m_value;
	T const m_step;
};

/** Convenience class to generate stepped iterator when begin and end functions are called. */
template <typename T, bool LOpen, bool ROpen>
class stepped_interval {
public:
	/** Create an object that generates iterators from \a lowerbound to \a upperbound with
	    a step of \a step. */
	stepped_interval(T const& lowerbound, T const& upperbound, T const& step)
	: m_lowerbound{lowerbound}
	, m_upperbound{upperbound}
	, m_step{step} {
	}

	/** Create a stepped_interval in reverse. */
	stepped_interval<T, ROpen, LOpen> reverse() const {
		return {m_upperbound, m_lowerbound, -m_step};
	}

	const_interval_iterator<T> begin() const {
		static_assert(!LOpen, "Cannot iterate through an interval that is left-open.");
		return {m_lowerbound, m_step};
	}

	const_interval_iterator<T> end() const {
		return {detail::sentinel_helper<!ROpen>::f(m_lowerbound, m_upperbound, m_step), m_step};
	}

	const_interval_iterator<T> rbegin() const {
		static_assert(!ROpen, "Cannot iterate backwards through an interval that is right-open.");
		return {detail::sentinel_helper<!ROpen>::f(m_lowerbound, m_upperbound, m_step) - m_step,
		        -m_step};
	}

	const_interval_iterator<T> rend() const {
		return {m_lowerbound - m_step, -m_step};
	}

private:
	T const m_lowerbound;
	T const m_upperbound;
	T const m_step;
};

template <typename T, bool LOpen, bool ROpen>
class basic_interval;

/** Helper function to create an interval from the common type of \a lowerbound and \a upperbound. */
template <bool LOpen, bool ROpen, typename L, typename U>
auto create_interval(L const& lowerbound, U const& upperbound) ->
  basic_interval<typename std::common_type<L, U>::type, LOpen, ROpen> {
	typedef typename std::common_type<L, U>::type Common;
	return {static_cast<Common>(lowerbound), static_cast<Common>(upperbound)};
}

/** This class represents a mathematical interval that can be iterated over if the underlying type
    is exact. */
template <typename T, bool LOpen, bool ROpen>
class basic_interval {
public:
	typedef T type;

	basic_interval(T const& lowerbound, T const& upperbound)
	: m_lowerbound{lowerbound}
	, m_upperbound{upperbound} {
	}

	basic_interval(T&& lowerbound, T&& upperbound)
	: m_lowerbound{std::move(lowerbound)}
	, m_upperbound{std::move(upperbound)} {
	}

	/** Returns the lowerbound. */
	T const& lower() const {
		return m_lowerbound;
	}

	/** Returns the upperbound. */
	T const& upper() const {
		return m_upperbound;
	}

	/** Returns true if this interval is empty. */
	bool empty() const {
		return detail::less_than_helper<LOpen || ROpen>::f(m_upperbound, m_lowerbound);
	}

	/** Same as the empty() function. */
	explicit operator bool() const {
		return !empty();
	}

	/** Returns the difference between the upper and lower bound. */
	T diameter() const {
		if(empty())
			throw empty_interval{"The diameter of an empty interval is undefined."};
		return m_upperbound - m_lowerbound;
	}

	/** Returns the midpoint between the upper and lower bound.

	    Pass an additional type if the underlying type of basic_interval is not appropriate. For
	    example, if you have an interval of [0, 3]. Then midpoint<int> will give (3-0)/2 = 1, but
	    midpoint<double> will give (3.0-1.0)/2 = 1.5
	    */
	template <typename U = T>
	U midpoint() const {
		if(empty())
			throw empty_interval{"The midpoint of an empty interval is undefined."};
		return (static_cast<U>(m_upperbound) + static_cast<U>(m_lowerbound)) / 2;
	}

	/** Returns half of the diameter.

	    Pass a different type if you are worried about rounding issues with the underlying type. */
	template <typename U = T>
	U radius() const {
		if(empty())
			throw empty_interval{"The radius of an empty interval is undefined."};
		return static_cast<U>(diameter()) / 2;
	}

	/** Returns true if \a value is within the interval. */
	bool contains(T const& value) const {
		return detail::less_than_helper<!LOpen>::f(m_lowerbound, value) &&
		       detail::less_than_helper<!ROpen>::f(value, m_upperbound);
	}

	/** Returns true if this object is a subset of \a rhs. */
	template <typename T2, bool LOpen2, bool ROpen2>
	bool operator<(basic_interval<T2, LOpen2, ROpen2> const& rhs) const {
		return detail::proper_subset_helper<LOpen, ROpen, LOpen2, ROpen2>::f(*this, rhs);
	}

	/** Returns true if this object is a proper subset of \a rhs. */
	template <typename T2, bool LOpen2, bool ROpen2>
	bool operator<=(basic_interval<T2, LOpen2, ROpen2> const& rhs) const {
		return detail::subset_helper<LOpen, ROpen, LOpen2, ROpen2>::f(*this, rhs);
	}

	/** Returns true if this object is a superset of \a rhs. */
	template <typename T2, bool LOpen2, bool ROpen2>
	bool operator>(basic_interval<T2, LOpen2, ROpen2> const& rhs) const {
		return detail::proper_superset_helper<LOpen, ROpen, LOpen2, ROpen2>::f(*this, rhs);
	}

	/** Returns true if this object is a proper superset of \a rhs. */
	template <typename T2, bool LOpen2, bool ROpen2>
	bool operator>=(basic_interval<T2, LOpen2, ROpen2> const& rhs) const {
		return detail::superset_helper<LOpen, ROpen, LOpen2, ROpen2>::f(*this, rhs);
	}

	/** Returns true if this interval is equal to \a rhs. */
	template <typename T2, bool LOpen2, bool ROpen2>
	bool operator==(basic_interval<T2, LOpen2, ROpen2> const& rhs) const {
		return detail::equality_helper<LOpen, ROpen, LOpen2, ROpen2>::f(*this, rhs);
	}

	/** Returns true if this interval is not equal to \a rhs. */
	template <typename T2, bool LOpen2, bool ROpen2>
	bool operator!=(basic_interval<T2, LOpen2, ROpen2> const& rhs) const {
		return !(*this == rhs);
	}

	/** Return a proxy object which has a begin and end function. */
	stepped_interval<T, LOpen, ROpen> step(T const& step) const {
		static_assert(std::numeric_limits<T>::is_exact, "Unable to iterate through an interval in"
		                                                "which the underlying type is not exact.");
		return {m_lowerbound, m_upperbound, step};
	}

	const_interval_iterator<T> begin() const {
		return step(1).begin();
	}

	const_interval_iterator<T> end() const {
		return step(1).end();
	}

	const_interval_iterator<T> rbegin() const {
		return step(1).rbegin();
	}

	const_interval_iterator<T> rend() const {
		return step(1).rend();
	}

	stepped_interval<T, ROpen, LOpen> reverse() const {
		return step(1).reverse();
	}

private:
	T const m_lowerbound;
	T const m_upperbound;
};

/** Serialise a basic_interval to a stream. */
template <typename T, bool LOpen, bool ROpen>
std::ostream& operator<<(std::ostream& stream,
                         basic_interval<T, LOpen, ROpen> const& interval) {
	return stream << (LOpen ? "(" : "[")
	              << interval.lower() << ", " << interval.upper()
	              << (ROpen ? ")" : "]");
}

/** Find the intersection of two basic_intervals. */
template <typename T1, typename T2, bool LOpen, bool ROpen>
basic_interval<typename std::common_type<T1, T2>::type, LOpen, ROpen>
  operator|(basic_interval<T1, LOpen, ROpen> const& lhs,
            basic_interval<T2, LOpen, ROpen> const& rhs) {
	typedef typename std::common_type<T1, T2>::type Common;
	return basic_interval<Common, LOpen, ROpen>(std::max<Common>(lhs.lower(), rhs.lower()),
	                                            std::min<Common>(lhs.upper(), rhs.upper()));
}

/** An intermediate class used to generate a basic_interval. */
template <typename L, bool LOpen>
class lower_bound {
public:
	explicit lower_bound(L&& lowerbound)
	: m_lowerbound{std::forward<L>(lowerbound)} {
	}

	template <typename U>
	auto operator()(U&& upperbound) const & ->
	  basic_interval<typename std::common_type<L, U>::type, LOpen, true> {
		return create_interval<LOpen, true>(m_lowerbound, std::forward<U>(upperbound));
	}

	template <typename U>
	auto operator()(U&& upperbound) && ->
	  basic_interval<typename std::common_type<L, U>::type, LOpen, true> {
		return create_interval<LOpen, true>(std::move(m_lowerbound), std::forward<U>(upperbound));
	}

	template <typename U>
	auto operator[](U&& upperbound) const & ->
	  basic_interval<typename std::common_type<L, U>::type, LOpen, false> {
		return create_interval<LOpen, false>(m_lowerbound, std::forward<U>(upperbound));
	}

	template <typename U>
	auto operator[](U&& upperbound) && ->
	  basic_interval<typename std::common_type<L, U>::type, LOpen, false> {
		return create_interval<LOpen, false>(std::move(m_lowerbound), std::forward<U>(upperbound));
	}

private:
	L m_lowerbound;
};

/** Used to create an object of lower_bound which is then used to create the basic_interval
    object. */
class interval_factory {
public:
	template <typename L>
	lower_bound<L, true> operator()(L&& lowerbound) const {
		return lower_bound<L, true>{std::forward<L>(lowerbound)};
	}

	template <typename L>
	lower_bound<L, false> operator[](L&& lowerbound) const {
		return lower_bound<L, false>{std::forward<L>(lowerbound)};
	}
};

/** This variable is used to create intervals by calling the index array operator and function
    operator. */
interval_factory interval;

/** Typedef for an open interval. */
template <typename T>
using open_interval = basic_interval<T, true, true>;

/** Typedef for a closed interval. */
template <typename T>
using closed_interval = basic_interval<T, false, false>;

/** Typedef for left open/right closed interval. */
template <typename T>
using lopen_interval = basic_interval<T, true, false>;

/** Typedef for left closed/right open interval. */
template <typename T>
using ropen_interval = basic_interval<T, false, true>;

}

#endif
