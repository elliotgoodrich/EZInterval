#include "ez/direct_iterator.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

// Workaround for missing std::string implementation on some systems
template <typename T>
std::string to_string(T const& t) {
	std::stringstream ss;
	ss << t;
	return ss.str();
}

int main() {
	auto fizzbuzz = [](int i){
		if(i % 15 == 0) return std::string{"fizzbuzz"};
		if(i % 3  == 0) return std::string{"fizz"};
		if(i % 5  == 0) return std::string{"buzz"};
		return to_string(i);
	};

	std::cout << "FizzBuzz\n";
	std::transform(ez::make_direct_iterator(1),
	               ez::make_direct_iterator(101),
	               std::ostream_iterator<std::string>(std::cout, ", "),
	               fizzbuzz);

	std::cout << '\n';
}
