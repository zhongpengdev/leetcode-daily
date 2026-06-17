/**
 * 
 * n的范围是. (n < 1,000,000)
 * 
 * int爆了，主要考查点在于取余结果是不是0
 */

#include <stdio.h>

// int fab(int n) {
//     if(n == 0) return 7;
//     if(n == 1) return 11;

//     int cur = 18, last = 11;
//     for (int i = 2; i < n; i++)
//     {
//         // last的值一直都是11没变
//         // int temp = last;
//         // cur += last;
//         // last = temp;

//         int temp = cur;
//         cur += last;
//         // 更新last
//         last = temp;
//     }
//     return cur;
// }


int fab(int n) {
    // 7 % 3
    if(n == 0) return 1;
    // 11 % 3
    if(n == 1) return 2;

    // cur = 18 % 3 = 0 last = 11 % 3
    int cur = 0, last = 2;
    for (int i = 2; i < n; i++)
    {
        // last的值一直都是11没变
        // int temp = last;
        // cur += last;
        // last = temp;

        int temp = cur;
        cur = (cur + last) % 3;
        // 更新last
        last = temp;
    }
    return cur;
}

int main()
{
    int n;
    while (scanf("%d", &n) != EOF)
    {
        // 输入n，计算n位是什么
        int result = fab(n);
        if(result == 0) printf("yes\n");
        else printf("no\n");
    }
        
    return 0;
}