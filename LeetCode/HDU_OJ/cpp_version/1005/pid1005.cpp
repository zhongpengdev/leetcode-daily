/**
 * 1005
 * 
 * Memory Limit Exceeded
 */

#include <iostream>
#include <vector>

// i 表示当前的i值从3开始
void calculate_next_value(std::vector<int> &array, int A, int B, int i)
{
    // 拿到最后两个值并计算

    // 那么对应出来的index下标应该是i-2 i-3
    int f_n_1 = array[i - 2];
    int f_n_2 = array[i - 3];

    int f_next = (A * f_n_1 + B * f_n_2) % 7;

    array.push_back(f_next);
}

int main()
{
    int A, B, n;
    while (std::cin >> A >> B >> n && (A != 0 || B != 0 || n != 0))
    {
        std::vector<int> array;
        array.push_back(1);
        array.push_back(1);

        for (int i = 3; i <= n; i++)
        {
            calculate_next_value(array, A, B, i);
        }

        std::cout << array.back() << "\n";
    }
    return 0;
}
