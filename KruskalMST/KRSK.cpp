#include <algorithm> //std::max, std::min, std::sort
#include <stack> //std::stack
#include "KRSK.hpp" //std::vector, Trio, UnorderedEdgeList


class UnsafeDSU {
private: //Index and weight vectors
	std::vector<size_t> m_parents;
	std::vector<size_t> m_sizes;
	size_t m_size;
public: //Methods
	size_t findSet(const size_t& idx) {
		std::stack<size_t> idxes; idxes.push(idx);
		//Find root parent and save children on its way to stack
		for (; m_parents[idxes.top()] != idxes.top(); idxes.push(m_parents[idxes.top()]));
		size_t parent = idxes.top(); idxes.pop();
		//Change parent of children on its way to parnt to root parent
		for (; !idxes.empty(); m_parents[idxes.top()] = parent, idxes.pop());
		return parent;
	}	
	bool unionSets(const size_t& idx1, const size_t& idx2) {
		//Find root parents for both sets, if same ret false
		size_t p1 = findSet(idx1), p2 = findSet(idx2);
		if (p1 == p2) return false;
		//Select higher and lower size one
		size_t max = (m_sizes[p1] > m_sizes[p2]) ? p1 : p2;
		size_t min = (max == p1) ? p2 : p1;
		//Merge smaller size one into higher size one
		m_sizes[max] += m_sizes[min];
		m_parents[min] = max; 
		m_size--; return true;
	}
	size_t size() const {return m_size;}
public: //Constructor / destructor
	UnsafeDSU(const size_t& size) : m_size(size) {
		//Set parents of individual nodes to themselves
		m_parents = std::vector<size_t>(size);
		for (size_t i = 0; i < size; ++i) m_parents[i] = i;
		//Set sizes for all sets to 1
		m_sizes = std::vector<size_t>(size, 1);
	}
};

UnorderedEdgeList KruskalMST(UnorderedEdgeList edges, const size_t& nodeRange) {
	//Sorting edges by edge lenght value 
	std::sort(edges.begin(), edges.end(), [](auto& a, auto& b) {return a.b < b.b;});
	UnorderedEdgeList mst; mst.reserve(nodeRange);
	//Main DSU loop for going through the sorted edges and select mst ones
	UnsafeDSU dsu(nodeRange);	
	for (const auto& edge : edges) {
		//Searching for set parents of given nodes in dsu 
		size_t p1 = dsu.findSet(edge.a); 
		size_t p2 = dsu.findSet(edge.c);
		if (p1 == p2) continue;
		//Insert edge to mst and union sets
		mst.push_back(edge);
		dsu.unionSets(p1, p2);
		if (dsu.size() <= 1) break; 
	} return mst;
}
