#include <stdio.h>

int main()
{

    char name[50];
    double num, price, total = 0;

    while (scanf("%s %lf %lf", name, &num, &price) != EOF)
    {
        total += num * price;
    }

    // %。1lf自带四舍五入。
    printf("%.1lf\n", total);

    return 0;
}
