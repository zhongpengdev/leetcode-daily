#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int count;
    while (scanf("%d", &count) != EOF)
    {
        double min = 101.0, max = 0.0;
        double sum = 0.0, currentScore = 0.0;

        for (int i = 0; i < count; i++)
        {
            if (scanf("%lf", &currentScore) == 1)
            {
                if (currentScore > max)
                {
                    max = currentScore;
                }
                if (currentScore < min)
                {
                    min = currentScore;
                }

                sum += currentScore;
            }   
        }
        printf("%.2lf\n", (sum - min - max) / (count - 2));
    }

    return 0;
}