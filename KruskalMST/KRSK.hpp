#pragma once
#include <vector>

template <typename A, typename B, typename C>
struct Trio {
	A a; B b; C c;
	Trio(){}
	Trio(const A& a, const B& b, const C& c) : 
	a(a), b(b), c(c) {}
};

using UnorderedEdgeList = std::vector<Trio<size_t, double, size_t>>;

UnorderedEdgeList KruskalMST(UnorderedEdgeList edges, const size_t& nodeRange);