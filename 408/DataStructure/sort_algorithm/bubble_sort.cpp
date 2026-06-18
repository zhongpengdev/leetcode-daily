//bubble_sort()

#include<vector>

void bubble_sort(std::vector<int> &array) {
    int n = array.size();

    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i+1; j++) {
            if(array[j] > array[j+1]) {
                std::swap(array[j], array[j+1]);
            }
        }
    }
}

/**
 * 最好：array有序：比较n-1次 移动0次 O(n)
 * 最坏：array逆序
 * 平均复杂度O(n^2)
 */

void bubble_sort_enhanced(std::vector<int> &array) {
    int n = array.size();
    
    for(int i = 0; i < n-1; i++) {
        bool swapped_flage = false;
        
        for(int j = 0; j < n-i-1; j++) {
            if(array[j] < array[j+1]) {
                std::swap(array[j], array[j+1]);
                swapped_flage = true;
            }
        }

        if(swapped_flage == false) {
            return;
        }
    }
} 