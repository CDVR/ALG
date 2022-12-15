#include "GD.hpp" //OutgoingEdgeList, Edges, std::vector, std::list
#include <stack> //std::stack
#include <algorithm> //std::fill


EdgeDependencies graphDependency(const Edges& edges, size_t graphSize) {
	OutgoingEdgesList oel(graphSize, std::list<size_t>());
	for (const auto& edge : edges) oel[edge.from].push_back(edge.to);
	EdgeDependencies ret(graphSize, std::list<size_t>());
	std::vector<bool> in_stack(graphSize);
	for (size_t i = 0; i < graphSize; i++) {
		std::fill(in_stack.begin(), in_stack.end(), false);
		std::stack<size_t> stack;
		stack.push(i); in_stack[i] = true;
		while (!stack.empty()) {
			size_t val = stack.top(); stack.pop();
			ret[val].push_back(i);
			for (const size_t& edge_to : oel[val]) {
				if (!in_stack[edge_to]) {
					stack.push(edge_to);
					in_stack[edge_to] = true;
				}
			} 
		} 
	} return ret;
}
