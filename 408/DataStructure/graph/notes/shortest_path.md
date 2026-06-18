# 最短路径算法

## 1. Dijkstra 算法

**适用：** 非负权图的单源最短路径。

**思想：** 贪心，每次从未确定最短路径的顶点中选择距离最小的，松弛其邻居。

```
void dijkstra(int start) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    fill(dist, dist + n, INF);
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;  // 懒删除

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}
```

**时间复杂度：**
- 朴素实现：O(n²)
- 堆优化：O((n + m) log n)

**关键：** 不能处理负权边。

---

## 2. Bellman-Ford 算法

**适用：** 可处理负权边，能检测负权环。

**思想：** 对所有边进行 n-1 轮松弛。

```
struct Edge { int u, v, w; };
vector<Edge> edges;

void bellman_ford(int start) {
    fill(dist, dist + n, INF);
    dist[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    // 检测负权环
    for (auto [u, v, w] : edges) {
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            // 存在负权环
        }
    }
}
```

**时间复杂度：** O(nm)

---

## 3. SPFA（Shortest Path Faster Algorithm）

Bellman-Ford 的队列优化版本。

```
void spfa(int start) {
    fill(dist, dist + n, INF);
    queue<int> q;
    vector<bool> inq(n, false);

    dist[start] = 0;
    q.push(start);
    inq[start] = true;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = false;

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
}
```

**时间复杂度：** 平均 O(km)，最坏 O(nm)

**注意：** 实际中可能被卡（精心构造数据退化为 O(nm)）。

---

## 4. Floyd-Warshall 算法

**适用：** 全源最短路径。

**思想：** 动态规划，枚举中间点 k，更新所有点对之间的最短距离。

```
int dist[MAXN][MAXN];

void floyd() {
    // 初始化 dist[u][v] 为直接边权，dist[i][i] = 0

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
}
```

**时间复杂度：** O(n³)

**空间：** O(n²)

**注意：** 可处理负权边，不能处理负权环（但可检测）。

---

## 对比总结

| 算法 | 单源/全源 | 负权 | 负环检测 | 时间复杂度 |
|------|----------|------|---------|-----------|
| Dijkstra | 单源 | 不支持 | 否 | O((n+m)logn) |
| Bellman-Ford | 单源 | 支持 | 是 | O(nm) |
| SPFA | 单源 | 支持 | 是 | O(km) 平均 |
| Floyd | 全源 | 支持 | 是 | O(n³) |
