#include <stdio.h>
#include <string.h>

int main()
{
    char str[100];

    // scanf的判断是EOF
    while (fgets(str, sizeof(str), stdin) != NULL)
    {

        str[strcspn(str, "\n")] = '\0';

        // str[0] = str[0] - 32;
        // 是将每个单词的首字母变成大写

        // 首先处理第一个开头单词
        // 坑：先判断是不是先写字母
        if (str[0] >= 'a' && str[0] <= 'z') {
            str[0] = str[0] - 32;
        }

        for (int i = 1; i < strlen(str)-1; i++)
        {
            // 当前字符是空格，下一个字符不是空格
            if (str[i] == ' ' && str[i+1] >= 'a' && str[i+1] <= 'z') {
                str[i+1] = str[i+1] - 32;
            }
        }
        
        printf("%s\n", str);
    }
    
    return 0;
}