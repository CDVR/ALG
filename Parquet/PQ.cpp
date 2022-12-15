#include <cstddef> //size_t
#include <algorithm> //std::swap, std::fill
#define D(A, B) ((A)*a + (B))


size_t possibleParquetPatterns(size_t a, size_t b) {
	//Early return with 0 if even num or 0 size
	if (!a || !b || (a*b)%2) return 0;
	//Swapping a with b to get smaller dp and calculating mask size
	if (a < b) std::swap(a, b); 
	size_t h = 1 << b; 
	//Creating dp array representing values ways to get to certain mask
	size_t* dp = new size_t[a * h]; std::fill(dp, dp + (a * h), 0);
	//Resursive lambda to calculate and update masks in next row based on the prev mask given 
	auto genMaskReach_ = [a, b, dp]
		(auto& gmr_, const size_t prevMask, size_t nextMask, const size_t idx, const size_t row) -> void {
			//Iterate through bits of prevMask and generate nextMask based on it
			for (size_t i = idx; i < b; ++i) {
				if (prevMask & (1 << i)) continue;
				//If there are multiple ways the nextMask could be 'fork' the function
				if (((i + 1) < b) && !(prevMask & (1 << (i + 1)))) 
					gmr_(gmr_, prevMask, nextMask, i + 2, row);
				nextMask |= (1 << i);
			} //Increment the dp for the generated mask in row with value of prevmask from prev row
			dp[D(nextMask, row)] += (row ? dp[D(prevMask, row - 1)] : 1);
		}; //Initial call of rec lambda for 1st row with empty bitmask 
	genMaskReach_(genMaskReach_, 0, 0, 0, 0);
	//For each next row for each mask that has way to get to it generate nextMasks
	for (size_t i = 1; i < a; ++i)
		for (size_t j = 0; j < h; ++j)
			if (dp[D(j, i - 1)]) genMaskReach_(genMaskReach_, j, 0, 0, i);
	//Answer lays in last row and 0 mask (prev row must have finshed all)
	size_t ret = dp[D(0, a - 1)]; 
	delete[] dp; return ret;
}
