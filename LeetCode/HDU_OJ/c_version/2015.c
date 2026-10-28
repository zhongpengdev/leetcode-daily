/**
 * 有一个长度为n(n<=100)的数列，该数列定义为从2开始的递增有序偶数，现在要求你按照顺序每m个数求出一个平均值，如果最后不足m个，则以实际数量求平均值。编程输出该平均值序列。
 

Input
输入数据有多组，每组占一行，包含两个正整数n和m，n和m的含义如上所述。
 

Output
对于每组输入数据，输出一个平均值序列，每组输出占一行。
 */
#include <stdio.h>

int main()
{
    int n, m;
    while (scanf("%d %d", &n, &m) != EOF)
    {
        int i = 1;      // 当前位置（从第1个数开始）
        while (i <= n)
        {
            int sum = 0;
            int count = 0;
            
            // 累加 m 个数，但不超过 n
            for (int j = 1; j <= m && i <= n; j++)
            {
                sum += 2 * i;   // 第 i 个偶数是 2*i
                count++;
                i++;
            }
            
            // 输出平均值
            printf("%d", sum / count);
            
            // 如果还有剩余数字，输出空格
            if (i <= n)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    return 0;
}