#include "ez/direct_iterator.hpp"

#include <boost/iterator/indirect_iterator.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <typename RandomAccessIt, typename UnaryPredicate, typename Compare>
void sort_selected(RandomAccessIt begin, RandomAccessIt end, UnaryPredicate p, Compare comp) {
	std::vector<RandomAccessIt> positions;
	std::copy_if(ez::make_direct_iterator(begin),
	             ez::make_direct_iterator(end),
	             std::back_inserter(positions),
	             [p](RandomAccessIt it){ return p(*it); });

	std::sort(boost::make_indirect_iterator(std::begin(positions)),
	          boost::make_indirect_iterator(std::end(positions)),
	          comp);
}

int main() {
	auto numbers = std::vector<int>{0, 0, 8, 0, 0, 0, 0, 2, 0, 5, 0, 1, 4, 0, 0, 0, 7};

	// Sort the non-zero elements in place
	sort_selected(std::begin(numbers),
	              std::end(numbers),
	              [](int i){ return i != 0; },
	              std::less<int>());

	std::copy(numbers.cbegin(),
	          numbers.cend(),
	          std::ostream_iterator<int>(std::cout, " "));
}
