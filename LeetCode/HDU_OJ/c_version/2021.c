#include <stdio.h>
#include <stdlib.h>

int main()
{
    int count;
    // 贪心算法，每次拿最多的往下抵扣
    int salary[6] = {100, 50, 10, 5, 2, 1};

    while (scanf("%d", &count) == 1 && count != 0)
    {
        int totalSheet = 0;
        int *array = (int *)malloc(count * sizeof(int));

        for (int i = 0; i < count; i++)
        {
            scanf("%d", &array[i]);
        }

        // 处理当前用户的sheet
        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                totalSheet += array[i] / salary[j];
                array[i] %= salary[j]; // 坑1：是 %=
            }
        }

        printf("%d ", totalSheet);
        printf("\n");
    }

    return 0;
}