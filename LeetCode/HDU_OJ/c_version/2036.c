/**
 * 给定n边形。以及n个坐标求其面积
 *
 * 鞋带公式
 */
#include <math.h>
#include <stdio.h>

// 计算多边形面积（顶点按顺时针或逆时针顺序给出）
double polygonArea(int n, int x[], int y[]) {
  double sum = 0.0;

  for (int i = 0; i < n; i++) {
    int j = (i + 1) % n; // 下一个顶点，最后一个的下一个是第一个
    sum += x[i] * y[j] - x[j] * y[i];
  }

  return fabs(sum) / 2.0;
}

int main() {
  int n;
  int x[100], y[100];

  while (scanf("%d", &n) == 1 && n != 0) {
    for (int i = 0; i < n; i++) {
      scanf("%d %d", &x[i], &y[i]);
    }

    double area = polygonArea(n, x, y);
    printf("%.1f\n", area);
  }

  return 0;
}
