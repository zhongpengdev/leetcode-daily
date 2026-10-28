#include <stdio.h>

// hahah
int main()
{
    int n;
    scanf("%d", &n);

    while (n--)
    {
        char str[10000];

        scanf("%s", str);

        int count = 1;
        char last_char = str[0];

        // 要让'\0'也进入循环，这样才能识别打印最后一个字符
        for(int i = 1; ; i++)
        {
            // 如果当前值和前一个char不相等,且包括了最后 '\0'的情况
            if (str[i] != last_char)
            {
                if (count == 1)
                {
                    printf("%c", last_char);
                }
                else
                {
                    // count不是1
                    printf("%d%c ", count, last_char);
                    count = 1;
                }
                // 更新lastchar
                last_char = str[i];

                if (str[i] == '\0')
                {
                    break;
                }
                
            }
            else
            {
                // 如果等于最后一个char
                count++;
            }
        }

        printf("\n");
    }

    return 0;
}