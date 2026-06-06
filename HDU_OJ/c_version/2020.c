/**
 *
 * 看到从小到大输出直接对array做一个排序就行
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    // HDU
    int count;

    while (scanf("%d", &count) == 1 && count != 0)
    {
        int *array = (int *)malloc(count * sizeof(int));
        for (int i = 0; i < count; i++)
        {
            int temp;
            scanf("%d", &temp);
            array[i] = temp;
        }

        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (abs(array[j]) < abs(array[j + 1]))
                {
                    int temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < count; i++)
        {
            printf("%d ", array[i]);
        }
        printf("\n");

        // 释放内存
        free(array);
    }

    return 0;
}