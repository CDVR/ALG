#pragma once
#include <stack> //std::stack
#include <cmath> //std::abs
#include "AVLIterators.hpp" //iterators

#define L_CHLD 0
#define R_CHLD 1


template <typename Key, typename Value>
class AVLtree {	

public: //Node container
	typedef struct Node {
		//KV data stored in node
		Key key; Value value;
		//Iterator memory safety
		long its_on_it; bool removed;
		//Position in tree - right 1, left 0
		int balance; Node* next[2]; 
		//Constructor for creating new node with empty value
		Node(const Key& key) : key(key), its_on_it(0), removed(false), balance(0) 
		{next[R_CHLD] = next[L_CHLD] = nullptr;} 
	} Node; 

public: ///Iterators type export 
	using Type1 = Key;
	using Type2 = Value;

private: ///Iterators type import
	using InorderIterator = AVLInorderIterator<AVLtree<Key, Value>>;
	using ReverseInorderIterator = AVLReverseInorderIterator<AVLtree<Key, Value>>;
	using PreOrderIterator = AVLPreOrderIterator<AVLtree<Key, Value>>;
	using ReversePreOrderIterator = AVLReversePreOrderIterator<AVLtree<Key, Value>>;
	using PostOrderIterator = AVLPostOrderIterator<AVLtree<Key, Value>>;
	using ReversePostOrderIterator = AVLReversePostOrderIterator<AVLtree<Key, Value>>;

public: //Inorder Iterators
	InorderIterator begin() {return InorderIterator(m_root);}
	InorderIterator end() {return InorderIterator(nullptr);}
	ReverseInorderIterator rbegin() {return ReverseInorderIterator(m_root);}
	ReverseInorderIterator rend() {return ReverseInorderIterator(nullptr);}

public: //Preorder Iterators
	PreOrderIterator preBegin() {return PreOrderIterator(m_root);}
	PreOrderIterator preEnd() {return PreOrderIterator(nullptr);}
	ReversePreOrderIterator rpreBegin() {return ReversePreOrderIterator(m_root);}
	ReversePreOrderIterator rpreEnd() {return ReversePreOrderIterator(nullptr);}

public: //PostOrder Iterators
	PostOrderIterator postBegin() {return PostOrderIterator(m_root);}
	PostOrderIterator postEnd() {return PostOrderIterator(nullptr);}
	ReversePostOrderIterator rpostBegin() {return ReversePostOrderIterator(m_root);}
	ReversePostOrderIterator rpostEnd() {return ReversePostOrderIterator(nullptr);}	

public: //Fields
	std::stack<Node*> m_stack;
	Node* m_root;
	long m_size;

private: //Helper find method
	bool m_find(const Key& key) {
		//Iterating down the tree
		for (Node* cur = m_root; cur != nullptr; cur = cur->next[key > cur->key]){		 
			//Keeping track of the path in stack for deletion
			m_stack.push(cur);
			//Premature return when key is found
			if (cur->key == key) return true;
		} return false;
	}

private: //Helper rotations methods
	Node* m_rotateRRLL(Node* node, bool right = true) { 
		//Preforming AVL 1 step RR (true) / LL (false) rotaion by relinking nodes
		Node* newTop = node->next[!right];
		node->next[!right] = newTop->next[right];
		newTop->next[right] = node;
		//Changing node balances based on revious balances of new top
		if (newTop->balance != 0) newTop->balance = node->balance = 0;
		else newTop->balance = -1*(node->balance = right ? -1 : 1);
		//Returning newly created top node after the rotation
		return newTop;
	}

	Node* m_rotateLRRL(Node* node, bool right = true) {
		//Preforming AVL 2 step LR (true) / RL (false) rotaion by relinking nodes
		Node* newTop = node->next[!right]->next[right];
		node->next[!right]->next[right] = newTop->next[!right];
		newTop->next[!right] = node->next[!right];
		node->next[!right] = newTop->next[right];
		newTop->next[right] = node;
		//Changing node balances based on revious balances of new top
		if (newTop->balance == 0) newTop->next[!right]->balance = newTop->next[right]->balance = 0;
		else newTop->next[L_CHLD]->balance = -1*(!(newTop->next[R_CHLD]->balance = (newTop->balance == -1) ? 1 : 0));
		newTop->balance = 0;
		//Returning newly created top node after the rotation
		return newTop;
	}

