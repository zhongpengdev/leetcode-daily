# 强连通分量（SCC）

## 定义

有向图中极大的强连通子图（任意两点互相可达）。

---

## Kosaraju 算法

1. 对原图做 DFS，记录完成时间（后序）。
2. 构建原图的**转置图**（所有边反向）。
3. 按完成时间**逆序**在转置图上做 DFS，每次 DFS 访问的顶点构成一个 SCC。

**时间复杂度：** O(n + m)

---

## Tarjan 算法

**核心：** 一次 DFS 同时维护两个数组：
- `dfn[u]`：DFS 访问 u 的时间戳（编号）。
- `low[u]`：u 能回溯到的最小编号（通过子树中的一条后向边）。

```
int dfn[MAXN], low[MAXN], timer = 0;
stack<int> st;
bool onStack[MAXN];
int scc_count = 0;

void tarjan(int u) {
    dfn[u] = low[u] = ++timer;
    st.push(u);
    onStack[u] = true;

    for (auto [v, w] : adj[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (onStack[v]) {
            low[u] = min(low[u], dfn[v]);
        }
    }

    if (dfn[u] == low[u]) {
        // u 是一个 SCC 的根
        scc_count++;
        int v;
        do {
            v = st.top(); st.pop();
            onStack[v] = false;
            // v 属于当前 SCC
        } while (v != u);
    }
}
```

**时间复杂度：** O(n + m)

---

## Kosaraju vs Tarjan

| | Kosaraju | Tarjan |
|---|----------|--------|
| 遍历次数 | 2 次 DFS | 1 次 DFS |
| 额外结构 | 转置图 | 栈 |
| 常数 | 较大 | 较小 |
| 实现难度 | 较简单 | 稍复杂 |
