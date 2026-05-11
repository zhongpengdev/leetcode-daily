#include<iostream>

/**
 * 堆排序 (Heap Sort)
 *
 * 思路：把数组看成一棵完全二叉树，建一个大顶堆（最大的在根），
 *       每次把根（最大值）和末尾交换，缩小堆，重复。
 *
 * 两个核心操作：
 *   1. 建堆：从最后一个非叶子节点开始，逐个向下调整（heapify）
 *   2. 排序：交换堆顶和末尾 → 堆大小-1 → 对堆顶heapify
 *
 * 时间复杂度：O(n log n) —— 建堆 O(n)，排序 n次 × 每次heapify O(log n)
 * 空间复杂度：O(1) —— 就地排序
 * 稳定性：不稳定。交换堆顶和末尾可能打乱相等元素的顺序。
 */

// 向下调整：以 i 为根，维护大顶堆性质，堆大小为 n
void heapify(int a[], int n, int i) {
    int largest = i;        // 假设根最大
    int left = 2 * i + 1;   // 左孩子
    int right = 2 * i + 2;  // 右孩子

    if (left < n && a[left] > a[largest]) largest = left;
    if (right < n && a[right] > a[largest]) largest = right;

    if (largest != i) {
        std::swap(a[i], a[largest]);
        heapify(a, n, largest); // 交换后下面可能乱了，继续往下调
    }
}

void heapSort(int a[], int n) {
    // 第一步：建大顶堆
    // 从最后一个非叶子节点开始（n/2 - 1），往上逐个调整
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(a, n, i);
    }

    // 第二步：排序
    // 每次把堆顶（最大值）换到末尾，缩小堆，重新调整
    for (int i = n - 1; i > 0; i--) {
        std::swap(a[0], a[i]); // 最大值放到末尾
        heapify(a, i, 0);      // 对缩小后的堆调整堆顶
    }
}

int main() {
    int a[] = {9, 3, 7, 1, 8, 2, 5, 4, 6};
    int n = sizeof(a) / sizeof(a[0]);

    heapSort(a, n);

    for (int x : a) std::cout << x << " ";
}
