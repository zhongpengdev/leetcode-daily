#include <stdio.h>
#include<math.h>

int main()
{
    double input;
    while (scanf("%lf", &input) != EOF)
    {
        // use fabs not abs
        printf("%.2f\n", fabs(input));
    }
    
    return 0;
}