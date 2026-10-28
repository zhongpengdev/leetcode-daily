#include <stdio.h>
#include <string.h>

#define N 50
#define M 5

double score[N][M], stuAvg[N], couAvg[M];

int main()
{
    int n, m, i, j;

    while (~scanf("%d%d", &n, &m))
    {
        /* 清零 */
        memset(stuAvg, 0, sizeof(stuAvg));
        memset(couAvg, 0, sizeof(couAvg));

        /* 读数据的同时累加行和、列和 */
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                scanf("%lf", &score[i][j]);
                stuAvg[i] += score[i][j];
                couAvg[j] += score[i][j];
            }
        }

        /* 第一行：每个学生的平均分 */
        for (i = 0; i < n; i++)
        {
            if (i != 0) printf(" ");
            printf("%.2f", stuAvg[i] / m);
        }
        printf("\n");

        /* 第二行：每门课的平均分 */
        for (j = 0; j < m; j++)
        {
            couAvg[j] /= n;
            if (j != 0) printf(" ");
            printf("%.2f", couAvg[j]);
        }
        printf("\n");

        /* 第三行：各科成绩均 >= 该科平均分的学生数 */
        int cnt = 0, pass;
        for (i = 0; i < n; i++)
        {
            pass = 0;
            for (j = 0; j < m; j++)
            {
                if (score[i][j] < couAvg[j]) break;
                else pass++;
            }
            if (pass == m) cnt++;
        }
        printf("%d\n\n", cnt);
    }
    return 0;
}
