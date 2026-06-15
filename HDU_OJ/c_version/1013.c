#include <stdio.h>

// int sum_digits(int n) {
//     int sum = 0;

//     // 2. 这里应该是 n > 0 而不是 n >= 0
//     while (n > 0)
//     {
//         sum += n % 10;

//         //1. 应该是 n / 10 而不是sum / 10
//         n /= 10;
//     }

//     return sum;
// }

// int main()
// {
//     int n;
//     while(scanf("%d", &n) && n != 0) {
//         // while n >= 10则继续循环，否则输出
//         while (n >= 10)
//         {
//             n = sum_digits(n);
//         }

//         printf("%d\n", n);
//     }
//     return 0;
// }

int main()
{
    char str[1000];
    // 不是 str[0] != 0
    while (scanf("%s", str) && str[0] != '0')
    {

        int sum = 0;

        // 将所有数字都加起来
        for (int i = 0; str[i] != '\0'; i++)
        {
            sum += str[i] - '0';
        }

        // sum > 10
        while (sum >= 10)
        {
            int new_sum = 0;
            while (sum > 0)
            {
                // 不是 new_sum += sum %= 10
                new_sum += sum % 10;
                sum /= 10;
            }
            sum = new_sum;
        }

        printf("%d\n", sum);
    }
    return 0;
}