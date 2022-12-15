#include <vector> //std::vector
#include "PQ/IPQV.hpp" //IPQV
#include "PRIM.hpp" //OutgoingEdgesList

#include <iostream>


OutgoingEdgesList PrimMST(const OutgoingEdgesList& oel) {
	std::vector<bool> connected(oel.size(), false);
	OutgoingEdgesList ret(oel.size());
	//IPQ for lazy Prim implementation, edge-to index, edge-from value
	IPQV<size_t, double, size_t> ipqv(oel.size()); 
	if (!connected.size()) return OutgoingEdgesList(0);
	connected[0] = true; 
	for (const auto& pr : oel[0]) if(pr.second) ipqv.addConditional(pr.second, pr.first, 0);
	//Greedy heap loop to find mst edges
	while (!ipqv.empty()) {
		auto trio = ipqv.pollTop();
		connected[trio.first.first] = true;
		ret[trio.second].push_back({trio.first.second, trio.first.first});
		for (const auto& pr : oel[trio.first.first]) {
			if (connected[pr.second]) continue;
			ipqv.addConditional(pr.second, pr.first, trio.first.first);
		}
	} return ret;
}
