#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int count;
    while (scanf("%d", &count) == 1 && count != 0)
    {
        int min = 1000, index = 0, array[101];

        for (int i = 0; i < count; i++)
        {
            // 大数组
            scanf("%d", &array[i]);

            if (array[i] < min)
            {
                index = i;
                min = array[i];
            }
        }

        array[index] = array[0];
        array[0] = min;

        for (int i = 0; i < count; i++)
        {
            printf("%d ", array[i]);
            printf("\n");
        }
    }

    return 0;
}