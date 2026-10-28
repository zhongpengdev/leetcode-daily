/**
 *
 */
#include <iostream>
#include <queue>
#include <vector>
#include <string>

typedef struct HuffmanNode
{
    HuffmanNode *left;
    HuffmanNode *right;
    int value;
    HuffmanNode() {};
    HuffmanNode(int value) : value(value), left(nullptr), right(nullptr) {};
    HuffmanNode(HuffmanNode *left, HuffmanNode *right) : value(left->value + right->value), left(left), right(right) {};
};

struct Compare
{
    bool operator()(HuffmanNode *a, HuffmanNode *b)
    {
        return a->value > b->value ? true : false;
    }
};

// 字符转频率数组
std::vector<int> stringToFreqVector(const std::string &s)
{
    // 长度26的数组将每个
    int freq[26] = {0};
    // 预分配最大为s的结果数组
    std::vector<int> result;

    for (int i = 0; i < s.length(); i++)
    {
        freq[s[i] - 'a'] += 1;
    }

    for (int i = 0; i < 26; i++)
    {
        if (freq[i] != 0)
        {
            result.push_back(freq[i]);
        }
    }

    return result;
}

int buildAndCalculateCost(const std::vector<int> &nodes)
{
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, Compare> heap;

    HuffmanNode *root = nullptr;
    int cost = 0;

    for (int i = 0; i < nodes.size(); i++)
    {
        heap.push(new HuffmanNode(nodes[i]));
    }

    // 边界检查，如果只有一个元素或者没有元素，那么直接返回对应值
    if (heap.size() == 0)
        return 0;
    if (heap.size() == 1)
        return heap.top()->value;

    while (heap.size() > 1)
    {
        HuffmanNode *tempLeft = heap.top();
        heap.pop();

        HuffmanNode *tempRight = heap.top();
        heap.pop();

        // 创建父节点
        HuffmanNode *parent = new HuffmanNode(tempLeft, tempRight);

        cost += parent->value;

        // 将父节点加入heap
        heap.push(parent);
    }

    // 循环结束 只剩下根节点
    root = heap.top();

    return cost;
}

int main()
{
    int N;

    std::cin >> N;

    for (int i = 0; i < N; i++)
    {
        int n;
        std::string input_string;

        std::cin >> n >> input_string;

        int cost = buildAndCalculateCost(stringToFreqVector(input_string));

        if (cost <= n)
            std::cout << "yes" << "\n";
        else
            std::cout << "no" << "\n";
    }

    return 0;
}
