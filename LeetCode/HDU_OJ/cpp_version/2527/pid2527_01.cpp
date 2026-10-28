/**
 * 性能优化版
 */

#include <iostream>
#include <queue>
#include <vector>
#include <string>

int calculateCost(const std::string &s)
{
    // 统计频率
    int freq[26] = {0};
    for (int i = 0, len = s.length(); i < len; i++)
    {
        freq[s[i] - 'a']++;
    }

    // 小顶堆，greater<int> 让最小的在堆顶
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    for (int i = 0; i < 26; i++)
    {
        if (freq[i] > 0)
        {
            heap.push(freq[i]);
        }
    }

    // 边界处理
    if (heap.size() == 0)
        return 0;
    if (heap.size() == 1)
        return heap.top();

    // 模拟 Huffman 合并
    int cost = 0;
    while (heap.size() > 1)
    {
        int a = heap.top();
        heap.pop();
        int b = heap.top();
        heap.pop();
        int merged = a + b;
        cost += merged;
        heap.push(merged);
    }

    return cost;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;

    while (N--)
    {
        int m;
        std::string s;
        std::cin >> m >> s;
        std::cout << (calculateCost(s) <= m ? "yes" : "no") << "\n";
    }

    return 0;
}
