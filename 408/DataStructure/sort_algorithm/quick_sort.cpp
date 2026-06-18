#include<iostream>

/**
 * 1. 随机抓一个学生 pivot
 * 
 * 2. 老师在最左侧 i，找比pivot高的人 助教在最右侧 j 找比pivot低的人
 * 
 * 3. 老师往左走，助教往右走 遇到了就交换两个人 
 * 
 * 4. 交换完一轮之后pivot左侧全是比 pivot 低的人 右边全是比pivot高的人
 * 
 * 5. quicksort(array, l, j)  递归左半边
 * 
 * 6. quick(array, i, r) 递归右半边
 */



 /**
  * 
  * QuickSort不稳定，且仅适用于顺序存储的线性表
  * 
  * 时间复杂度：
  * 
  * 最坏情况：两个区域：一个有n-1个元素，另一个有0个元素
  * 
  * 最坏下：O(n^2)
  * 
  * 时间复杂度是：O(nlogn)
  * 
  * 空间复杂度：递归栈的深度
  * 
  * 最好情况：O(n)
  * 
  * 最坏情况：O(logn) 
  * 
  * 平均：O(logn)
  */
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

// 双基准快排 (Dual Pivot QuickSort) 详解见 notes/quicksort/quicksort.md
