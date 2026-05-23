# 最小生成树（MST）

## 定义

无向连通带权图中，选取 n-1 条边连接所有 n 个顶点，使得总权值最小。

## 性质

- MST 不唯一（除非所有边权不同）。
- **切割性质：** 连接两个不相交集合的最小权边一定属于某个 MST。
- **回路性质：** 环中最大权边一定不属于 MST。

---

## 1. Kruskal 算法

**思想：** 按边权从小到大排序，依次加入不成环的边（并查集维护）。

```
struct Edge { int u, v, w; };
bool cmp(Edge a, Edge b) { return a.w < b.w; }

int find(int x) {  // 并查集
    return parent[x] == x ? x : parent[x] = find(parent[x]);
}

int kruskal() {
    sort(edges.begin(), edges.end(), cmp);
    // 初始化并查集 parent[i] = i

    int total = 0, cnt = 0;
    for (auto [u, v, w] : edges) {
        int pu = find(u), pv = find(v);
        if (pu != pv) {
            parent[pu] = pv;
            total += w;
            cnt++;
            if (cnt == n - 1) break;
        }
    }
    return total;
}
```

**时间复杂度：** O(m log m)（排序主导）

---

## 2. Prim 算法

**思想：** 类似 Dijkstra，从任意顶点出发，每次选择连接已选集合和未选集合的最小权边。

```
int prim() {
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    vector<bool> inMST(n, false);
    int total = 0;

    pq.push({0, 0});  // {weight, vertex}

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;
        total += w;

        for (auto [v, wt] : adj[u]) {
            if (!inMST[v]) {
                pq.push({wt, v});
            }
        }
    }
    return total;
}
```

**时间复杂度：**
- 朴素：O(n²)
- 堆优化：O((n + m) log n)

---

## Kruskal vs Prim

| | Kruskal | Prim |
|---|---------|------|
| 数据结构 | 并查集 | 优先队列 |
| 基于 | 边 | 顶点 |
| 适合 | 稠密图 | 稀疏图 |
| 时间 | O(m log m) | O((n+m)logn) |
