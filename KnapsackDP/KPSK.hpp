#include <vector> //std::vector
#include <cstddef> //size_t
using Items = std::vector<std::pair<double, size_t>>;
Items KnapsackDP(const Items& items, const size_t& size);
