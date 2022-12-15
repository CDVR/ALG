#include <stack> //std::stack
#include <algorithm> //std::fill
#include <utility> //std::pair
#include "KPSK.hpp" //Item

#define b second
#define a first
#define diff (ksc + 1)
#define D(A, B) ((A)*diff + (B))


Items KnapsackDP(const Items& items, const size_t& ksc) {
	//Allocating dp array
	double* dp = new double[(items.size() + 1)*diff];
	//Filling initial zeros
	for (size_t i = 0; i < items.size() + 1; ++i) dp[D(i, 0)] = 0;
	std::fill(dp, dp + diff, 0);
	//Filling rest of dp based on take / dont take
	for (size_t i = 1; i < items.size() + 1; ++i) {
		for (size_t j = 1; j < diff; ++j) {
			//Deciding weather to take the item and decrease max weight or dont take
			if (items[i - 1].b <= j && dp[D(i - 1, j)] < (dp[D(i - 1, j - items[i - 1].b)] + items[i - 1].a))
				dp[D(i, j)] = dp[D(i - 1, j - items[i - 1].b)] + items[i - 1].a;
			else dp[D(i, j)] = dp[D(i - 1, j)];
		} //Items that we ended up taking
	} Items ret; //Collecting backwards the items we calculated
	for (size_t x = ksc, y = items.size(); y > 0; --y) {
		if (dp[D(y - 1, x)] == dp[D(y, x)]) continue;
		//We took the item so we decrement weight and add it to ret
		x -= items[y - 1].b; ret.push_back(items[y - 1]);
	} delete[] dp; 
	return ret;
}
