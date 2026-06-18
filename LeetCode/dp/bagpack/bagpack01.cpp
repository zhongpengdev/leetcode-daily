/*
背包类DP

w[]
v[]
W 当前重量
N 物品数

01：每件物品只能拿一次，要么拿上，要么不拿。
完全背包问题：可以拿多次。
*/
#include<iostream>
#include<vector>
#include<algorithm>


//二维
int knapsack01(const std::vector<int> &weight, const std::vector<int> &value, int W){
    //物品数量
    int N = weight.size();

    //dp[N][W]
    std::vector<std::vector<int>>dp(N+1, std::vector<int>(W+1, 0));

    for(int i = 1; i <= N; i++) {
        for(int j = 0; j <= W; j++) {
            //不拿当前物品
            dp[i][j] = dp[i-1][j];

            if(j >= weight[i-1]) {
                dp[i][j] = std::max(dp[i][j], dp[i-1][j - weight[i-1]] + value[i-1]);
            }
        }
    }

    return dp[N][W];
}

int main() {
    std::vector<int> w = {3, 4, 5, 2};   // 重量
    std::vector<int> v = {5, 7, 9, 4};   // 价值
    int W = 10;                         // 背包容量
    
    std::cout << "最大价值 = " << knapsack01(w, v, W) << std::endl;  // 输出 18
    return 0;
}

