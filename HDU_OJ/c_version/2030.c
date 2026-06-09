#include <stdio.h>
#include <string.h>


int countOfChinese(char str[]) {
    int sum = 0;

    for (int i = 0; i < (int)strlen(str); i++)
    {
        unsigned char stri = (unsigned char)str[i];
        if (stri > 0x80 && stri <= 0xEF) 
        {
            sum++;
            i += 1; // 自动跳过一个字节。
        }
    }
    return sum;
}
int main()
{
    int count;
    if(scanf("%d", &count) == 1) {

        getchar();
        
        char str[10005];
        while (count--)
        {
            // 使用scanf遇到空格截止 scanf("%s", str) == 1
            if(scanf("%1000[^\n]%*c", str) == 1) {
                printf("%d\n", countOfChinese(str));
            }
        }
    }

    return 0;
}