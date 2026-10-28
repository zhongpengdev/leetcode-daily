#include <stdio.h>

int main()
{
    int count;
    scanf("%d", &count);
    scanf("%*c");  /* 吃掉 count 后面的换行，只吃一次 */

    while (count--)
    {
        char str[101];
        int i;
        char ch;

        /* 逐字符读到换行 */
        for (i = 0; i < 100; i++)
        {
            if (scanf("%c", &ch) != 1 || ch == '\n') break;
            str[i] = ch;
        }
        str[i] = '\0';

        int a = 0, e = 0, ii = 0, o = 0, u = 0;

        for (i = 0; str[i] != '\0'; i++)
        {
            switch (str[i])
            {
            case 'a': a++; break;
            case 'e': e++; break;
            case 'i': ii++; break;
            case 'o': o++; break;
            case 'u': u++; break;
            }
        }

        printf("a:%d\n", a);
        printf("e:%d\n", e);
        printf("i:%d\n", ii);
        printf("o:%d\n", o);
        printf("u:%d\n", u);

        if (count != 0)
            printf("\n");
    }

    return 0;
}
