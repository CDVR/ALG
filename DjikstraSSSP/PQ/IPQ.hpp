#pragma once
#include <algorithm> //std::swap
#include <unordered_map> //std::unordered_map
#include <vector> //std::vector, std::pair
#include <initializer_list> //std::initializer_list
#include <cstddef> //size_t


template <typename Idx, typename Prio>
class IPQ {

public: //Renaming stuff
	using Duo = std::pair<Idx, Prio>;
	using Heap = std::vector<Duo>;
	using Htable = std::unordered_map<Idx, size_t>;

public: //Print function
	template <typename IPQ>
	friend void printQueue(const IPQ& ipq); 

private: //Fields
	size_t m_size;
	const bool m_minHeap;
	Heap m_heap;
	Htable m_table;

private: //Helper methods
	void m_swap(const size_t& posA, const size_t& posB) {
		//Swap heap indexes in hashmap
		m_table[m_heap[posA].first] = posB;
		m_table[m_heap[posB].first] = posA;
		//Swap heap values nad indexes in heap
		std::swap(m_heap[posA], m_heap[posB]);
	}

	void m_rebalanceDown(size_t pos) {
		//Loop rebalancing with children as long as both children exist
		while ((2*pos + 2) < m_heap.size()) {
			//Left children check and rebalance if
			if ((m_heap[2*pos + 1].second > m_heap[pos].second) ^ m_minHeap 
			&& (m_heap[2*pos + 1].second > m_heap[2*pos + 2].second) ^ m_minHeap) {
				m_swap(pos, 2*pos + 1); pos = 2*pos + 1;
			} //Right children check and rebalance if
			else if ((m_heap[2*pos + 2].second > m_heap[pos].second) ^ m_minHeap) {
				m_swap(pos, 2*pos + 2); pos = 2*pos + 2;
			} else return; //Early exit if both children are already worse candidate then parent
		} //If right child does not exist we still have to check left
		if ((2*pos + 1) < m_heap.size() && (m_heap[2*pos + 1].second > m_heap[pos].second) ^ m_minHeap) m_swap(pos, 2*pos + 1);
	}

	void m_rebalanceUp(size_t pos) {
		//Early return if elemejnt is already root
		if (!pos) return;
		//Loop iterating up until current element is root
		for (; pos; pos = (pos - 1)/2) {
			//Swapping with parent if better candidate then parent
			if ((m_heap[(pos - 1)/2].second < m_heap[pos].second) ^ m_minHeap)
				m_swap(pos, (pos - 1)/2); 
			//Else no need to rebalance further
			else return;			
		}
	}

public: //Methods
	Duo pollTop() {
		//If the top does not exist return uninitialized variable
		if (!m_size) return Duo();
		m_size--; //Save the old value
		Duo saved = m_heap[0];
		//Swapping with last element in heap
		m_swap(0, m_heap.size() - 1);
		//Removing the last element from table and heap
		m_table.erase(m_heap.back().first);
		m_heap.pop_back();
		//If the removed element was not last we need to rebalance
		if (m_size) m_rebalanceDown(0);
		return saved;
	}

	Prio getPriority(const Idx& idx) {
		//Getting the iterator to the presumed index
		auto it = m_table.find(idx);
		//If the index does not exist in table return empty variable
		if (it == m_table.end()) return Prio();
		return m_heap[it->second].second;
	}

	void add(const Idx& idx, const Prio& prio) {
		//Getting the iterator to the presumed index
		auto it = m_table.find(idx);
		//Check the iterator if the element exists
		if (it == m_table.end()) {
			//Add it to the table and heap
			m_table[idx] = m_heap.size();
			m_heap.push_back({idx, prio});
			//Rebalance up frop the last element we just added
			m_rebalanceUp(m_heap.size() - 1);
			//If table bucket count is too small rehash the table
			m_size++; if (m_table.bucket_count() <= m_size) 
				m_table.rehash(m_size*3);
			return;
		} //Replace the value in heap using table to find it in heap 
		Prio prev = m_heap[it->second].second;
		m_heap[it->second].second = prio;
		//Rebalance down if the new value is worse candidate or down
		if ((prev > prio) ^ m_minHeap) m_rebalanceDown(it->second);
		else m_rebalanceUp(it->second);
	}

	bool addConditional(const Idx& idx, const Prio& prio) {
		//Getting the iterator to the presumed index
		auto it = m_table.find(idx);
		//Check the iterator if the element exists
		if (it == m_table.end()) {
			//Add it to the table and heap
			m_table[idx] = m_heap.size();
			m_heap.push_back({idx, prio});
			//Rebalance up frop the last element we just added
			m_rebalanceUp(m_heap.size() - 1);
			//If table bucket count is too small rehash the table 
			m_size++; if (m_table.bucket_count() <= m_size) m_table.rehash(m_size*3);
			return true; 
		} //If value of found index is already better candidate return
		if ((m_heap[it->second].second > prio) ^ m_minHeap) return false;
		//Replace the value in heap using table to find it in heap
		m_heap[it->second].second = prio;
		//Rebalance up because it can be only better candidate not worse
		m_rebalanceUp(it->second);
		return true;
	}

	bool deleteEntry(const Idx& idx) {
		//Early return if element with given id does not exits
		if (m_table.find(idx) == m_table.end()) return false;
		m_size--; //Swapping with last element in heap
		size_t pos = m_table[idx]; 
		m_swap(pos, m_heap.size() - 1);
		//Removing the last element from table and heap
		m_table.erase(m_heap.back().first);
		m_heap.pop_back();
		//If the removed element was not last we need to rebalance
		if (m_size) m_rebalanceDown(pos);
		return true;
	}
	
	bool empty() {return m_size == 0;}
	Duo peekTop() {if (!m_size) return Duo(); return m_heap[0];}	
	size_t size() {return m_size;}

public: //Constructor / destructor
	IPQ(const size_t& bcnt, const bool& minheap = true) : 
		m_size(0), m_minHeap(minheap), m_heap(Heap()), m_table(bcnt) {}

	IPQ(const std::initializer_list<std::pair<Idx, Prio>>& list) : IPQ(list.size()*3) {
		for (const auto& pair : list) addConditional(pair.first, pair.second);
	}

};
