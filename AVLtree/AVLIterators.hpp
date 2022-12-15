#pragma once
#include <stack>

#define L_CHLD 0
#define R_CHLD 1

namespace duo{
	template<typename A, typename B>
	class duo {
	public:
		A first; B second;
		duo(const A& f, const B& s) : first(f), second(s) {}
		duo(const B& s) : second(s) {}
	};
}

template <typename AVL>
class AVLInorderIterator {
private: 
	using iterator_category = std::forward_iterator_tag;
	using Node = typename AVL::Node;
	using Key = typename AVL::Type1;
	using Value = typename AVL::Type2;
    using reference = typename duo::duo<Key, Value*>;
    using pointer = Value*;
protected:	
	std::stack<Node*> m_stack;
protected:
	AVLInorderIterator(){}	
public:
    AVLInorderIterator& operator++() { 
    	if (m_stack.empty()) return *this;
    	//Extracting current node and poping it from stack so it wont repeat in iteration
    	Node* cur = m_stack.top(); m_stack.pop();
    	//Going to its right successor
    	Node* right = cur->next[R_CHLD];
    	//Deletion if last owner
    	cur->its_on_it--;
    	if (!cur->its_on_it && cur->removed) delete cur;
    	//Going the most left
    	while(right != nullptr) {
    		//Claiming ownership
    		m_stack.push(right);
    		right->its_on_it++;
    		//Going more left if possible
    		right = right->next[L_CHLD];
    	} return *this;
    }
public:
    AVLInorderIterator(Node* ptr) {
    	while(ptr != nullptr) {m_stack.push(ptr); ptr->its_on_it++; ptr = ptr->next[L_CHLD];}
    }    
public:
    reference operator*() const {
    	if (!m_stack.empty()) return {m_stack.top()->key, &m_stack.top()->value}; 
    	return reference(nullptr);
    }
    pointer operator->() const {
    	if (!m_stack.empty()) return &(m_stack.top()->value); 
    	return nullptr;
    }
    AVLInorderIterator operator++(int) {
    	AVLInorderIterator tmp = *this; ++(*this); return tmp;
    }
    bool operator== (const AVLInorderIterator& b) {
    	if (m_stack.empty() && b.m_stack.empty()) return true;
    	if (!m_stack.empty() && !b.m_stack.empty()) return m_stack.top() == b.m_stack.top();
    	return false;
    };
    bool operator!= (const AVLInorderIterator& b) {return !((*this) == b);};
    operator bool() const {return !m_stack.empty();}
public:
	~AVLInorderIterator() {
		while (!m_stack.empty()) {
			//Clearing the stack and deleting the pointers to nodes if they are their last owners
			Node* todel = m_stack.top(); m_stack.pop(); 
			todel->its_on_it--;
			if (!todel->its_on_it && todel->removed) delete todel;
		}
	}
};

template <typename AVL>
class AVLReverseInorderIterator : public AVLInorderIterator<AVL> {
private: 
	using AVLInorderIterator<AVL>::m_stack;
	using iterator_category = std::forward_iterator_tag;
	using Node = typename AVL::Node;

public:
    AVLReverseInorderIterator& operator++() {
    	if (m_stack.empty()) return *this;
    	//Extracting current node and poping it from stack so it wont repeat in iteration
    	Node* cur = m_stack.top(); m_stack.pop();
    	//Going to its left successor
    	Node* left = cur->next[L_CHLD];
    	//Deletion if last owner
    	cur->its_on_it--;
    	if (!cur->its_on_it && cur->removed) delete cur;
    	//Going the most right
    	while(left != nullptr) {
    		//Claiming ownership
    		m_stack.push(left);
    		left->its_on_it++;
    		//Going more right if possible
    		left = left->next[R_CHLD];
    	} return *this;
    }	
public:
    AVLReverseInorderIterator(Node* ptr) {
    	while(ptr != nullptr) {m_stack.push(ptr); ptr->its_on_it++; ptr = ptr->next[R_CHLD];}
    }    
public:
    AVLReverseInorderIterator operator++(int) {
    	AVLReverseInorderIterator tmp = *this; ++(*this); return tmp;
    }
};

template <typename AVL>
class AVLPreOrderIterator : public AVLInorderIterator<AVL> {
private: 
	using AVLInorderIterator<AVL>::m_stack;
	using iterator_category = std::forward_iterator_tag;
	using Node = typename AVL::Node;
public:
    AVLPreOrderIterator& operator++() {
    	if (m_stack.empty()) return *this;
    	//Extracting current node and poping it from stack so it wont repeat in iteration
    	Node* cur = m_stack.top(); m_stack.pop();
    	//Building pathway for left and right, similiar to DFS, claiming of node ownership
    	if (cur->next[R_CHLD]) {m_stack.push(cur->next[R_CHLD]); cur->next[R_CHLD]->its_on_it++;}
    	if (cur->next[L_CHLD]) {m_stack.push(cur->next[L_CHLD]); cur->next[L_CHLD]->its_on_it++;}
    	//Deleting if last owner
    	cur->its_on_it--;
    	if (!cur->its_on_it && cur->removed) delete cur;  
    	return *this;
    }	
public:
    AVLPreOrderIterator(Node* ptr) {
    	if (ptr) {m_stack.push(ptr); ptr->its_on_it++;}
    }    
public:
    AVLPreOrderIterator operator++(int) {
    	AVLPreOrderIterator tmp = *this; ++(*this); return tmp;
    }
};

