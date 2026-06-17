#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int n;
    scanf("%d", &n);

    while (n--)
    {
        char str[10000];

        scanf("%s", str);

        int count = 1;
        char last_char = str[0];

        for (int i = 1; str[i] != '\0'; i++)
        {
            // 如果当前值和前一个char不相等
            if (str[i] != last_char)
            {
                if (count == 1)
                {
                    printf("%c", last_char);
                }
                else
                {
                    // count不是1
                    printf("%d%c ", count, last_char);
                    count = 1;
                }
                // 更新lastchar
                last_char = str[i];
            }
            else
            {
                // 如果等于最后一个char
                count++;
            }
        }

        if (count == 1)
        {
            printf("%c", last_char);
        }
        else
        {
            printf("%d%c", count, last_char);
        }

        printf("\n");
    }

    return 0;
}