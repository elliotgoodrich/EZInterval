#include "ez/interval.hpp"

#include <iostream>
#include <vector>

int main() {
	// Iterate through digits of phi using a standard range-for loop
	std::cout << "phi: 1.";
	auto phi = std::vector<int>{6, 1, 8, 0, 3, 3, 9, 8, 8, 7, 4, 9, 8};
	for(auto digit : phi) {
		std::cout << digit;
	}
	std::cout << std::endl;

	// Iterate through digits of pi using an iterator
	std::cout << "pi:  3.";
	auto pi = std::vector<int>{1, 4, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};
	for(auto it : ez::iterate(pi)) {
		std::cout << *it;
	}
	std::cout << std::endl;

	// Iterate through digits of e using the index
	std::cout << "e:   2.";
	auto e = std::vector<int>{7, 1, 8, 2, 8, 1, 8, 2, 8, 4, 5, 9, 0};
	for(auto index : ez::indices(e)) {
		std::cout << e[index];
	}
	std::cout << std::endl;
}
