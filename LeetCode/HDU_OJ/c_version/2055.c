#include <stdio.h>

int main()
{
    int count;
    scanf("%d", &count);
    
    while (count--)
    {
        char x;
        int y, x_number;

        // 坑：前面加空格吸收上一个的换行符
        scanf(" %c %d", &x, &y);

        // 1. 判断x是大写还是小写
        // 2. 转换成对应的数字
        if (x >= 'a' && x <= 'z')
        {
            // 小写字母的话结果是负数
            x_number = -(x - 'a' + 1);
        } else {
            x_number = x - 'A' + 1;
        }

        printf("%d", y+x_number);
    }
    
    return 0;
}