#include<iostream>

/**
 * 选择排序 (Selection Sort)
 *
 * 思路：每一轮从未排序的部分里选出最小的，放到已排序部分的末尾。
 *
 * 时间复杂度：不管什么情况都是 O(n²) —— 每轮都要扫一遍找最小值
 * 空间复杂度：O(1) —— 就地排序
 * 稳定性：不稳定。比如 [5, 5, 1]，第一个5会被换到后面去。
 */

void selectionSort(int a[], int n) {
    for(int i = 0; i < n-1; i++) {
        int min = i;
        for(int j = i+1; j < n; j++) {
            if(a[j] < a[min]) min = j;
        }
        if(min != i) std::swap(a[i], a[min]);
    }
}

int main() {
    int a[] = {5, 3, 1, 4, 2};
    int n = sizeof(a) / sizeof(a[0]);

    selectionSort(a, n);

    for (int x : a) std::cout << x << " ";
}
