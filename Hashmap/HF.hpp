#include <vector> //std::vector, static_cast, std::pair
#include <list> //std::list
#include <map> //std::map
#include <array> //std::array
#include <queue> //std::queue, std::priority_queue
#include <set> //std::set
#include <cmath> //std::pow

//Hashmask based on size of size_t
#if __x86_65__ || __ppc64__ || _WIN64
#define HASHFUNCTION_HASHMASK 0x1000000000
#else
#define HASHFUNCTION_HASHMASK 0x1000
#endif


namespace HSH {
    using byte = unsigned char;
    //Generic hash function for numeric datatypes
    template <typename T>
    size_t hash(T key) {
        static_assert(std::is_arithmetic<T>::value, "wrong type");
        //Getting byte ptr from static cast to key 
        size_t size = sizeof(T);
        byte *buffer = static_cast<byte*>(static_cast<void*>(&key));
        //Multiplier, mask and hash sum to calculate absolute hash
        size_t ret = 0, mask = HASHFUNCTION_HASHMASK, mul = (size_t)std::pow(0x100, sizeof(size_t)/(double)size);
        mul = mul ? mul : 1; //Calculating hash using bytes
        for (byte* it = &buffer[0]; it != &buffer[size]; ++it, mask/=mul, mask = mask ? mask : 1)
            ret += (size_t)((*it)*mask);
        return ret;
    }
    //std::string hash function
    template <>
    size_t hash(std::string key) {
        size_t ret = 0;
        for (size_t i = 0; i < key.length(); ++i) ret += (i+1)*(size_t)key[i]; 
        return ret;
    }
    //STL datastructures hashes
    template <typename T>
    size_t hash(std::vector<T> key) {
        size_t ret = 0;
        for (auto elem : key) ret += hash(elem); 
        return ret;
    }
    //std::vector hash function
    template <typename T>
    size_t hash(std::list<T> key) {
        size_t ret = 0;
        for (auto elem : key) ret += hash(elem); 
        return ret;
    }
    template <typename A, typename B>
    size_t hash(std::map<A, B> key) {
        size_t ret = 0;
        for (auto elem : key) ret += hash(elem); 
        return ret;
    }
    template <typename T, size_t N>
    size_t hash(std::array<T, N> key) {
        size_t ret = 0;
        for (auto elem : key) ret += hash(elem); 
        return ret;
    }
    template <typename T>
    size_t hash(std::set<T> key) {
        size_t ret = 0;
        for (auto elem : key) ret += hash(elem); 
        return ret;
    }
    template <typename T>
    size_t hash(std::multiset<T> key) {
        size_t ret = 0;
        for (auto elem : key) ret += hash(elem); 
        return ret;
    }
    template <typename A, typename B>
    size_t hash(std::multimap<A, B> key) {
        size_t ret = 0;
        for (auto elem : key) ret += hash(elem); 
        return ret;
    }
    template <typename A, typename B>
    size_t hash(std::pair<A, B> key) { 
        return hash(key.first) + hash(key.first);
    }
}
