#include <stdio.h>
#include<stdlib.h>

// 在开头加注释可以跳过403

int gcd(int a, int b) {
    // haha
    while (b)
    {
        int temp = a % b;
        a = b;
        b = temp;
    }

    return a;
}

int lcm(int a, int b) {
    // haha
    if (a == 0 || b == 0)
    {
        return 0;
    }

    return abs(a / gcd(a, b) * b); 
    
}

int main()
{
    // haha
    int a, b;
    while(scanf("%d %d", &a, &b) != EOF){
        printf("%d\n", lcm(a, b));
    }     

    return 0;
}