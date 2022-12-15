#pragma once
#include <stack> //sta::stack
#include <vector> //std::vector

#define MIN(A, B) ((A < B) ? A : B)
#define MAX(A, B) ((A > B) ? A : B)


template <typename Key, typename Value, long ORDER>
class Btree {

public: //Containers
	template <typename A, typename B> 
	class duo { //Std::pair with custom constructors
	public:
		A first; 
		B second;
		duo(){}
		duo(const A& a) : first(a) {}
		duo(const A& a, const B& b) : first(a), second(b) {}
	};

	typedef struct Node { //Single B-tree node 
		using KeyPairs = std::vector<duo<Key, Value>>;
		using Pointers = std::vector<Node*>;
		KeyPairs keyPairs;
		Pointers pointers;
		Node(const Key& key) : keyPairs({{key}}), pointers({nullptr, nullptr}) {}
		Node(const duo<Key, Value>& kv, Node* p0, Node* p1) : keyPairs({kv}), pointers({p0, p1}) {}
		Node(const KeyPairs& kp, const Pointers& p) : keyPairs(kp), pointers(p) {}
	} Node; 

private: //Renaming stuff 
	using KeyPairs = typename Node::KeyPairs;
	using Iter = typename KeyPairs::iterator;
	using Pointers = typename Node::Pointers;
	using BLuo = duo<bool, long>;
	using kvNuo = duo<duo<Key, Value>, Node*>;
	using NLuo = duo<Node*, long>;

public: //Print function
	template <typename T, int O>
	friend void printTree(Btree<int, T, O> avl);		

private: //Constands
	const long m_maxNodeSize;
	const long m_leftSplit;
	const long m_minNodeSize;
	
public: //Fields
	std::stack<NLuo> m_stack;
	Node* m_root;
	long m_size; 

private: //Helper methods for finding keyvalue
	BLuo m_bsearch(const Key& key, const Iter& begin, Iter lidx, Iter ridx) const {
		while (ridx > (lidx + 1)) {
			Iter cur = lidx + (ridx - lidx)/2;
			if (key == cur->first) return {true, cur - begin};
			(key > cur->first) ? (lidx = cur) : (ridx = cur);
		} return {false, 1 + lidx - begin};
	}

	bool m_find(const Key& key, BLuo found = {}) {	
		for (Node* cur = m_root; cur != nullptr; cur = cur->pointers[found.second]) {
			found = m_bsearch(key, cur->keyPairs.begin(), cur->keyPairs.begin() - 1, cur->keyPairs.end());
			m_stack.push({cur, found.second});
			if (found.first) return true;
		} return false;
	}

private: //Helper methods for deleting node
	void m_merge(const long& nbridx, const long& meidx) {
		//Getting direct ptr to child, neighbour and parent 
		Node* parent = m_stack.top().first;
		Node* left = m_stack.top().first->pointers[MIN(nbridx, meidx)];
		Node* right = m_stack.top().first->pointers[MAX(nbridx, meidx)];
		//Merge the 2 vectors and KV from parent
		left->keyPairs.push_back(parent->keyPairs[MIN(nbridx, meidx)]);
		left->keyPairs.insert(left->keyPairs.end(), right->keyPairs.begin(), right->keyPairs.end());
		left->pointers.insert(left->pointers.end(), right->pointers.begin(), right->pointers.end());
		//Removing the merged KV and ptr form parent node and deallocating the pointer
		delete parent->pointers[MAX(nbridx, meidx)];
		parent->pointers.erase(parent->pointers.begin() + MAX(nbridx, meidx));
		parent->keyPairs.erase(parent->keyPairs.begin() + MIN(nbridx, meidx));
	}

	void m_borrow(const long& nbridx, const long& meidx) {
		//Getting direct ptr to child, neighbour and parent 
		Node* parent = m_stack.top().first;
		Node* nbr = m_stack.top().first->pointers[nbridx];
		Node* me = m_stack.top().first->pointers[meidx];
		//Copying KV from parent to unbalanced child and ptr from neighbour
		me->keyPairs.insert(me->keyPairs.begin() + ((nbridx > meidx) ? me->keyPairs.size() : 0), 
			parent->keyPairs[MIN(nbridx, meidx)]);
		me->pointers.insert(me->pointers.begin() + ((nbridx > meidx) ? me->pointers.size() : 0), 
			(nbridx > meidx) ? nbr->pointers[0] : nbr->pointers[nbr->pointers.size() - 1]);
		//Replacing parents KV with copy of childs neighbours KV
		parent->keyPairs[MIN(nbridx, meidx)] = nbr->keyPairs[(nbridx > meidx) ? 0 : (nbr->keyPairs.size() - 1)];
		//Deleting that KV copied to parent from neighbour
		nbr->pointers.erase(nbr->pointers.begin() + ((nbridx > meidx) ? 0 : (nbr->pointers.size() - 1)));
		nbr->keyPairs.erase(nbr->keyPairs.begin() + ((nbridx > meidx) ? 0 : (nbr->keyPairs.size() - 1)));			
	}

