#include <stdio.h>
#include<ctype.h>

// HDI
int isLegalString(char* str) {
    // 边界处理，空字符串
    if(str == NULL || str[0] == '\0') return 0;

    if(!(isalpha(str[0]) || str[0] == '_')) return 0;

    for (int i = 1; str[i] != '\0'; i++)
    {
        // 剩下的字母
        if(!(isalnum(str[i]) || str[i] == '_')) return 0;
    }
    
    return 1;
}
int main()
{ 
    // HDU
    int count;
    char str[100];
    if (scanf("%d", &count) == 1)
    {
        getchar();
        while (count--)
        {
            // scanf("%s") 遇到空格会停止
            // 读取一行包含空格
            fgets(str, sizeof(str), stdin);

            // 去掉换行符
            str[strcspn(str, "\n")] = '\0';

            // 判断逻辑
            if(isLegalString(str)) printf("yes\n");
            else printf("no\n");
        }
    }

    return 0;
}