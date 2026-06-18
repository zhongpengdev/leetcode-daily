#include <iostream>
#include <climits>
#include <vector>

const int INF = INT_MAX; // 无穷大举例

/// @brief 邻接矩阵实现Prim算法 O(V*(2V))
/// @param n
/// @param graph
void prim(int n, const std::vector<std::vector<int>> &graph)
{
    // lowCost[i]: 顶点i到当前节点的最短距离
    std::vector<int> lowCost(n, INF);

    std::vector<int> visited(n, false);

    // 起始节点
    lowCost[0] = 0;
    // 权值和
    int totalWeight = 0;

    // 每次找到一个离树最近的点，找n次
    for (int i = 0; i < n; i++)
    {
        int u = -1; // 未访问的顶点中最近的点
        int minDintance = INF;

        // 找lowCost中最近的点，并加入树
        for (int v = 0; v < n; v++)
        {
            if (!visited[v] && lowCost[v] < minDintance)
            {
                minDintance = lowCost[v];
                u = v;
            }
        }

        if (u == -1)
        {
            // 图不连通
            return;
        }

        // 将u加入生成树
        visited[u] = true;
        totalWeight += lowCost[u];

        // 根据新的点更新lowCost
        for (int v = 0; v < n; v++)
        {
            if (!visited[v] && graph[u][v] != INF && graph[u][v] < lowCost[v])
            {
                lowCost[v] = graph[u][v];
            }
        }
    }

    std::cout << "最小生成树的权值为" << totalWeight << std::endl;
}











#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// 定义边：{目标节点, 权值}
typedef pair<int, int> PII;

struct Edge
{
    int to;
    int weight;
};

/**
 * Prim 算法函数
 * @param n 顶点数
 * @param adj 邻接表
 * @return 最小生成树的总权重
 */
int prim(int n, const vector<vector<PII>> &adj)
{
    // 标记节点是否已加入 MST
    vector<bool> visited(n + 1, false);
    // 存储最小生成树的总权值
    int mst_weight = 0;
    // 已加入 MST 的边数
    int edge_count = 0;

    // 优先队列（小根堆），存储：{权值, 目标节点}
    // 注意：pair 默认按第一个元素排序，所以权值放前面
    priority_queue<PII, vector<PII>, greater<PII>> pq;

    // 从 1 号节点开始（假设节点编号 1 到 n）
    pq.push({0, 1});

    while (!pq.empty() && edge_count < n)
    {
        PII top = pq.top();
        pq.pop();

        int w = top.first;
        int u = top.second;

        // 如果节点已经访问过，跳过
        if (visited[u])
            continue;

        // 正式加入 MST
        visited[u] = true;
        mst_weight += w;
        edge_count++;

        // 遍历 u 的所有邻居
        for (auto &edge : adj[u])
        {
            int v = edge.first;
            int weight = edge.second;
            if (!visited[v])
            {
                pq.push({weight, v});
            }
        }
    }

    // 如果加入的节点数小于 n，说明图不连通
    return (edge_count == n) ? mst_weight : -1;
}

int main()
{
    int n, m;
    cout << "请输入顶点数和边数: ";
    cin >> n >> m;

    vector<vector<PII>> adj(n + 1);
    cout << "请输入每条边的起点、终点和权值:" << endl;
    for (int i = 0; i < m; ++i)
    {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // 无向图
    }

    int result = prim(n, adj);

    if (result == -1)
    {
        cout << "图不连通，无法生成最小生成树。" << endl;
    }
    else
    {
        cout << "最小生成树的总权值为: " << result << endl;
    }

    return 0;
}