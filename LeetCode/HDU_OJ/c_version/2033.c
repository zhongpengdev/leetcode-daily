/**
 * 一个二维数组存两个值的对应
 * 
 * 进位标识，如果超出了60 flag就标记成1 
 * 
 * 结果数组
 */

#include <stdio.h>
int main()
{
    int count;
    if (scanf("%d", &count) == 1)
    {
        while (count--)
        {
            int array[2][3];
            int flag = 0;

            // 两个输入
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    scanf("%d", &array[i][j]);
                }
            }

            int S = array[0][2] + array[1][2];
            if (S >= 60) // 坑：应该是 >= 60
            {
                // S大于60 那么flag 为 S / 60 余数为S
                flag = S / 60;
                S = S % 60;
            }
            
            // M先直接加上flag再做判断
            int M = array[0][1] + array[1][1] + flag;
            
            if (M >= 60)
            {
                // 重复对flag赋值，用else将flag置0
                flag = M / 60;
                M %= 60;
            }else {
                flag = 0;
            }
            
            int H = array[0][0] + array[1][0] + flag;   

            printf("%d %d %d\n", H, M, S);
        }
    }

    return 0;
}