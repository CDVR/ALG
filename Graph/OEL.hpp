#pragma once
#include <vector> //std::vector
#include <initializer_list> //std::initializer_list

using OutgoingEdge = std::vector<std::pair<double, size_t>>;


class OutgoingEdgesList {
private: //Data
	OutgoingEdge* m_data;
	size_t m_size;
public: //Methods
	OutgoingEdge& operator[](const size_t& idx) {return *(m_data + idx);}
	const OutgoingEdge& operator[](const size_t& idx) const {return *(m_data + idx);}
	size_t size() const {return m_size;}
	OutgoingEdgesList& operator=(const OutgoingEdgesList& b) {
		m_size = b.m_size; delete[] m_data; m_data = new OutgoingEdge[m_size]; 
		for (auto *i = m_data, *j = b.m_data; i < m_data + m_size; ++i, ++j) *i = *j;
		return (*this);
	}
public: //Constructors / destructors
	~OutgoingEdgesList() {delete[] m_data;}
	OutgoingEdgesList(const size_t& size) : m_size(size) {
		m_data = new OutgoingEdge[m_size];
		for (size_t i = 0; i < m_size; ++i) m_data[i] = OutgoingEdge();
	}
	OutgoingEdgesList(const OutgoingEdgesList& b) : OutgoingEdgesList(b.m_size) {
		for (size_t i = 0; i < m_size; ++i) *(m_data + i) = *(b.m_data + i);
	}
	OutgoingEdgesList(const std::initializer_list<OutgoingEdge>& list) : OutgoingEdgesList(list.size()) {
		size_t idx = 0;
		for (const auto& vec : list) {*(m_data + idx) = vec; ++idx;}
	}
};