	void m_rebalanceDelete() {
		for (m_stack.pop(); !m_stack.empty() && //Iterating throught the stack backwards as long as the next node can be unbalanced
			(long)(m_stack.top().first->pointers[m_stack.top().second]->keyPairs.size()) < m_minNodeSize; m_stack.pop()) {
			//Identifying variables for single iteration
			long idx = m_stack.top().second;
			Node* parent = m_stack.top().first; //check these indexes
			//Testing neighbours if weather they can borrow it elements to node
			if (idx > 0 && (long)(parent->pointers[idx - 1]->keyPairs.size()) > m_minNodeSize) {
				m_borrow(idx -1, idx); return;
			} if ((long)(parent->pointers.size()) > (idx + 1) && (long)(parent->pointers[idx + 1]->keyPairs.size()) > m_minNodeSize) {
				m_borrow(idx + 1, idx); return;
			} //Merging with neighbour if no neighbour can borrow it elements
			m_merge(idx + (idx ? -1 : 1), idx);
		} //Checking weather to shrink root after its element might have been decreased
		if (m_root->keyPairs.size()) return;
		//Deleting top node if it is shrinked to 0 elements and replacing with the previous rebalanced node that stayed
		Node* oldroot = m_root; 
		m_root = oldroot->pointers[0];
		delete oldroot;
	}

	long m_deleteLeaf() { 
		m_size--; //If node from which we delte is not leafe we need to swap it with leafs
		if (m_stack.top().first->pointers[0]) {
			//Saving pointer to node to delete from and index of element to delete
			NLuo swap = m_stack.top();	
			//Iterating all the way left of node keys right child pointer 
			for (Node* cur = m_stack.top().first->pointers[++m_stack.top().second]; cur != nullptr; cur = cur->pointers[0])
				m_stack.push({cur, 0}); 
			//Copying the value that will be deleted to lace of value to delete so its kept
			swap.first->keyPairs[swap.second] = m_stack.top().first->keyPairs[0];
		} //Deleting leafs value on index and pointer as they are all null
		m_stack.top().first->pointers.pop_back();
		m_stack.top().first->keyPairs.erase(m_stack.top().first->keyPairs.begin() + m_stack.top().second); 
		//Returing current size of the leaf node to be checked by other method
		return m_stack.top().first->keyPairs.size();
	}

private: //Helper methods for adding node
	kvNuo m_split(const kvNuo& kvn, Value*& val, Key*& key) {
		//Inserting kvn into the vector of kvs and pointers to then split them
		m_insert_into(kvn, val, key);
		//Extracting iterators from stack
		auto kpBeg = m_stack.top().first->keyPairs.begin(); 
		auto ptBeg = m_stack.top().first->pointers.begin();
		//Creating new right trailing node
		Node* trail = new Node(KeyPairs(kpBeg + m_leftSplit + 1, m_stack.top().first->keyPairs.end()), 
			Pointers(ptBeg + m_leftSplit + 1, m_stack.top().first->pointers.end()));
		//Getting the node and pointer to right trail to return
		kvNuo ret = {*(kpBeg + m_leftSplit), trail};
		//Erasing extra kvs and pointers from original node
		m_stack.top().first->keyPairs.erase(kpBeg + m_leftSplit, m_stack.top().first->keyPairs.end());
		m_stack.top().first->pointers.erase(ptBeg + m_leftSplit + 1, m_stack.top().first->pointers.end());
		//If the recieved key is not the one saved in key ptr then return
		if ((*key) != kvn.first.first) return ret; 
		//Reseting or changing pointer to key value based on index in split
		if (m_stack.top().second == m_leftSplit) {val = nullptr; key = nullptr;}
		else if (m_stack.top().second > m_leftSplit) {
			val = &(trail->keyPairs[m_stack.top().second - (m_leftSplit + 1)].second);
			key = &(trail->keyPairs[m_stack.top().second - (m_leftSplit + 1)].first);
		} //Returning generated split kv and pointer their trailing right node 
		return ret;
	}

