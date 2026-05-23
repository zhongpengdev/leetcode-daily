# 拓扑排序（Topological Sort）

## 定义

**适用：** 有向无环图（DAG）。

将所有顶点排成线性序列，使得对于每条边 <u, v>，u 在序列中位于 v 之前。

---

## Kahn 算法（BFS）

```
vector<int> topo_sort() {
    vector<int> indegree(n, 0);
    for (int u = 0; u < n; u++)
        for (auto [v, w] : adj[u])
            indegree[v]++;

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indegree[i] == 0) q.push(i);

    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (auto [v, w] : adj[u]) {
            if (--indegree[v] == 0)
                q.push(v);
        }
    }

    if ((int)order.size() != n) {
        // 存在环，无法完成拓扑排序
    }
    return order;
}
```

---

## DFS 实现

```
bool visited[MAXN];
vector<int> order;

void dfs_topo(int u) {
    visited[u] = true;
    for (auto [v, w] : adj[u])
        if (!visited[v]) dfs_topo(v);
    order.push_back(u);  // 后序逆序即拓扑序
}

// 调用后 reverse(order.begin(), order.end());
```

---

## 时间复杂度

O(n + m)

---

## 应用

- 判断 DAG 是否有环
- 任务调度、课程安排
- 关键路径分析
