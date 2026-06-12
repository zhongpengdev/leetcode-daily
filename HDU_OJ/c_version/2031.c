/**
 *
 * 短除法
 *
 * 每次记录余数 remainder = N % R
 *
 * N的结果每次都是 N / R
 */
#include <stdio.h>

int main()
{

    int N, R;

    while (scanf("%d %d", &N, &R) != EOF)
    {
        char result[100];

        // N是负数时
        if(N < 0) {
            printf("-");
            N = -N;
        }

        int i = 0;
        do
        {
            int remainder = N % R; // 余数作为结果

            // 对结果进行变换
            if(remainder < 10) {
                result[i++] = remainder + '0';
            }
            else {
                result[i++] = remainder - 10 + 'A'; 
            }

            N /= R;
        } while (N > 0);

        // 逆序输出
        for (int j = i-1; j >= 0; j--)
        {
            printf("%c", result[j]);
        }

        printf("\n");
    }

    return 0;
}