#pragma once
#include <vector> //std::vector
#include <cstddef> //size_t

struct Cords {
	size_t y, x;
	bool operator==(const Cords& b) const {
		return this->x == b.x && this->y == b.y;
	}
};

std::vector<std::vector<int>> cointsDP(const std::vector<std::vector<int>>& grid);
std::vector<Cords> getPath(const std::vector<std::vector<int>>& grid);
