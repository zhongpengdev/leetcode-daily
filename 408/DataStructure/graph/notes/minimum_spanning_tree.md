# 最小生成树（MST）

## 定义

无向连通带权图中，选取 n-1 条边连接所有 n 个顶点，使得总权值最小。

## 性质

- MST 不唯一（除非所有边权不同）。
- **切割性质：** 连接两个不相交集合的最小权边一定属于某个 MST。
- **回路性质：** 环中最大权边一定不属于 MST。

---

## 1. Kruskal 算法 - 从整体中选最小边

**核心思想：** 每次选一条**权值最小**且**不会形成环**的边，重复直到选出 n-1 条边。

### 代码

```c
struct Edge { int u, v, w; }; // 边的第一个端点 第二个端点 权重
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
**空间复杂度：** O(n)（并查集）

---

## 2. Prim 算法 - 选点，选距离已选顶点集合最小的的新点

**核心思想：** 从任意一个顶点出发，维护一个"已选顶点集合 S"，每次找到**连接 S 和未选集合的最小权边**，把对应的新顶点加入 S。

---

**关键问题：如何快速找到从已选集合到未选集合的最小权边？→ 优先队列（小根堆）+ 松弛操作。**

这与 Dijkstra 算法的区别：Dijkstra 更新的是**到起点的距离**，Prim 更新的是**到已选集合的距离**（即 min(当前记录值, 新顶点的连接边权)）。

### 代码

```c
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
- 朴素：O(n²)（每次遍历 n 个顶点找最小 key）
- 堆优化：O((n + m) log n)（每条边入堆一次，弹出 n 次）

**空间复杂度：** O(n + m)

---

## Kruskal vs Prim

| | Kruskal | Prim |
|---|---------|------|
| 数据结构 | 并查集 | 优先队列（堆） |
| 基于 | **边** | **顶点** |
| 典型策略 | 全局选最小边，不成环就加 | 从起点扩散，每次选已选区域的最短"触手" |
| 适合 | **稀疏图**（m 小） | **稠密图**（m 接近 n²） |
| 时间复杂度 | O(m log m) | O((n+m) log n) 堆优化 / O(n²) 朴素 |
| 空间复杂度 | O(n) | O(n+m) |

**选择建议：**
- 边少（稀疏图）→ **Kruskal**（排序代价低，并查集轻量）
- 边多（稠密图）→ **Prim 朴素版** O(n²)（不比堆差，且省去了 log 因子）
