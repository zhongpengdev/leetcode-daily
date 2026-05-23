# 图的遍历：DFS 与 BFS

## 深度优先搜索（DFS）

沿一条路径尽可能深入，遇到死胡同后回溯。

### 递归版本

```c
bool visited[MAXN];

void dfs(int u) {
    visited[u] = true;
    // 处理当前节点
    for (auto [v, w] : adj[u]) { // 只访问边
        if (!visited[v]) {
            dfs(v);
        }
    }

    // 或者这样解构pair
    for(auto neighbor : adj[u]) {
        int u = neighbor.first();
        int v = neighbor.second();
        
        if(!visited[v]) {
            dfs(v); // 递归访问
        }
    }
}
```

### 迭代版本（栈）

```c
void dfs_iterative(int start) {
    stack<int> st;
    st.push(start);
    visited[start] = true;

    while (!st.empty()) {
        int u = st.top(); st.pop();
        // 处理 u
        for (auto [v, w] : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                st.push(v);
            }
        }
    }
}
```

### DFS 的应用

- 判断连通性
- 寻找路径
- 检测环
- 拓扑排序
- 求连通分量
- 割点与桥的判定

### 时间复杂度

O(n + m)，n 为顶点数，m 为边数。

---

## 广度优先搜索（BFS）

逐层扩展，先访问距离起点近的节点。

```
void bfs(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        // 处理 u
        for (auto [v, w] : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
```

### BFS 的应用

- 无权图最短路径
- 层序遍历
- 判断二分图
- 求连通分量
- 迷宫最短路

### 时间复杂度

O(n + m)

---

## DFS vs BFS 对比

| 特性 | DFS | BFS |
|------|-----|-----|
| 数据结构 | 栈（递归或显式） | 队列 |
| 空间 | O(n) 最坏 | O(n) 最坏 |
| 最短路径 | 不保证（无权图） | 保证（无权图） |
| 适合场景 | 路径搜索、连通性 | 最短路径、层序 |
