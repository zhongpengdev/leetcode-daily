/**
 * 
 * 约数：能被a整除的数
 * 正约数：不包含a本身其他的约数
 */

#include <stdio.h>

// 约束的和
int sum_divisors(int a)
{
    int result = 1;
    // 2. 这里应该是从2开始
    for (int i = 2; i < a; i++)
    {
        if (a % i == 0)
        {
            result += i;
        }
    }

    // 1. 忘记写返回值了
    return result;
}

// 计算约数并返回判断结果
int is_equal(int a, int b)
{
    if (sum_divisors(a) == b && sum_divisors(b) == a)
    {
        return 1;
    }

    return 0;
}

int main()
{
    int M;
    scanf("%d", &M);

    while (M--)
    {
        int a, b;
        scanf("%d %d", &a, &b);

        if (is_equal(a, b))
            printf("YES\n");

        else
            printf("NO\n");
    }

    return 0;
}