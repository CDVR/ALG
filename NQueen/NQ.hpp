#include <vector> //std::vector
#include <cstddef> //size_t

using Solutions = std::vector<std::vector<size_t>>;


Solutions nQueen(const size_t& dim);
Solutions getDistinct(const Solutions& sols);