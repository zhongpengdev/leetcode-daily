/**
 *
 * 气球数量匹配问题
 *
 * 字符串匹配问题
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

// 接受一个vector<string>
// 使用一个map搭配字符串遍历记录次数
// 返回出现次数最多的那个字符串index
std::string mostFrequent(std::vector<std::string> &stringArray)
{
    std::unordered_map<std::string, int> count;

    for (std::string thisString : stringArray)
        count[thisString]++;

    // 问题1：比较器语法不是很了解
    return std::max_element(count.begin(), count.end(),
                            [](const auto &a, const auto &b)
                            { return a.second < b.second; })
        ->first;
}

int main()
{
    int N;
    while (std::cin >> N && N != 0)
    {
        std::vector<std::string> stringVector;

        for (int i = 0; i < N; i++)
        {
            std::string tempString;
            std::cin >> tempString;
            stringVector.push_back(tempString);
        }
        // 问题2：输出放在for外面
        std::string balloon = mostFrequent(stringVector);
        std::cout << balloon << "\n";
    }

    return 0;
}