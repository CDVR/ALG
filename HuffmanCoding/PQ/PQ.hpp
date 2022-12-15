#pragma once
#include <algorithm> //std::swap
#include <unordered_map> //std::unordered_map
#include <vector> //std::vector, std::pair
#include <initializer_list> //std::initializer_list


template <typename Value, typename Prio>
class PriorityQueue {

public: //Renaming stuff
	using Duo = std::pair<Value, Prio>;
	using Heap = std::vector<Duo>;

public: //Print function
	template <typename PQ>
	friend void printQueue(const PQ& ipq); 

private: //Fields
	size_t m_size;
	const bool m_minHeap;
	Heap m_heap;

private: //Helper methods
	void m_swap(const size_t& posA, const size_t& posB) {
		//Swap heap values in heap
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
		m_heap.pop_back();
		//If the removed element was not last we need to rebalance
		if (m_size) m_rebalanceDown(0);
		return saved;
	}

	void add(const Value& val, const Prio& prio) {
		//Annd element and rebalance up
		m_size++;
		m_heap.push_back({val, prio});
		m_rebalanceUp(m_heap.size() - 1);
		return;
	}

	bool empty() {return m_size == 0;}
	Duo peekTop() {if (!m_size) return Duo(); return m_heap[0];}	
	size_t size() {return m_size;}

public: //Constructor / destructor
	PriorityQueue(const bool& minheap = true) : 
		m_size(0), m_minHeap(minheap), m_heap(Heap()) {}

	PriorityQueue(const std::initializer_list<std::pair<Value, Prio>>& list) : PriorityQueue(true) {
		for (const auto& pair : list) add(pair.first, pair.second);
	}

};
