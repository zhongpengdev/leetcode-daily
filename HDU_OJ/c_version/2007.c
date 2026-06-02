#include <stdio.h>
#include <stdbool.h>

bool isOdd(int number)
{
    return number % 2 != 0;
}

int main()
{

    int start, end;
    while (scanf("%d %d", &start, &end) != EOF)
    {
        long long sum_odd = 0, sum_even = 0;

        // 需要考虑start 和 end大小
        if (start > end)
        {
            int temp = end;
            end = start;
            start = temp;
        }

        for (int i = start; i <= end; i++)
        {
            // pow返回的是double
            if (isOdd(i))
                sum_odd += (long long)i * i * i;
            else
                sum_even += (long long)i * i;
        }

        printf("%lld %lld\n", sum_even, sum_odd);
    }

    return 0;
}