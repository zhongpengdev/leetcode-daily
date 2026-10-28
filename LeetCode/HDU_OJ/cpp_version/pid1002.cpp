/**
 *  Notice that the integers are very large,
 * that means you should not process them by using 32-bit integer.
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

// int main() {
//     // 1002
//     int n;
//     long long a, b;

//     std::cin >> n;

//     // use 1 to start i -> case1
//     for(int i = 1; i <= n; i++)
//     {
//         std::cin >> a >> b;
//         std::cout << "Case " << i << ":" << std::endl;
//         std::cout << a << " + " << b << " = " << a + b << std::endl;

//         // 除了最后一个case，都要换行
//         if(i != n) {
//             std::cout << std::endl;
//         }
//     }

//     return 0;
// }

string addBigIntegers(const string &num1, const string &num2)
{
    // 任意一个num为空，直接返回
    if (num1.empty())
        return num2;
    if (num2.empty())
        return num1;

    string result;

    // 内存预分配
    result.reserve(max(num1.length(), num2.length()) + 1);

    // 两个num的指针逻辑
    // 注意地址和真实数值是反的
    int pointer1 = num1.length() - 1;
    int pointer2 = num2.length() - 1;
    int carry = 0; // 进位

    // 竖式计算
    while (pointer1 >= 0 || pointer2 >= 0 || carry > 0)
    {
        int currentSum = carry; // 直接加上当前的进位

        if (pointer1 >= 0)
        {
            currentSum += num1[pointer1] - '0';
            pointer1--;
        }

        if (pointer2 >= 0)
        {
            currentSum += num2[pointer2] - '0';
            pointer2--;
        }

        carry = currentSum / 10; // 新carry的计算公式
        result.push_back((currentSum % 10) + '0');
    }

    // 将result逆序
    reverse(result.begin(), result.end());
    return result;
}

int main()
{
    // 优化标准输入输出
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n))
        return 0; // 直接返回

    for (int i = 1; i <= n; i++)
    {
        string bigint1, bigint2;
        cin >> bigint1 >> bigint2;

        cout << "Case " << i << ":\n";
        std::cout << bigint1 << " + " << bigint2 << " = "
                  << addBigIntegers(bigint1, bigint2) << "\n";

        if(i != n) {
            cout << "\n";
        }
    }

    return 0;
}