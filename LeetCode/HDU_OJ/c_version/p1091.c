#include <stdio.h>

int main()
{
    // p1091
    int a, b;

    // 问题：遇到一个输入0就会终止
    while (scanf("%d %d", &a, &b) && a != 0 && b != 0)
    {
        printf("%d\n", a + b);
    }
    return 0;
}

int main()
{
    // p1091
    int a, b;

    while (scanf("%d %d", &a, &b) == 2)
    {
        // 内部判断
        if (a == 0 && b == 0)
            break;

        printf("%d\n", a + b);
    }
    return 0;
}