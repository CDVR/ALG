#include "BT.hpp" //Brick, Bricks, Brickdex, std::vector, std::pair 
#include <algorithm> //std::sort, std::max, std::min, std::max_element
#include <numeric> //std::accumulate


Brick* permutateAndSort_(const Bricks& bricks) {
    Brick* ret = new Brick[bricks.size()*3];
    for (size_t i = 0; i < bricks.size(); ++i){
        ret[i*3] = {bricks[i].a, std::max(bricks[i].b, bricks[i].c), std::min(bricks[i].b, bricks[i].c)};
        ret[i*3 + 1] = {bricks[i].b, std::max(bricks[i].a, bricks[i].c), std::min(bricks[i].a, bricks[i].c)};
        ret[i*3 + 2] = {bricks[i].c, std::max(bricks[i].b, bricks[i].a), std::min(bricks[i].b, bricks[i].a)};
    }
    std::sort(ret, ret + bricks.size()*3, [](const Brick& a, const Brick& b) {return a.b > b.b;});
    return ret;
}

Bricks getBricks_(Brickdex* dp, Brick* bricks, size_t size) {
    Bricks ret;
    auto lbda = [](const Brickdex& a, const Brickdex& b) {return a.first < b.first;};
    for (auto i = std::max_element(dp, dp + size, lbda); i; i = i->second ? dp + i->second : nullptr)
        ret.push_back(bricks[i - dp]);
    delete[] bricks; delete[] dp;
    return ret;
}


Bricks towerOfBricks(const Bricks& initBricks) {   
    if (!initBricks.size()) return Bricks(0); 
    size_t size = initBricks.size()*3;
    Brick* bricks = permutateAndSort_(initBricks);
    Brickdex* dp = new Brickdex[size];    
    for (size_t i = size; i > 0; --i) {
        Brickdex maxp = {0, 0};
        for (size_t j = i; j < size; ++j) {
            if (bricks[i - 1].b > bricks[j].b && bricks[i - 1].c > bricks[j].c && dp[j].first > maxp.first)
                maxp = {dp[j].first, j};
        } 
        dp[i - 1] = {maxp.first + bricks[i - 1].a, maxp.second};
    }    
    return getBricks_(dp, bricks, size);
}

double getTowerHeigth(const Bricks& tower) {
    return std::accumulate(tower.begin(), tower.end(), 0, [](const double& sum, const Brick& a){return sum + a.a;});
}
