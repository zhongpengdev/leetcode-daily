#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

int lcm(int a, int b)
{
    return a / gcd(a, b) * b;
}

int lcmArray(int arr[], int n)
{
    int result = arr[0];
    for (int i = 1; i < n; i++)
    {
        result = lcm(result, arr[i]);
    }
    return result;
}

int main()
{
    int count;
    while (scanf("%d", &count) != EOF)
    {
        // 分配数组
        int *array = (int *)malloc(count * sizeof(int));

        for (int i = 0; i < count; i++)
        {
            scanf("%d", &array[i]);
        }

        printf("%d\n", lcmArray(array, count));
    }

    return 0;
}