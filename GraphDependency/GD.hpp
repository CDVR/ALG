#pragma once
#include <vector> //std::vector
#include <list> //std::list
#include <cstddef> //size_t
using OutgoingEdgesList = std::vector<std::list<size_t>>;
struct Edge {size_t from, to;}; 
using Edges = std::list<Edge>;
using EdgeDependencies = OutgoingEdgesList;
EdgeDependencies graphDependency(const Edges& edges, size_t graphSize);