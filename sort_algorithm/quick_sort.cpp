#include<iostream>

void quicksort(int a[], int l, int r) {
    if(l >= r) return;

    int i = l, j = r;

    int pivot = a[(i + j) / 2];

    while(i <= j) {
        while (a[i] < pivot) i++; // 左边 >= pivot的值
        while (a[j] > pivot) j--; // 右边 <= pivot的值

        if(i <= j) {
            std::swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    quicksort(a, l, j); // 左半部分
    quicksort(a, i, r); // 右半部分
}

int main() {
    int a[] = {9, 3, 1, 5, 13, 12};
    int n = sizeof(a) / sizeof(a[0]);
    quicksort(a, 0, n - 1);

    for (int x : a) std::cout << x << " ";
}
