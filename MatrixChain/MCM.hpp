#pragma once
#include <vector> //std::vector, std::pair
#include <cstddef> //size_t
template <typename A, typename B> struct Duo {A a; B b;};
using IdealBrackets = std::vector<Duo<size_t, size_t>>;
using Matrices = IdealBrackets;
using IdealBracketsOperations = Duo<IdealBrackets, size_t>; 
IdealBracketsOperations idealChainMatrixMultiplication(const Matrices& matrices);