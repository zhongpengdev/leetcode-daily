# include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);

    while (n--) {
        int a, b;
        scanf("%d %d", &a, &b);

        // 将a b 分别取低两位
        int a_again = a % 100;
        int b_again = b % 100;

        printf("%d\n", (a_again + b_again) % 100);
    }
}
