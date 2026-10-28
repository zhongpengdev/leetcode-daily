#include <stdio.h>
#include <stdbool.h>

bool isPrime(int num)
{
    if (num < 2)
        return false;
    if (num == 2)
        return true;
    if (num % 2 == 0)
        return false;

    for (int i = 3; i * i <= num; i += 2)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

// HDU
int main()
{
    int start, end;
    while (scanf("%d %d", &start, &end) == 2)
    {
        if (start == 0 && end == 0)
        {
            break;
        }

        int allOk = 1;

        for (int i = start; i <= end; i++)
        {
            int currentNumber = i * i + i + 41;
            if (!isPrime(currentNumber))
            {
                allOk = 0;
                break;
            }
        }
        if (allOk)
            printf("OK\n");
        else
            printf("Sorry\n");
    }

    return 0;
}