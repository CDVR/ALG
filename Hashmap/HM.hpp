#pragma once
#include <cstddef> //size_t
#include <vector> //std::vector
#include <list> //std::list
#include "HF.hpp" //Hash functions
#include "HMIterator.hpp" //Iterator


template <typename Key, typename Value>
class HashMap {

public: //Renaming stuff
	using Data = typename std::vector<std::list<std::pair<Key, Value>>>;
	using Bucket = typename std::list<std::pair<Key, Value>>;
	using BucketIndex = typename std::pair<Bucket&, typename Bucket::iterator>;

public: //Iterators type export
	using K = Key;
	using V = Value;

private: //Iterators type import 
	using Iterator = HashMapIterator<HashMap<Key, Value>>;

private: //Fields
	size_t m_size;
	size_t m_bucket_count;
	Data m_data;

public: //Iterators
	Iterator begin() {return Iterator(m_data.begin(), m_data.end());}
	Iterator end() {return Iterator(m_data.end(), m_data.end());}

private: //Helper methods
	size_t hashKey(Key key) {
		return HSH::hash(key)%m_bucket_count;
	}

	BucketIndex m_find(const Key& key) {
		//Get index of hased bucked
		size_t bcktidx = hashKey(key);
		//Searching the bucket for given key
		for (auto it = m_data[bcktidx].begin(); it != m_data[bcktidx].end(); ++it) {
			//Return the bucket and it to the key if found
			if (it->first == key) return {m_data[bcktidx], it};
		} //Return the bucket and its end iterator if not found
		return {m_data[bcktidx], m_data[bcktidx].end()};
	}

public: //Methods
	Value& operator[](const Key& key) {
		//Search for the kv
		BucketIndex fnd = m_find(key);
		if (fnd.second != fnd.first.end()) return fnd.second->second;
		//If not found create new kv and insert it
		m_size++; std::pair<Key, Value> pr;
		pr.first = key;
		fnd.first.push_back({pr});
		//Return reference to the added value from kv
		return fnd.first.back().second;
	}

	bool deleteEntry(const Key& key) {
		//Getting index of bucket and it to the kv
		BucketIndex fnd = m_find(key);
		//Guard return if not found
		if (fnd.second == fnd.first.end()) return false;
		m_size--; //Erase kv if found
		fnd.first.erase(fnd.second);
		return true;
	}

	void rehash(const size_t& bc) {
		//Input sanitization
		if (!bc) return;
		//Saving old data and replacing them with empty new with updated bc
		Data oldData = m_data;
		m_data = Data(m_bucket_count = bc, Bucket());
		m_size = 0; //Copying the data from old data to new data
		for (const auto& bucket : oldData) 
			for (const auto& kv : bucket) (*this)[kv.first] = kv.second;
	}

	bool find(const Key& key) {auto fnd = m_find(key); return (fnd.first.end() != fnd.second);}
	size_t size() {return m_size;}
	size_t bucket_count() {return m_bucket_count;}

public: //Constructors / destructors
	HashMap(const size_t& bc) : m_size(0), m_bucket_count(bc), m_data(Data(bc, Bucket(0))) {}

	HashMap(const std::initializer_list<std::pair<Key, Value>>& list) : HashMap(10*list.size()) {
		for (auto pair : list) (*this)[pair.first] = pair.second;
	}

};
