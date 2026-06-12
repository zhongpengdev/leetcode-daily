#include<iostream>
using namespace std;

void InsertOrder(int A[], int n) {
    int i, j;

    for(i = 2; i <= n; i++) {
        if(A[i] < A[i-1]) {
            A[0] = A[i];
            for(j = i - 1; A[0] < A[j]; j--) {
                A[j+1] = A[j]; 
            }
            A[j+1] = A[0];
        }
    }
}

// 打印数组函数
void PrintArray(int A[], int n) {
    for(int i = 1; i <= n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;
}

int main() {
    // 测试用例1：普通数组
    cout << "测试1：普通数组" << endl;
    int arr1[] = {0, 5, 2, 4, 6, 1, 3};
    int n1 = 6;
    cout << "排序前: ";
    PrintArray(arr1, n1);
    InsertOrder(arr1, n1);
    cout << "排序后: ";
    PrintArray(arr1, n1);
    cout << endl;
    
    // 测试用例2：已排序数组
    cout << "测试2：已排序数组" << endl;
    int arr2[] = {0, 1, 2, 3, 4, 5, 6};
    int n2 = 6;
    cout << "排序前: ";
    PrintArray(arr2, n2);
    InsertOrder(arr2, n2);
    cout << "排序后: ";
    PrintArray(arr2, n2);
    cout << endl;
    
    // 测试用例3：逆序数组
    cout << "测试3：逆序数组" << endl;
    int arr3[] = {0, 6, 5, 4, 3, 2, 1};
    int n3 = 6;
    cout << "排序前: ";
    PrintArray(arr3, n3);
    InsertOrder(arr3, n3);
    cout << "排序后: ";
    PrintArray(arr3, n3);
    cout << endl;
    
    // 测试用例4：包含重复元素
    cout << "测试4：包含重复元素" << endl;
    int arr4[] = {0, 3, 3, 1, 2, 3, 1};
    int n4 = 6;
    cout << "排序前: ";
    PrintArray(arr4, n4);
    InsertOrder(arr4, n4);
    cout << "排序后: ";
    PrintArray(arr4, n4);
    cout << endl;
    
    // 测试用例5：单个元素
    cout << "测试5：单个元素" << endl;
    int arr5[] = {0, 42};
    int n5 = 1;
    cout << "排序前: ";
    PrintArray(arr5, n5);
    InsertOrder(arr5, n5);
    cout << "排序后: ";
    PrintArray(arr5, n5);
    cout << endl;
    
    return 0;
}