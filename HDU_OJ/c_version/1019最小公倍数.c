#include <stdio.h>
#include <stdlib.h>

// lcm

int gcd(int a, int b)
{
    // haha
    while (b)
    {
        int temp = a % b;
        a = b;
        b = temp;
    }

    return a;
}

int lcm(int a, int b)
{
    // haha
    if (a == 0 || b == 0)
    {
        return 0;
    }

    return abs(a / gcd(a, b) * b);
}

// 任意两个数都要找到最小公倍数
int arrays_lcm(int array[], int count){
    int result = array[0];

    for (int i = 1; i < count; i++)
    {
        result = lcm(result, array[i]);
    }

    return result;
}


int main()
{
    int n;
    scanf("%d", &n);

    while (n--)
    {
        int count;
        scanf("%d", &count);

        int *array = (int *)malloc(count * sizeof(int));

        for (int i = 0; i < count; i++)
        {
            // 1. 是 &array[i]
            scanf("%d", &array[i]);
        }

        // 输出最小公倍数
        printf("%d\n", arrays_lcm(array, count));
    }

    return 0;
}