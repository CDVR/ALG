#include "MCM.hpp" //size_t, Duo, std::vector, IdealBrackets, Matrices, IdealBracketsOperations
#include <stack> //std::stack

#define D2(A, B) ((A)*size + (B))


IdealBrackets generateBrackets_(Duo<size_t, long>* dp, size_t size) {
	IdealBrackets ret = IdealBrackets(size + 1, {0, 0});
	std::stack<Duo<size_t, size_t>> stack;
	stack.push({size - 1, 0});
	while (!stack.empty()) {
		auto pos = stack.top(); stack.pop();
		long mid = dp[D2(pos.a, pos.b)].b;
		if (mid == -1) continue;
		ret[pos.b].a++; ret[mid + 1].a++; 
		ret[mid + 1].b++; ret[pos.a + 1].b++;
		stack.push({(size_t)mid, pos.b});
		stack.push({pos.a, (size_t)mid + 1});
	} delete[] dp; return ret;
}

IdealBracketsOperations idealChainMatrixMultiplication(const Matrices& mats) {
	if (mats.size() < 2) return {IdealBrackets(), 0};
	size_t size = mats.size();
	Duo<size_t, long>* dp = new Duo<size_t, long>[size * size];
	for (size_t i = 0; i < size; ++i) dp[D2(i, i)] = {0, -1};
	for (size_t i = 1; i < size; ++i) {
		for (size_t J = i, j = i - 1; J > 0; --j, --J) {
			dp[D2(i, j)] = {0, -1};
			for (size_t k = j; k < i; ++k) {
				size_t tmp = dp[D2(k, j)].a + dp[D2(i, k + 1)].a + mats[j].a * mats[k].b * mats[i].b;
				if (tmp < dp[D2(i, j)].a || dp[D2(i, j)].b == -1) dp[D2(i, j)] = {tmp, (long)k};
			}
		} 
	} size_t tmp = dp[D2(size - 1, 0)].a;
	return IdealBracketsOperations{generateBrackets_(dp, size), tmp};
}
