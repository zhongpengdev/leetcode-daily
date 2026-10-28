#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int score;

    while (scanf("%d", &score) != EOF)
    {
        // switch case不支持判断使用if else

        // 还需要判断>100的情况
        if (score < 0 || score > 100)
            printf("Score is error!\n");
        else if (score >= 90 && score <= 100)
            printf("A\n");
        else if (score >= 80)
            printf("B\n");
        else if (score >= 70)
            printf("C\n");
        else if (score >= 60)
            printf("D\n");
        else if (score >= 0)
            printf("E\n");
    }

    return 0;
}