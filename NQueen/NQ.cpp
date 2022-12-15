#include <set> //std::set
#include <stack> //std::stack
#include "NQ.hpp" //Solutions, size_t, std::vector 


struct cords {size_t x, y;};

Solutions nQueen(const size_t& dim) {
	//For deciding which roads are possible
	std::vector<size_t> board(dim*dim, 0);
	//The current exploration path
	std::vector<size_t> current; current.reserve(dim);
	//Exploration stack to test all possibilities using early DFS cutoff
	std::stack<std::pair<cords, bool>> stack;
	//All the solutions found during exploration
	Solutions solutions;
	//Initializing stack with first row
	for (size_t i = 0; i < dim; ++i) stack.push({{0, i}, true});
	//Exploration of stack to test all posibilities
	while (!stack.empty()) {
		//Was already explored for this test case
		if (!stack.top().second) { //Remove its limitations for other in board
			for (size_t i = 1; i + stack.top().first.x < dim; ++i) {
				board[stack.top().first.y*dim + i + stack.top().first.x]--;
				if (stack.top().first.y + i < dim) board[(stack.top().first.y + i)*dim + i + stack.top().first.x]--;
				if (stack.top().first.y >= i) board[(stack.top().first.y - i)*dim + i + stack.top().first.x]--;
			} //Remove from stack and current explored solution 
			current.pop_back(); stack.pop();
		} //Was not yet explored for this test case
		else { //Increment indexes in the board to limit possible answers
			for (size_t i = 1; i + stack.top().first.x < dim; ++i) {
				board[stack.top().first.y*dim + i + stack.top().first.x]++;
				if (stack.top().first.y + i < dim) board[(stack.top().first.y + i)*dim + i + stack.top().first.x]++;
				if (stack.top().first.y >= i) board[(stack.top().first.y - i)*dim + i + stack.top().first.x]++;
			} //Push current to solutions vector and set it to reset in stack
			current.push_back(stack.top().first.y); stack.top().second = false;
			//If it is the last one we have found a solution
			if (dim - 1 == stack.top().first.x) solutions.push_back(current);
			//Else we push its possible next nodes up
			else for (size_t cx = stack.top().first.x, i = 0; i < dim; ++i) 
				if (!board[i*dim + cx + 1]) stack.push({{cx + 1, i}, true});
		} //Return filled solutions
	} return solutions;
}

Solutions getDistinct(const Solutions& sols) {
	std::set<std::vector<size_t>> distinct;
	//Going through all the solutions to filter out non distinct
	for (const auto& sol : sols){
		bool dist = true;
		Solutions rots(4, sol);
		//Generating 90, 180 andd 270 rotations
		for (size_t i = 0; i < sol.size(); ++i) rots[1][(sol.size() - 1) - sol[i]] = i;
		for (size_t i = 0; i < sol.size(); ++i) rots[2][sol[i]] = (sol.size() - 1) - i;
		for (size_t i = 0; i < sol.size(); ++i) rots[3][i] = (sol.size() - 1) - sol[(sol.size() - 1) - i];
		//For each rotation generating v and h mirror on top of the rotation
		for (const auto& rot : rots) {
			//Generating mirror for each rotation
			std::vector<size_t> mir(sol.size());
			for (size_t i = 0; i < sol.size(); ++i) mir[i] = rot[(sol.size() - 1) - i];
			//Checking if the mirror exists
			if (distinct.find(rot) != distinct.end()) {dist = false; break;}
			if (distinct.find(mir) != distinct.end()) {dist = false; break;}
		} //If solutions rotations and mirrors are not in solutions insert solution 
		if (dist) distinct.insert(sol);
	} //Moving distinct from set to vector
	Solutions ret; 
	for (const auto& sol : distinct) ret.push_back(sol);
	return ret;
}
