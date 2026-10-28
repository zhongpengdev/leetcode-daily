#include <stdio.h>
#include <stdlib.h>

int main()
{
    // HDU
    int n, m;

    while (scanf("%d %d", &n, &m) && (n != 0 || m != 0))
    {
        int *array1 = (int *)malloc(n * sizeof(int));
        int *array2 = (int *)malloc(m * sizeof(int));

        for (int i = 0; i < n; i++)
        {
            scanf("%d", &array1[i]);
        }

        for (int i = 0; i < m; i++)
        {
            scanf("%d", &array2[i]);
        }

        // 双数组逐个遍历，如果存在则直接退出，如果不在，就当前的array1[i]加到result数组
        int result[n];
        int cnt = 0;

        for (int i = 0; i < n; i++)
        {
            int inArray2 = 0;

            for (int j = 0; j < m; j++)
            {
                if (array2[j] == array1[i])
                {
                    inArray2 = 1;
                    break;
                }
            }
            if (!inArray2)
            {
                result[cnt++] = array1[i];
            }
        }

        // 从小到大排序
        for (int i = 0; i < cnt - 1; i++)
        {
            for (int j = 0; j < cnt - 1 - i; j++)
            {
                if (result[j] > result[j + 1])
                {
                    int temp = result[j];
                    result[j] = result[j + 1];
                    result[j + 1] = temp;
                }
            }
        }

        // 错误：cnt如果是0的话那么的输出
        if (cnt == 0)
        {
            printf("NULL\n");
        }
        else
        {
            for (int i = 0; i < cnt; i++)
            {
                printf("%d ", result[i]);
            }
            printf("\n");
        }
    }
}
