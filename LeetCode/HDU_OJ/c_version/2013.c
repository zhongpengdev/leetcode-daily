#include <stdio.h>

// HDU

int main()
{
    int n;
    while (scanf("%d", &n) != EOF)
    {
        // 第n天导回第一天
        int sum = 1;
        while (n != 1)
        {
            sum = (sum + 1) * 2;
            n--;
        }

        printf("%d\n", sum);
    }
    
    return 0;
}