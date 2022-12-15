#pragma once
#include <unordered_map> //std::unordered_map
#include <list> //std::list, std::pair
#include <cstddef> //size_t


template <typename Key, typename Value>
class LRUCache {

private: //Inner classes
	struct Node {
		Key key; 
		Value value;
		Node* next;
		Node* prev;
	};

	class iterator {
	public: 
	    using iterator_category = std::bidirectional_iterator_tag;
	    using reference = std::pair<Key, Value&>;
	    using creference = std::pair<Key, const Value&>;
	private:
	    Node* m_ptr;
	public:
	    iterator(Node* ptr) {m_ptr = ptr;}
	    operator bool() const {return m_ptr;}
	    bool operator==(const iterator& it) const {return m_ptr == it.m_ptr;}
	    bool operator!=(const iterator& it) const {return m_ptr != it.m_ptr;}
	    iterator& operator++(){m_ptr = m_ptr->next; return(*this);}
	    iterator& operator--(){m_ptr = m_ptr->prev; return (*this);}
	    iterator operator++(int){auto tmp(*this); m_ptr = m_ptr->next; return tmp;}
	    iterator operator--(int){auto tmp(*this); m_ptr = m_ptr->prev; return tmp;}
	    reference operator*(){reference ret = {m_ptr->key, m_ptr->value}; return ret;}
	    const creference operator*() const {creference ret = {m_ptr->key, m_ptr->value}; return ret;}
	};

private: //Containers
	std::unordered_map<Key, Node*> m_map;
	size_t m_maxSize;
	Node* m_root;
	Node* m_Rroot;

private: //Helper methods
	Node* m_moveToFirst(Node* node) {
		if (!m_root) {
			m_root = m_Rroot = node;
			m_root->next = m_root->prev = nullptr;
		} else {
			m_root->prev = node;
			node->next = m_root;
			node->prev = nullptr;
			m_root = node;
		} return node;
	}

	Node* m_extractNode(Node* node) {
		if (m_root == m_Rroot && m_root == node) 
			m_root = m_Rroot = nullptr;
		else if (m_root == node) {
			m_root = m_root->next;
			m_root->prev = nullptr;
		} else if (m_Rroot == node) {
			m_Rroot = m_Rroot->prev;
			m_Rroot->next = nullptr;
		} else {
			node->next->prev = node->prev;
			node->prev->next = node->next;
		} return node;
	}

	Node* m_addKey(const Key& key) {
		Node* node = new Node;
		node->key = key;
		m_moveToFirst(node);
		m_map[key] = node;
		return node;
	}

public: //Methods
	bool exists(const Key& key) const {
		return m_map.find(key) != m_map.end();
	}

	Value& operator[](const Key& key){
		auto it = m_map.find(key);
		if (it == m_map.end()) {
			if (m_map.size() == m_maxSize) {
				m_map.erase(m_Rroot->key);
				delete m_extractNode(m_Rroot);
			} return m_addKey(key)->value;
		} if (m_root == it->second) return it->second->value;
		return m_moveToFirst(m_extractNode(it->second))->value;
	}

public: //Iterators
	iterator begin() {return iterator(m_root);}
	iterator end() {return iterator(nullptr);}

public: //Constructor / Destructor
	LRUCache(size_t maxSize) : 
		m_map(maxSize ? maxSize : 1), m_maxSize(maxSize ? maxSize : 1), 
		m_root(nullptr), m_Rroot(nullptr) {}
	
	~LRUCache() {
		for (Node* node = m_root; node;) {
			Node* tmp = node;
			node = node->next;
			delete tmp;
		}
	}
};
