/**
 * 数据可能非常的长，使用char接受
 */
// test
#include <stdio.h>
// HDU
int my_strcmp(char *a, char *b) {
    // test
  int i;
  while (a[i] != '\0' && b[i] != '\0') {
    if (a[i] != b[i])
      return 1;
    i++;
  }

  // 都到了最末尾才返回0
  if (a[i] == '\0' && b[i] == '\0')
    return 0;

  return 1;
}

int main() {
    // test
  char a[1000], b[1000];

  while (scanf("%s %s", a, b) != EOF) {

    if (my_strcmp(a, b) == 0) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }
    // if (strcmp(a, b) == 0) {
    //   printf("YES\n");
    // } else
    //   printf("NO\n");
  }
  return 0;
}
