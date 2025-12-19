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