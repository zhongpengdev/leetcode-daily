#include <stdio.h>
#include <stdbool.h>

// 判断闰年
bool isLeapYear(int year)
{
    // 能被4整除且不能被100整除，或能被400整除
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int main()
{
    // 2005
    int year, month, day;
    int days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31,
                    30, 31, 30, 31};

    while (scanf("%d/%d/%d", &year, &month, &day) != EOF)
    {
        if(isLeapYear(year)) days[2] = 29;
        else days[2] = 28;

        int sumDays = 0;

        for (int i = 1; i <= month - 1; i++)
        {
            sumDays += days[i];
        }

        sumDays += day;
        printf("%d\n", sumDays);
    }

    return 0;
}