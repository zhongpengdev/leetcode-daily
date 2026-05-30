#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fix: 返回char* 而不是char[]
char* orderString(char string[]) {
    char temp = string[0];

    if(string[0] > string[1]) {
        char temp = string[0];
        string[0] = string[1];
        string[1] = temp; 
    }

    if(string[1] > string[2]) {
        char temp = string[1];
        string[1] = string[2];
        string[2] = temp; 
    }

    // fix: 这里需要的是0 1再做交换
    if(string[0] > string[1]) {
        char temp = string[0];
        string[0] = string[1];
        string[1] = temp; 
    }

    return string;
}


int main()
{
    // c中没有char[] 使用大数组
    char usersString[100];
    
    while (scanf("%s", usersString) != EOF)
    {
        char* orderedString = orderString(usersString);
        for (int i = 0; i < 3; i++)
        {
            printf("%c ",orderedString[i]);
        }
    }

    return 0;
}