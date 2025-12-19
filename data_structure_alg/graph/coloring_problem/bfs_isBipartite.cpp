#include <queue>
#include <vector>
#include<iostream>

/// @brief 判断二分图
/// @param n 
/// @param adj 
/// @return bool
bool isBipartite(int n, std::vector<std::vector<int>> &adj)
{
    /*
    0：未染色
    1：红色
    -1：蓝色
    充当visited[]
    */
    std::vector<int> color(n, 0);

    for (int i = 0; i < n; i++)
    {
        if (color[i] != 0)
            continue;

        std::queue<int> q;
        q.push(i);
        color[i] = 1;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (int v : adj[u])
            {
                // 未染色
                if (color[v] == 0)
                {
                    color[v] = -color[u]; // 颜色与u相反
                    q.push(v);
                }
                else if (color[u] == color[v]) { //环中出现颜色相同的相邻节点
                    return false;
                }   
            }
        }
    }
    return true;
}

int main() {
    int n, m;
    std::cout << "请输入顶点数和边数: ";
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n);
    std::cout << "请输入每条边的两个顶点 (0 到 n-1):" << std::endl;
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // 无向图
    }

    if (isBipartite(n, adj)) {
        std::cout << "这是一个二分图 (无奇数环)" << std::endl;
    } else {
        std::cout << "这不是一个二分图 (存在奇数环)" << std::endl;
    }

    return 0;
}