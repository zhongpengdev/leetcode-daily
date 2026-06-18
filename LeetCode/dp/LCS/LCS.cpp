/*
求最长公共子序列
*/
#include<iostream>

int LCSLength(std::string X, std::string Y) {
    int xLength = X.length();
    int yLength = Y.length();

    int dp[xLength + 1][yLength + 1];

    for (int i = 0; i <= xLength; i++) {
        for (int j = 0; j <= yLength; j++) {
            if (i == 0 || j == 0) { //初始化第一行和第一列
                dp[i][j] = 0;
            } else if(X[i - 1] == Y[j - 1]) { //数组的i，j对应字符串中的i-1， j-1
                dp[i][j] = dp[i - 1][j - 1] + 1; //直接操作的是dp数组
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[xLength][yLength]; 
}


int main() {
    std::string X = "AGGTAB";
    std::string Y = "GXTXAYB";

    int length = LCSLength(X, Y);
    std::cout << "Length of Longest Common Subsequence is " << length << std::endl;

    return 0;
}