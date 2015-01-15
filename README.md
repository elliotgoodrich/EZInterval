EZInterval
==========

A C++11 header-only library to help iterating through numeric types.

As range-for loops and algorithms become more prevalent in modern C++ code, it's become more obvious that there is a gap in functionality with iterating through fundamental types such as `int`, `char` and pointers. EZInterval allows iterating through types which have most of the properties of iterators (incrementing, decrementing, copy construction, equality comparison), but don't provide a dereference operator or `operator->()`.

ez::direct_iterator
--------------------
The way this is achieved is through a class called `ez::direct_iterator<T>`. It takes an object of type `T` and forwards all operators to this object, but when dereferenced, returns a const-reference to this object, rather than dereferencing it. This allows iterators over the built-in numeric types, pointers and other similar types.

    ez::direct_iterator<int> it = 3;
    std::cout << *it; // prints 3
    it += 4;
    std::cout << *it; // prints 7

    // prints 1 2 3 4 5 6 7 8 9
    std::copy(ez::make_direct_iterator(1),
              ez::make_direct_iterator(10),
              std::ostream_iterator<int>(std::cout, " "));

`ez::direct_iterator<T>` can be considered the opposite to `boost::indirect_iterator<T>`, to the point that `boost::indirect_iterator<ez::direct_iterator<T>>` should be functionally identical to `T` when `T` is itself an iterator. A good example of the duality of these classes can be seen in the [sort_selected](examples/sort_selected.cpp) algorithm shown in the examples.

`ez::direct_interval` will figure out what operations the underlying type `T` supports, and will set it's `iterator_category` appropriately. This means that if we have `ez::direct_interval<typedef std::list<int>::iterator>`, then it will have an `iterator_category` of `std::bidirectional_iterator`.

ez::direct_iterator vs boost::counting_iterator
-----------------------------------------------
These classes are almost identical. The main differences are that `boost::counting_iterator` works in C++98 whereas `ez::direct_iterator` is C++11 only. However, `boost::counting_iterator` requires you to specialise a template to work on custom types that aren't iterators or have `std::numeric_limits` specialised. `ez::direct_iterator` figures out the correct iterator tag to use based on what operations that type supports and requires no additional work.

ez::interval<T> and ez::basic_interval<T, bool, bool>
-----------------------------------------------------
The other classes in EZInterval are `ez::basic_interval<T, bool, bool>` and `ez::interval<T>` which represent intervals (where whether the interval is left open and right open is statically determined in `ez::basic_interval` and determined at run-time in `ez::interval`). `ez::basic_interval` can be constructed using the variable `ez::make_interval`.

Intervals can be left-open, right-open, neither, or both. A usual syntax is [a, b] for closed intervals, [a, b) for right-open intervals, (a, b] for left-open intervals, and (a, b) for open intervals. Overloading the functor operator and array index operator, we can mimic this notation (there are 4 template aliases to `ez::basic_interval` for readability, which we use below).

    ez::closed_interval<int> a = ez::make_interval[0][5]; // [0, 5] = {x | 0 <= x <= 5}
    ez::ropen_interval<int>  b = ez::make_interval[0](5); // [0, 5) = {x | 0 <= x < 5}
    ez::lopen_interval<int>  c = ez::make_interval(0)[5]; // (0, 5] = {x | 0 <  x <= 5}
    ez::open_interval<int>   d = ez::make_interval(0)(5); // (0, 5) = {x | 0 <  x < 5}

These classes have `begin()` and `end()` defined, which return appropriate values of `ez::direct_iterator`, so these objects are available to use in range-for loops.

    // prints 2 3 4
    for(auto i : ez::make_interval(1)[4]) {
        std::cout << i;
    }

    std::vector<int> vec = foo();
    for(auto p : ez::make_interval[vec.begin()](vec.end())) {
        auto const& i = *p;
        auto const offset = p - vec.begin();
        // do stuff
    }

Arithmetic
----------
The interval classes have +, -, /, and * defined as the usual interval arithmetic functions.

    auto i = ez::make_interval[0][4]  + ez::interval(1)(5); // i = (1, 9)
    auto j = ez::make_interval[-1][2] * ez::interval(0)[3]; // j = [-3, 6]

Most of these operators return a value of type `ez::interval`, even if the parameters are of type of `ez::basic_interval`. This is because the openness of the result depends on the lower and upper bounds of the intervals, which is decided at run-time. However, there are several cases where operators can return `ez::basic_interval`.

* operator+ if both parameters have the same openness
* operator- if both parameters have the same openness
* operator/ if both parameters are closed, or if both parameters are open
* operator* if both parameters are closed, or if both parameters are open

ez::interval vs boost::interval
-------------------------------
There is a small overlap in functionality between this library and Boost's interval arithmetic library, but the libraries are fundamentally different.

Boost's interval arithmetic library is a high-quality library aimed at making interval arithmetic easy. There is no notion of an open interval; all intervals are closed. One main difference is that Boost's library has wide functionality to deal with rounding. For example, if we have a type `SmallFloat`, which can only store decimal values to 2 significant figures, we can see the difference when doing arithmetic with this type.

    boost::interval<SmallFloat> r(11.0, 12.0);
    r += 0.1;
    assert(r == boost::interval<SmallFloat>(11.0, 13.0));

    ez::open_interval<SmallFloat> r(11.0, 12.0);
    r += 0.1;
    assert(r == ez::open_interval<SmallFloat(11.0, 12.0));

As you can see, Boost can guarantee that the resulting interval will always be a strict superset of the interval if we were using a type that has infinite accuracy. EZInterval's interval classes have a more naive approach to arithmetic.

However, there is no mechanisms to iterate through the interval.

EZInterval should be used to facilitate iteration, but it shouldn't be used as an accurate interval arithmetic library. Basically, if you want accurate arithmetic or are using float, double, or long double, you probably want to use Boost. If you just want some syntactic sugar to iterate through some numbers or pointers, then use EZInterval.