	void m_handleRotate() {
		//Extracting top node from stack
		Node *rot, *torot = m_stack.top(); m_stack.pop(); 
		//Checking which AVL rotation to preform on it 
		if (torot->balance == 2 && torot->next[R_CHLD]->balance >= 0) rot = m_rotateRRLL(torot, false);
		else if (torot->balance == 2 && torot->next[R_CHLD]->balance < 0) rot = m_rotateLRRL(torot, false);
		else if (torot->balance == -2 && torot->next[L_CHLD]->balance <= 0) rot = m_rotateRRLL(torot);
		else rot = m_rotateLRRL(torot);
		//Linking top node after rotation to parent 
		if (m_stack.empty()) m_root = rot;
		else m_stack.top()->next[m_stack.top()->next[R_CHLD] == torot] = rot;
		//Returing it back to stack 
		m_stack.push(rot);
	}

private: //Helper methods for deleting node
	void m_rebalanceDelete() {
		m_size--; //Balance checking loop iterating stack backwards
		for (Node* prev = nullptr; !m_stack.empty(); prev = m_stack.top(), m_stack.pop()) {
			//Calculating new balance based of which side did addition impact
			if (prev) m_stack.top()->balance += (m_stack.top()->next[R_CHLD] == prev) ? -1 : 1;
			//Checking weather node is balanced meaning it lowered its depth and we have to balance further
			if (m_stack.top()->balance == 0) continue;
			//Checking weather node is inbalanced meaning we have to preform AVL rotation
			if (std::abs(m_stack.top()->balance) == 2) m_handleRotate();
			//Checking if the node is slightly inbalanced (even after rogtation) meanig it did not lower its depth and we can stop
			if (std::abs(m_stack.top()->balance) == 1) return;
		}
	}

	void m_swapToLeaf(Node* swap) {
		//No need to swap
		if (swap->next[R_CHLD] == nullptr && swap->next[L_CHLD] == nullptr) return;
		//Still has children, we need to swap
		bool side = swap->next[R_CHLD] != nullptr; 
		for (Node* cur = swap->next[side]; cur != nullptr; cur = cur->next[!side]) m_stack.push(cur);
		//Swapping values with node to be deleted
		swap->value = m_stack.top()->value; 
		swap->key = m_stack.top()->key;
		if (m_stack.top()->next[side]) {
			//If direct replacable child is not leaf we need to replace it too
			m_stack.top()->value = m_stack.top()->next[side]->value; 
			m_stack.top()->key = m_stack.top()->next[side]->key;
			m_stack.push(m_stack.top()->next[side]);
		}
	}

private: //Helper methods for adding node
	Node* m_rebalanceAdd(Node* added) {
		m_size++; //Premature return when tree is empty, just assigning it to root
		if (m_stack.empty()) {m_root = added; return added;}
		//Configuring direct parent of added and giving him the new child  
		m_stack.top()->balance += (m_stack.top()->key < added->key) ? 1 : -1;
		m_stack.top()->next[m_stack.top()->key < added->key] = added;
		//Balance checking loop iterating stack backwards
		for (Node* prev = nullptr; !m_stack.empty(); prev = m_stack.top(), m_stack.pop()) {	
			//Calculating new balance based of which side did addition impact
			if (prev) m_stack.top()->balance += (m_stack.top()->next[R_CHLD] == prev) ? 1 : -1;
			//Checking weather node is balanced for exiting balance checking or continuing balance checking
			if (m_stack.top()->balance == 0) return added;
			if (std::abs(m_stack.top()->balance) != 2) continue;
			//Node is not balanced, balance it with AVL rotation and stop balance checking
			m_handleRotate(); return added;
		} return added;
	}

public: //Methods
	Value& operator[](const Key& key) { 
		Node* ret;
		//Found, return its reference
		if (m_find(key)) ret = m_stack.top(); 
		//Not found, add it and return its reference
		else ret = m_rebalanceAdd(new Node(key)); 
		//Clear stack and return reference to value
		m_stack = std::stack<Node*>(); return ret->value;
	}

	bool deleteEntry(const Key& key) {
		//Key was not find, not deleting enything
		if (!m_find(key)) {m_stack = std::stack<Node*>(); return false;}
		//Swaps to the most left/right leaf node to be deleted instead of middle node 
		m_swapToLeaf(m_stack.top());
		//Extracting node to delete from stack
		Node* del = m_stack.top(); m_stack.pop();
		//Case when deleted node is the last node
		if (m_stack.empty()) {delete del; m_root = nullptr; return true;} 
		//Case when deleted node had parent
		m_stack.top()->balance += (m_stack.top()->next[R_CHLD] == del) ? -1 : 1;
		m_stack.top()->next[m_stack.top()->next[R_CHLD] == del] = nullptr;
		//Deleting the node
		if (del->its_on_it) del->removed = true;
		else delete del;
		//Rebalancing tree using nodes in stack
		m_rebalanceDelete();
		//Clear stack and return status
		m_stack = std::stack<Node*>(); return true;
	}

	bool find(const Key& key) const {
		//Iterating down the tree without pushing stack
		for (Node* cur = m_root; cur != nullptr; cur = cur->next[key > cur->key])
			if (cur->key == key) return true;
		return false;
	}

	long size() const {return m_size;}

public: //Constructors / destructors
	AVLtree() : m_root(nullptr), m_size(0) {}
	
	AVLtree(const std::initializer_list<std::pair<Key, Value>>& list) : AVLtree() { 
		for (auto pair : list) (*this)[pair.first] = pair.second;
	}

	~AVLtree() {
		//Premature exit when tree is empty
		if (m_root) m_stack.push(m_root);
		//DFS deletion of the tree
		while (!m_stack.empty()) {
			//Extracting node from stack and pushing its children into it
			Node* todel = m_stack.top(); m_stack.pop();
			for (Node* ptr : todel->next) if (ptr) m_stack.push(ptr); 
			//Deleting  extracted node
			if (todel->its_on_it) todel->removed = true;
			else delete todel;
		}
	}

};
