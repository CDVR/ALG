#include <stack> //std::stack
#include "PQ/PQ.hpp" //PriorityQueue
#include "HFMN.hpp" //Huffmap, byte, std::string, std::unordered_map, std::vector


struct HuffmanNode {
	HuffmanNode *lc, *rc;
	byte content;
	HuffmanNode(const byte& b) : 
	lc(nullptr), rc(nullptr), content(b) {}
	HuffmanNode(HuffmanNode* l, HuffmanNode* r, const byte& b) : 
	lc(l), rc(r), content(b) {}
};

HuffMap huffmanCoding(const std::string& str) {
	std::unordered_map<byte, size_t> hmap(255);
	//Calculate amount of bytes into map
	for (const auto& cr : str) {
		if (hmap.find(cr) == hmap.end()) hmap[cr] = 1;
		else hmap[cr]++;
	} //Error checking if someone inserted empty string 
	if (hmap.size() == 0) return HuffMap(0);
	if (hmap.size() == 1) {
		HuffMap hfr(1); hfr[hmap.begin()->first] = std::vector<bool>(1, false); 
		return hfr;
	} //Transform to huffman nodes and add them to pq
	size_t nodeCount = hmap.size();
	PriorityQueue<HuffmanNode*, size_t> pq;
	for (const auto& elem : hmap) pq.add(new HuffmanNode(elem.first), elem.second);
	//Create Huffman tree taking 2 lowest nodes in pq and merging them exp. information theory
	while (pq.size() > 1) {
		auto low = pq.pollTop();
		auto high = pq.pollTop();
		pq.add(new HuffmanNode(low.first, high.first, '\0'), low.second + high.second);
	}//DFS iteration over huffman tree to find codes for nodes and delete tree
	HuffMap huffmap(nodeCount); std::vector<bool> direction;
	std::stack<HuffmanNode*> nstk; nstk.push(pq.pollTop().first); 
	while (!nstk.empty()) {
		//Move the most down prefering left
		while (nstk.top()->lc || nstk.top()->rc) {
			if (nstk.top()->lc) { //Go left
				direction.push_back(false); nstk.push(nstk.top()->lc);	
			} else { //Go right if left does not exist
				direction.push_back(true); nstk.push(nstk.top()->rc);
			} //Insert entry into table
		} huffmap[nstk.top()->content] = direction;
		auto prev = nstk.top(); //Backtrack upwards until node still has children
		for (nstk.pop(); !nstk.empty(); prev = nstk.top(), nstk.pop()) {
			direction.pop_back(); delete prev; 
			(nstk.top()->lc == prev) ? nstk.top()->lc = nullptr : nstk.top()->rc = nullptr;
			//Check if node has children
			if (nstk.top()->lc != nullptr || nstk.top()->rc != nullptr) break;
		} if (nstk.empty()) delete prev;
	} return huffmap;
}
