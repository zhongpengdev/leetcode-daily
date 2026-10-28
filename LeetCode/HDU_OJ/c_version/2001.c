#include <stdio.h>
#include<math.h>

int main()
{
    double x1, x2, y1, y2;
    // double使用lf接收
    while (scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2) != EOF)
    {
        double diff_x_2 = pow((x2 - x1), 2);
        double diff_y_2 = pow((y2 - y1), 2);

        double result = sqrt(diff_x_2 + diff_y_2);

        printf("%.2f\n", result);
    }
    
    return 0;
}