#include "ez/interval.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int main() {
	auto const limit = 20;
	std::vector<int> seq;
	seq.push_back(1);

	std::cout << "The first " << limit << " values of Conway's Look-And-Say sequence\n";
	for(auto x : ez::make_interval[1][limit]) {
		std::cout << x << ": ";
		std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>(std::cout));
		std::cout << "\n";

		std::vector<int> next_seq;
		for(auto current = std::begin(seq), end = std::end(seq); current != end;) {
			using namespace std::placeholders;
			auto const next =
			  std::find_if_not(current, end, std::bind(std::equal_to<int>(), *current, _1));
			next_seq.push_back(next - current);
			next_seq.push_back(*current);
			current = next;
		}

		seq = next_seq;
	}

	std::cout << std::endl;
}

