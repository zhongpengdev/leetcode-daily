#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // 1095
    int a, b;
    while (scanf("%d %d", &a, &b) != EOF) 
    {
        printf("%d", a+b);
        printf("\n");
    }
    
    return 0;
}