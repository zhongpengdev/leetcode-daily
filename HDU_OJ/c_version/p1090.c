#include<stdio.h>

int main() {
    //p1090
    int a, b, N;

    scanf("%d", &N);

    for (int i = 0; i < N; i++)
    {
        scanf("%d %d", &a, &b);
        printf("%d\n", a+b);
    }
    return 0;
}