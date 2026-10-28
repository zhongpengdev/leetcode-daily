/**
 * 有一头母牛，它每年年初生一头小母牛。每头小母牛从第四个年头开始，每年年初也生一头小母牛。
 * 请编程实现在第n年的时候，共有多少头母牛？
 * 
 * 
 * f(n) = f(n-1) + f(n-3)
 * 
 * 使用一个55的数组记录每一年的牛的数量f(n)
 */

#include <stdio.h>

int main()
{
    int n;
    
    while (scanf("%d", &n) == 1 && n != 0)
    {
        int array[55] = {0, 1, 2, 3};

        for(int i = 4; i <= n; i++) {
            array[i] = array[i-1] + array[i-3];
        }

        printf("%d\n", array[n]);
    }
    
    return 0;
}