	void m_insert_into(const kvNuo& kvn, Value*& val, Key*& key) {
		//Insering Keyvalue and its corresponding pointer to precalculated place in node
		m_stack.top().first->keyPairs.insert(m_stack.top().first->keyPairs.begin() + m_stack.top().second, kvn.first); 
		m_stack.top().first->pointers.insert(m_stack.top().first->pointers.begin() + m_stack.top().second + 1, kvn.second); 
		if (!key) { //Setting pointer to value if the original key was still in kvn
			val = &(m_stack.top().first->keyPairs[m_stack.top().second].second); 
			key = &(m_stack.top().first->keyPairs[m_stack.top().second].first);
		}
	}

	void m_splitRoot(const kvNuo& kvn, Value*& val, Key*& key) {
		//Saving old root pointer
		Node* oldroot = m_root;
		//Creating new root from kvn and roots most left ptr left
		m_root = new Node(kvn.first, oldroot, kvn.second); 
		//Setting pointer to value if the original key was still in kvn
		if (!key) {val = &(m_root->keyPairs[0].second); key = &(m_root->keyPairs[0].first);}
	}

	Value& m_insert(kvNuo kvn) {
		m_size++; //Pointer to value which's reference will be be returned
		Value* val = nullptr; Key* key = nullptr;
		//Iterating over the tree upwards until spliting is no longer needed
		for (; !m_stack.empty() && (long)m_stack.top().first->keyPairs.size() == m_maxNodeSize; m_stack.pop()) 
			kvn = m_split(kvn, val, key);
		//If stack was empty that means we have split root and have to create new root from the upwards going kv
		if (m_stack.empty()) m_splitRoot(kvn, val, key);
		//We insert the upwards going key into the current node in stack onto saved place
		else m_insert_into(kvn, val, key);
		//Returning reference to the inserted key's value
		return *val;
	}

	Value& m_create_root(const Key& key) {m_size++; return (m_root = new Node(key))->keyPairs[0].second;}

public: //Methods
	Value& operator[](const Key& key) {
		Value* ret; //If key is present return reference to its value
		if (m_find(key)) ret = &(m_stack.top().first->keyPairs[m_stack.top().second].second);
		//If there are no nodes create new root node with that key and return reference to its value
		else if (m_stack.empty()) ret = &m_create_root(key);
		//Insert it into already established tree
		else ret = &m_insert({{key}, nullptr});
		m_stack = std::stack<NLuo>(); return *ret;
	}

	bool deleteEntry(const Key& key) {
		bool ret = true; //If key is not found return false
		if (!m_find(key)) ret = false;
		//Else swap with leaf and delete it, if leaf has less then min keys then rebalance
		else if (m_deleteLeaf() < m_minNodeSize) m_rebalanceDelete();
		//Clearing working stack and returning status
		m_stack = std::stack<NLuo>(); return ret;
	}

	bool find(const Key& key) const {
		BLuo found; //Loop to iterate down tree until next node is null
		for (Node* cur = m_root; cur != nullptr; cur = cur->pointers[found.second]) {
			//Finding key / next node to explore using bsearch
			found = m_bsearch(key, cur->keyPairs.begin(), cur->keyPairs.begin() - 1, cur->keyPairs.end());
			//Premature return if found
			if (found.first) return true;
		} return false;
	}

	long size() const {return m_size;}

public: //Constructors and destructors
	Btree() : m_maxNodeSize((ORDER > 2) ? (ORDER - 1) : 2),   
		m_leftSplit((m_maxNodeSize + 1)/2), 
		m_minNodeSize(m_maxNodeSize/2), 
		//Calculating internal constants from given parameter and creating empty tree
		m_root(nullptr), m_size(0) {}

	Btree(const std::initializer_list<std::pair<Key, Value>>& list) : Btree() { 
		//Inserting elements from initializer list
		for (auto pair : list) (*this)[pair.first] = pair.second;
	}

	~Btree() {
		//Premature exit if tree is empty
		if (m_root) m_stack.push(m_root);
		//DFS like deletion of tree
		while (!m_stack.empty()) { //Extracting node from stack
			Node* cur = m_stack.top().first; m_stack.pop();
			//Pushing nodes children onto stack
			for (auto& ptr : cur->pointers) if (ptr) m_stack.push(ptr);
			//Deleting extracted node
			delete cur;
		}
	}

};
