#include <stdio.h>

int main()
{
    int n, m;
    while (scanf("%d %d", &n, &m) == 2 && (n != 0 && m != 0))
    {
        // int* array = (int*)malloc(n * sizeof(int));

        int flag = 1;

        for (int i = 0; i < n; i++)
        {
            int temp;
            if (scanf("%d", &temp) == 1)
            {
                if (temp < m)
                    printf("%d ", temp);
                else if (temp >= m && flag)
                {
                    printf("%d %d ", m, temp);
                    flag = 0;
                }
                else
                {
                    printf("%d", temp);
                }
            }
        }
        printf("\n");
    }
    return 0;
}