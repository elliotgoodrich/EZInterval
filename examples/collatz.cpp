#include "ez/interval.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

int main() {
	auto next_collatz = [](int n){ return (n % 2) ? 3 * n + 1 : n / 2; };
	auto stopping_time = [next_collatz](int n){
		int i = 0;
		while(n != 1) {
			n = next_collatz(n);
			++i;
		}
		return i;
	};

	std::cout << "Stopping time for the Collatz conjecture\n";
	std::transform(ez::make_direct_iterator(1),
	               ez::make_direct_iterator(101),
	               std::ostream_iterator<int>(std::cout, ", "),
	               stopping_time);

	std::cout << std::endl;
}
