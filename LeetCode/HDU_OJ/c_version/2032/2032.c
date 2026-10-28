/**
 * 杨辉三角的解法
 * 
 * 1. 二维数组
 * 
 * 2. 一维数组
 * 
 * 每一列的0号元素都是1 对角线元素ii 都是 1
 * 
 * 其他元素 a[i][j] = a[i-1][j] + a[i-1][j-1]
 */

#include <stdio.h>

int main()
{
    int n;
    while (scanf("%d", &n) != EOF)
    {
        int array[30][30];


        // 控制行数
        for (int i = 0; i < n; i++)
        {
            // 每一行的第一列是1
            array[i][0] = 1;

            // 对角线都是1
            array[i][i] = 1;

            // 每一行i个数
            for (int j = 1; j < i; j++)
            {
                // 其他元素 a[i][j] = a[i-1][j] + a[i-1][j-1]
                array[i][j] = array[i-1][j-1] + array[i-1][j];
            }
        }

        // 输出
        for (int i = 0; i < n; i++)
        {
            // 坑1：这里应该是j <= i
            for (int j = 0; j <= i; j++)
            {
                printf("%d ", array[i][j]);

                // 连带错误：这里应该是 j == 1
                if(j == i) {
                    printf("\n");
                }
            }
        }
    }
    return 0;
}