#include <unordered_map> //std::unordered_map
#include <vector> //std::vector
#include <string> //std::string
using byte = unsigned char;
using HuffMap = std::unordered_map<byte, std::vector<bool>>;
HuffMap huffmanCoding(const std::string& str);
