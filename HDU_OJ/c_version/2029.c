#include <stdio.h>
#include <string.h>
/**
 * 回文串，双指针控制
 *
 *
 */

int isPalindromes(char str[])
{
    int left = 0;
    int right = strlen(str) - 1;
    while (left < right)
    {
        if (str[left] != str[right])
            return 0;

        left++;
        right--; // 是right--
    }
    return 1;
}

int main()
{

    int count;
    if (scanf("%d", &count) == 1)
    {
        char str[100];

        while (count--)
        {
            if (scanf("%s", str) == 1)
            {

                if (isPalindromes(str) == 1)
                    printf("yes\n");
                else
                    printf("no\n");
            }
        }
    }

    return 0;
}