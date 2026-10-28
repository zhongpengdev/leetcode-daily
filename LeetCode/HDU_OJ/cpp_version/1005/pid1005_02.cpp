/**
 * 不需要全部都存，直接替换就行
 * 
 * Time Limit Exceeded
 */

#include <iostream>
#include <vector>

// i 表示当前的i值从3开始
void calculate_next_value(std::vector<int> &array, int A, int B)
{
    // 拿到最后两个值并计算

    int f_n_1 = array[0];
    int f_n_2 = array.back();

    int f_next = (A * f_n_2 + B * f_n_1) % 7;

    array[0] = f_n_2;
    array[1] = f_next;
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
            calculate_next_value(array, A, B);
        }

        std::cout << array.back() << "\n";
    }
    return 0;
}
