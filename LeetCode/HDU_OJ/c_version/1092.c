#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int N;

    while (scanf("%d", &N) == 1)
    {
        if (N == 0)
            break;

        int sum = 0;
        int current_value = 0;

        for (int i = 0; i < N; i++)
        {
            scanf("%d", &current_value);
            sum += current_value;
        }

        printf("%d\n", sum);
    }

    return 0;
}