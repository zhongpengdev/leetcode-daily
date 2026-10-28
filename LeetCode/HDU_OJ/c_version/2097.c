/**
 * 十进制 四位数字之和
 *
 * 十六进制之和
 *
 * 十二进制之和
 *
 * 都相等
 *
 */
#include <stdio.h>

int get_result(int num, int base) {
  int result = 0;

  while (num > 0) {
    result += num % base;
    num /= base;
  }

  return result;
}

int main() {
  int count;
  while (scanf("%d", &count) && count != 0) {

    int sum10 = get_result(count, 10);
    int sum12 = get_result(count, 12);
    int sum16 = get_result(count, 16);

    if (sum10 == sum12 && sum12 == sum16)
      printf("%d is a Sky Number.\n", count);
    else
      printf("%d is not a Sky Number.\n", count);
  }

  return 0;
}
