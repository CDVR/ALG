#include <type_traits> //static_assert
#include <algorithm> //std::minmax_element, std::fill, std::copy
#include <limits> //std::numeric_limits::is_integer


template <typename T, typename RandIt>
void distributionCS(const RandIt& begin, const RandIt& end) {
	//Type and bounds checking for early return
	static_assert(std::numeric_limits<T>::is_integer);
	if (begin == end || (begin + 1) == end) return;
	//Finding range of elements
	auto [min, max] = std::minmax_element(begin, end);
	size_t range = (*max) - (*min) + 1;
	//Filling occurance array with sum of prev
	T* occ = new T[range]; std::fill(occ, occ + range, 0);
	for (auto it = begin; it != end; ++it) occ[(*it) - (*min)]++;
	for (T* i = occ + 1; i != occ + range; ++i) (*i) += *(i - 1);
	//Transforming the original collection to new sorted one
	T* res = new T[end - begin];
	for (auto it = begin; it != end; ++it) res[--occ[(*it) - (*min)]] = (*it);
	delete[] occ; //Copying the new collection to original one	
	std::copy(res, res + (end - begin), begin);
	delete[] res;
}
