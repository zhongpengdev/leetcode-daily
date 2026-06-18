/*
折半二分查找demo
*/
#include<iostream>
#include<vector>

/// @brief 
/// @param array 
/// @param toFind 
/// @return 
int half_serach(const std::vector<int> &array, int toFind) { //！！使用const + 引用的方式！！
    int n = array.size();
    int left = 0, right = n-1;
    
    while(left <= right) {

        // int mid = left + (right - left) / 2
        int mid = (left + right) / 2;

        if(toFind < array[mid]) {
            right = mid - 1; //必须严格缩小，死循环
        } else if(toFind > array[mid]) {
            left = mid + 1; //严格缩小
        } else {
            return mid;
        }
    }

    return -1;
}

//递归 - 折半查找
int binarySearchRecursiveHelper(const std::vector<int>& array, int toFind, int left, int right) {
    int mid = left + (right - left) / 2;

    while(left < right) {
        if(toFind > array[mid]) {
            binarySearchRecursiveHelper(array, toFind, mid+1, right);
        } else if(toFind < array[mid]) {
            binarySearchRecursiveHelper(array, toFind, left, mid-1);
        } else {
            return mid;
        }
    }

    return -1;
}

//递归折半对外接口
int binarySearchRecursive(const std::vector<int>& array, int toFind) {
    return binarySearchRecursiveHelper(array, toFind, 0, array.size() - 1);
}