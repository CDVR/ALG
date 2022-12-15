#include "HMDP.hpp" //std::vector, Cords
using namespace std;


//Expects all inner vectors representing vectors to be of the same size aka rectangular grid
vector<vector<int>> cointsDP(const vector<vector<int>>& grid) {
	if (grid.size() == 0 || grid[0].size() == 0) return vector<vector<int>>();
	//Size definition
	size_t H = grid.size(); size_t W = grid[0].size();
	//DP vector
	vector<vector<int>> dp(H, vector<int>(W, 0));
	//Last element and last row fill
	dp[H - 1][W - 1] = grid[H - 1][W - 1];
	for (long i = W - 2; i >= 0; --i) dp[H - 1][i] = dp[H - 1][i + 1] + grid[H - 1][i];
	//DP fill loop
	for (long i = H - 2; i >= 0; --i)
		for (long j = W - 1; j >= 0; --j) {
			if (j < ((long)(W - 1)) && (dp[i][j + 1] > dp[i + 1][j])) dp[i][j] = dp[i][j + 1] + grid[i][j];
			else dp[i][j] = dp[i + 1][j] + grid[i][j];
		} //Result is stored at the start of path
	return dp;
}

vector<Cords> getPath(const vector<vector<int>>& grid) {
	if (grid.size() == 0 || grid[0].size() == 0) return vector<Cords>();
	//vector with result
	vector<Cords> ret;
	//Size definition
	size_t H = grid.size(); size_t W = grid[0].size();
	for (size_t x = 0, y = 0, i = 0; i < (H + W - 1); i++) {
		//Storing current position in the path
		ret.push_back({.y = y, .x = x});
		//Moving down or up based on what yields better
		if ((x + 1) >= W) y++;
		else if ((y + 1) >= H) x++;
		else (grid[y][x + 1] > grid[y + 1][x]) ? x++ : y++;
	} //Returning one of best paths (if there are multiple) 
	return ret;
}