template <typename AVL>
class AVLReversePreOrderIterator : public AVLInorderIterator<AVL> {
private: 
	using AVLInorderIterator<AVL>::m_stack;
	using iterator_category = std::forward_iterator_tag;
	using Node = typename AVL::Node;
public:
    AVLReversePreOrderIterator& operator++() {
    	if (m_stack.empty()) return *this;
    	//Extracting current element and poping it from stack so it wont repeat in iteration
    	Node* cur = m_stack.top(); m_stack.pop();
    	//Building pathway for right and left, similiar to DFS nut reverse, claiming of node ownership
    	if (cur->next[L_CHLD]) {m_stack.push(cur->next[L_CHLD]); cur->next[L_CHLD]->its_on_it++;} 
    	if (cur->next[R_CHLD]) {m_stack.push(cur->next[R_CHLD]); cur->next[R_CHLD]->its_on_it++;}
    	//Deleting if last owner
    	cur->its_on_it--;
    	if (!cur->its_on_it && cur->removed) delete cur; 
    	return *this;
    }	
public:
    AVLReversePreOrderIterator(Node* ptr) {
    	if (ptr) {m_stack.push(ptr); ptr->its_on_it++;}
    }    
public:
    AVLReversePreOrderIterator operator++(int) {
    	AVLReversePreOrderIterator tmp = *this; ++(*this); return tmp;
    }
};

template <typename AVL>
class AVLPostOrderIterator : public AVLInorderIterator<AVL> {
private: 
	using AVLInorderIterator<AVL>::m_stack;
	using iterator_category = std::forward_iterator_tag;
	using Node = typename AVL::Node;
public:
    AVLPostOrderIterator& operator++() {
    	if (m_stack.empty()) return *this;
    	//Extracting previous element and poping it from stack so it wont repeat in iteration
    	Node* prev = m_stack.top(); m_stack.pop();   	
		if (!m_stack.empty()) {
	    	//Taking current element
	    	Node* now = m_stack.top();
	    	//Checking where iterator came from
	    	if (prev == now->next[L_CHLD]) {
	    		now = now->next[R_CHLD];
	    		//Iterating all the way down prefering left first
				while(now != nullptr) {
					m_stack.push(now); 
					//Claiming node ownership
					now->its_on_it++;
					if (now->next[L_CHLD]) now = now->next[L_CHLD];
					else now = now->next[R_CHLD];
				}
	    	}
	    } //Deleting prev if last owner
	    prev->its_on_it--; 
	    if (!prev->its_on_it && prev->removed) delete prev;
		return *this;
    }	
public:
    AVLPostOrderIterator(Node* ptr) {
    	while(ptr != nullptr) {
			m_stack.push(ptr); 
			ptr->its_on_it++;
			if (ptr->next[L_CHLD]) ptr = ptr->next[L_CHLD];
			else ptr = ptr->next[R_CHLD];
		}
    }    
public:
    AVLPostOrderIterator operator++(int) {
    	AVLPostOrderIterator tmp = *this; ++(*this); return tmp;
    }
};

template <typename AVL>
class AVLReversePostOrderIterator : public AVLInorderIterator<AVL> {
private: 
	using AVLInorderIterator<AVL>::m_stack;
	using iterator_category = std::forward_iterator_tag;
	using Node = typename AVL::Node;
public:
    AVLReversePostOrderIterator& operator++() {
    	if (m_stack.empty()) return *this;
    	//Extracting previous element and poping it from stack so it wont repeat in iteration
    	Node* prev = m_stack.top(); m_stack.pop();
		if (!m_stack.empty()) {
			//Taking current element
	    	Node* now = m_stack.top();
	    	//Checking where iterator came from
	    	if (prev == now->next[R_CHLD]) {
	    		now = now->next[L_CHLD];
	    		//Iterating all the way down prefering right first
				while(now != nullptr) {
					m_stack.push(now); 
					//Claiming node ownership
					now->its_on_it++;
					if (now->next[R_CHLD]) now = now->next[R_CHLD];
					else now = now->next[L_CHLD];
				}
	    	}
	    } //Deleting prev if last owner
	    prev->its_on_it--;
	    if (!prev->its_on_it && prev->removed) delete prev;
		return *this;
    }	
public:
    AVLReversePostOrderIterator(Node* ptr) {
    	while(ptr != nullptr) {
			m_stack.push(ptr); 
			ptr->its_on_it++;
			if (ptr->next[R_CHLD]) ptr = ptr->next[R_CHLD];
			else ptr = ptr->next[L_CHLD];
		}
    }    
public:
    AVLReversePostOrderIterator operator++(int) {
    	AVLReversePostOrderIterator tmp = *this; ++(*this); return tmp;
    }
};
