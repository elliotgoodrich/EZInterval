#include "ez/interval.hpp"

#include <array>
#include <cstddef>
#include <iostream>

int main() {
	auto const limit = 100;
	std::array<bool, limit> is_prime;
	is_prime.fill(true);

	is_prime[0] = false;
	is_prime[1] = false;

	std::cout << "Primes under " << limit << ":\n";
	for(auto&& i : ez::make_interval[2](limit)) {
		if(is_prime[i]) {
			for(int n = 2; n < limit / i; ++n) {
				is_prime[i * n] = false;
			}
			std::cout << i << ", ";
		}
	}

	std::cout << '\n';
}
