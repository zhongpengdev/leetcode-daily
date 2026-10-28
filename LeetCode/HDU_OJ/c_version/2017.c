#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int n;
    scanf("%d", &n);

    while (n--)
    {
        char str[1000];
        scanf("%s", str); // 主要是如何读取字符串。

        int count = 0;
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] >= '0' && str[i] <= '9')
            {
                count++;
            }
        }

        printf("%d\n", count);
    }

    return 0;
}