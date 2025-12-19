#include <queue>
#include <vector>

std::priority_queue<int> pq; // 默认大顶堆

std::priority_queue<int, std::vector<int>, std::greater<int>> pq1; // 小顶堆

int main()
{
    pq.push(1); // 自动排序
    pq.pop(); //弹出堆顶元素

    pq.top(); //返回堆顶的引用，不弹出
    pq.empty();
    pq.size();
}
