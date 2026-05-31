#include <stdio.h>
#include<math.h>

#define PI 3.1415927

int main()
{
    // 2002
    double r;
    while (scanf("%lf", &r) != EOF)
    {
        // 体积公式是4.3
        // 必须使用double
        double v = 4.0/3.0 * PI * pow(r, 3);
        printf("%.3f\n", v);
    }
    
    return 0;
}