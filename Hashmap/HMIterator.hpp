#pragma once
#include <utility>
#include <memory>


template <typename MAP> 
class HashMapIterator {
private: 
	using iterator_category = std::forward_iterator_tag;
	using Key = typename MAP::K;
	using Value = typename MAP::V;
	using reference = typename std::pair<Key, Value&>;
    using BucketIterator = typename MAP::Bucket::iterator;
    using DataIterator = typename MAP::Data::iterator;
private:
	DataIterator m_end;
    DataIterator m_it;
    BucketIterator m_itl;
public:
    HashMapIterator(const DataIterator& it, const DataIterator& end) : m_end(end), m_it(it) {
        for (; m_it != m_end; ++m_it) if (m_it->size()) {m_itl = m_it->begin(); break;}
    }
    HashMapIterator& operator++() { 
        if (m_it == m_end) return *this;
        //Increment list iterator inside bucket
        ++m_itl; if (m_itl != m_it->end()) return *this; 
        //Iterate to next nonempty bucket in hashmap
        for (m_it++; m_it != m_end; m_it++)
            if (m_it->size()) {m_itl = m_it->begin(); break;}
        return *this;
    }
    bool operator== (const HashMapIterator& b) {
        //Check weather both are on end or both not then if not test if they have same list it
        if (m_it == m_end && b.m_it == b.m_end) return m_it == b.m_it;
        else if (m_it != m_end && b.m_it != b.m_end) return m_it == b.m_it && m_itl == b.m_itl;
        return false;
    };    
public:
    reference operator*() const {return {m_itl->first, m_itl->second};}
    HashMapIterator operator++(int) {HashMapIterator tmp = *this; ++(*this); return tmp;}
    bool operator!= (const HashMapIterator& b) {return !((*this) == b);};
    operator bool() const {return m_it == m_end;}
};
