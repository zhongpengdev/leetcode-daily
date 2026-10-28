#include <stdio.h>
#include <stdlib.h>

int main()
{
    // HDU
    int n, m;
    int array[50][5];

    while (scanf("%d %d", &n, &m) != EOF)
    {
        float *averageScore = (float *)malloc(m * sizeof(float));

        for (int i = 0; i < n; i++)
        {
            float allSocre = 0;

            for (int j = 0; j < m; j++)
            {
                scanf("%d", &array[i][j]);
                allSocre += array[i][j];
            }

            // 输出该学生的平均分数
            // printf("%.2f ", allSocre / m);

            // // 如果是最后一个学生，换行
            // if (i == n - 1)
            //     printf("\n");

            if (i == n - 1)
                printf("%.2f\n", allSocre / m);
            else
                printf("%.2f ", allSocre / m);
        }

        // 统计m门棵的平均成绩
        for (int i = 0; i < m; i++)
        {
            float scoreOfThisClass = 0;

            for (int j = 0; j < n; j++)
            {
                scoreOfThisClass += array[j][i];
            }

            // 这门课的n个学生的平均成绩
            // printf("%.2f ", scoreOfThisClass / n);
            averageScore[i] = scoreOfThisClass / n;

            // if (i == m - 1)
            //     printf("\n");
            if (i == m - 1)
                printf("%.2f\n", averageScore[i]);
            else
                printf("%.2f ", averageScore[i]);
        }

        int count = 0;
        // 统计各科乘积均 >= 平均成绩的学生人数。
        for (int i = 0; i < n; i++)
        {
            int all_pass = 1;
            for (int j = 0; j < m; j++)
            {
                if (array[i][j] < averageScore[j])
                {
                    all_pass = 0;
                    break;
                }
            }

            if (all_pass)
                count++;
        }

        printf("%d\n", count);

        free(averageScore);
    }
    return 0;
}
