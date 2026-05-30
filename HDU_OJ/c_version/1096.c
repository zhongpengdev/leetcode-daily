#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // 1096
    int count;
    scanf("%d", &count);

    for (int i = 0; i < count; i++)
    {
        int n;
        scanf("%d", &n);

        int sum = 0;
        for (int j = 0; j < n; j++)
        {
            int a;
            scanf("%d", &a);
            sum += a;
        }
        printf("%d\n", sum);
        printf("\n");
    }
    

    return 0;
}