/*
区间DP - 矩阵连乘
*/

#include<iostream>
#include<climits>
#include<vector>

int matrixChainMultiplication(const std::vector<int> &p) {
    //p：维度数组[10, 20, 30, 40]代表三个矩阵
    int n = p.size() - 1;

    std::vector<std::vector<int>> dp(n+1, std::vector<int>(n + 1));

    //单个矩阵
    for(int i = 0; i <= n; i++) {
        dp[i][i] = 0;
    }

    //2--n个矩阵
    for (int L = 2; L <= n; L++) {
        //首元素
        for (int i = 1; i <= n - L + 1; i++) {

            //当前首元素的末尾元素 j
            int j = i + L - 1;

            dp[i][j] = INT_MAX;

            //dp[i][j] = min(dp[i][k] + dp[k+1][j] + dim[i]*dim[k+1]*dim[j+1])
            for (int k = i; k < j; k++) {
                //使用k切分并记录每一次的值
                int q = dp[i][k] + dp[k+1][j] + p[i - 1]*p[k]*p[j];

                if(q < dp[i][j]) {
                    dp[i][j] = q;
                }
            }
        }
    }

    return dp[1][n];
}

int main() {
    // --- 测试用例 1: 刚才讲过的经典例子 ---
    // 矩阵 A1: 10 x 100
    // 矩阵 A2: 100 x 5
    // 矩阵 A3: 5 x 50
    // 应该先算 (A1 A2) 再乘 A3 -> 代价 7500
    std::vector<int> p1 = {10, 100, 5, 50};
    std::cout << "测试用例 1 (10, 100, 5, 50): " 
              << matrixChainMultiplication(p1) << std::endl;
    // 预期输出: 7500


    // --- 测试用例 2: 算法导论书上的经典例子 ---
    // A1: 30 x 35
    // A2: 35 x 15
    // A3: 15 x 5
    // A4: 5 x 10
    // A5: 10 x 20
    // A6: 20 x 25
    std::vector<int> p2 = {30, 35, 15, 5, 10, 20, 25};
    std::cout << "测试用例 2 (30, 35, 15, 5, 10, 20, 25): " 
              << matrixChainMultiplication(p2) << std::endl;
    // 预期输出: 15125


    // --- 测试用例 3: 简单递增 ---
    // A1: 1x2, A2: 2x3, A3: 3x4
    // 方案1 (A1A2)A3 = 1*2*3 + 1*3*4 = 6 + 12 = 18
    // 方案2 A1(A2A3) = 2*3*4 + 1*2*4 = 24 + 8 = 32
    // 最优应该是 18
    std::vector<int> p3 = {1, 2, 3, 4};
    std::cout << "测试用例 3 (1, 2, 3, 4): " 
              << matrixChainMultiplication(p3) << std::endl;
    // 预期输出: 18

    return 0;
}