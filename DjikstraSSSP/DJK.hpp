#pragma once
#include <cstddef> //size_t
#include <vector> //std::vector
#include "GR/OEL.hpp" //OutgoinEdgesList
std::vector<double> DjikstraSSSP(const OutgoingEdgesList& oel, const size_t& idx);
