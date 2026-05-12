# include<iostream>

/**
 * 1. heapify：将数组array[]调整成大顶堆
 * 
 * 2. 逐个取出每一个堆顶元素
 * 
 * 3. 将堆顶元素换到最后，最大值归位
 * 
 * 3. i-1 减小一个位置，排除掉最后的最大元素，继续heapify
 */

 /**
  * 时间复杂度：
  * 
  * 建堆过程：O(n)
  * 排序过程 log(n)+log(n-1)+...+log(1) = O(nlog(n))
  * 
  * 总共：O(nlog(n))
  * 
  * 空间复杂度：heapify递归：log(n) 迭代：O(1)
  * 
  * 不稳定
  * 
  */



/**
 * 
 * heapify()
 * 
 * largest 存i
 * 
 * array[i] 左节点：array[2*i+1], array[2*i+2]
 * 
 * largest = i > 左子 and 右子
 * 
 * 调整largest 
 */

 // 直接传array[] 地址
void heapify(int array[], int n, int i) {
    // for(int i = 0; i < n; i++) {
    //     int largest = i;

    //     if(array[largest] < array[2*i+1]) {
    //         std::swap(array[2*i+1], array[largest]);
    //     }

    //     if(array[largest] < array[2*i+2]) {
    //         std::swap(  array[2*i+2], array[largest]);
    //     }
    // }

    int largest = i;
    int right = 2 * i + 2;
    int left = 2 * i + 1;

    if(left < n && array[left] > array[i]) {
        largest = left;
    }

    if(right < n && array[right] > array[largest]) largest = right;

    // 交换元素
    if(largest != i) {
        std::swap(array[i], array[largest]);

        // 继续对子节点进行交换检查
        // largest是 当前交换后的，已经变动的节点
        heapify(array, n, largest);
    }    
}

/**
 * 迭代heapify
 */

 void iteration_heapify(int array[], int n, int i) {
    while (true)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if(left < n && array[left] > array[largest]) largest = left;

        if(right < n && array[right] > array[largest]) largest = right;

        if(largest == i) break;

        std::swap(array[i], array[largest]);
        i = largest;
    }
 }


/**
 * heap_sort() 
 * 
 * 传进去i，对前i个元素执行heapify
 * 
 * 取出堆顶元素
 * 
 * 堆顶元素和n-i-1元素交换。
 * 
 */
void heap_sort(int array[], int n) {
    // for(int i = 0; i < n; i++) {
    //     heapify(array, n, i);
    //     std::swap(array[i], array[n-i-1]);
    // }


    /**
     * 建堆：init一个堆
     * 从最后一个非叶节点往回建堆
     * 从n/2 到n-1都是叶子
     */
    for(int i = n / 2 - 1; i >= 0; i--) heapify(array, n, i);

    /**
     * 堆顶和array末尾元素末尾交换
     * 
     * 堆-1 重新heapify
     */
    for(int i = n-1; i > 0; i--) {
        std::swap(array[0], array[i]);

        heapify(array, i, 0); // 对前i个元素heapify
    }
}

int main() {
    int a[] = {5, 3, 1, 4, 2};
    int n = sizeof(a) / sizeof(a[0]);

    std::cout << "before: ";
    for (int i = 0; i < n; i++) std::cout << a[i] << " ";
    std::cout << std::endl;

    heap_sort(a, n);

    std::cout << "after:  ";
    for (int i = 0; i < n; i++) std::cout << a[i] << " ";
    std::cout << std::endl;

    return 0;
}