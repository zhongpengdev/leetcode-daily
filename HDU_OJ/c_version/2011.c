#include <stdio.h>

double calculate(int n)
{
    // 20
    double sum = 0;
    for (int i = 1; i <= n; i++)
    {
        if (i % 2 == 0)
        {
            sum -= 1.0 / i;
        }
        else
        {
            sum += 1.0 / i;
        }
    }
    return sum;
}

int main()
{
    // 20
    int N;
    if (scanf("%d", &N) == 1)
    {
        for (int i = 0; i < N; i++)
        {
            int n;
            if (scanf("%d", &n) == 1)
            {
                printf("%.2lf\n", calculate(n));
            };
        }
    }

    return 0;
}