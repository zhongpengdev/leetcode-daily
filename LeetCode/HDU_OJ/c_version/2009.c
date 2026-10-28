#include <stdio.h>
#include<math.h>

int main()
{
    // 2009
    int n, m;

    // while (scanf("%d %d", &n, &m) != EOF)
    // {
    // 必须是两个变量更新
    //     double result = n;

    //     for (int i = 2; i <= m; i++)
    //     {
    //         result += sqrt(result); 
    //     }

    //     printf("%.2lf", result);
    // }

        while (scanf("%d %d", &n, &m) != EOF)
    {
        double result = n;
        double current = n;

        for (int i = 2; i <= m; i++)
        {
            current = sqrt(current);
            result += current; 
        }

        printf("%.2lf\n", result);
    }
    
    return 0;
}