#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // 2008
    int n;
    while (scanf("%d", &n) && n != 0)
    {
        int negative = 0, zero = 0, positive = 0;
        for (int i = 0; i < n; i++)
        {
            double temp;
            if (scanf("%lf", &temp) == 1)
            {
                if (temp > 0)
                {
                    positive++;
                }
                else if (temp == 0)
                    zero++;
                else
                    negative++;
            }
        }

        printf("%d %d %d\n", negative, zero, positive);
    }

    return 0;
}