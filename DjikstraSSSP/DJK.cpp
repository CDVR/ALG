#include <limits> //std::numeric_limits::infinity
#include "DJK.hpp" //OutgoingEdgeList, //std::vector
#include "PQ/IPQ.hpp" //IPQ

using Vec = std::vector<double>;
#define Inf std::numeric_limits<double>::infinity()


Vec DjikstraSSSP(const OutgoingEdgesList& oel, const size_t& idx) {
	Vec ret(oel.size(), Inf);
	//IPQ for lazy Djikstra implementation and adding 1st element
	IPQ<size_t, double> ipq(oel.size());
	if (oel.size()) ipq.add(idx, 0); 
	//Greedy loop to select next most promissing edge from ipq 	
	while (!ipq.empty()) {
		auto top = ipq.pollTop();
		ret[top.first] = top.second;
		for (const auto& pair : oel[top.first]) {
			if (ret[pair.second] != Inf) continue;
			ipq.addConditional(pair.second, pair.first + top.second);
		} //Returning vec filled with shortest paths for all nodes
	} return ret;
}
