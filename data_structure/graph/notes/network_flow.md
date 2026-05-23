# 网络流（基础）

## 基本概念

- **流网络**：有向图，每条边有容量 c(e) ≥ 0，有源点 s 和汇点 t。
- **可行流**：满足容量限制和流量守恒的流量分配。
  - 容量限制：0 ≤ f(e) ≤ c(e)
  - 流量守恒：除 s 和 t 外，每个顶点流入 = 流出
- **最大流**：从 s 到 t 的最大流量值。

---

## 增广路径与残余网络

- **残余网络**：原图中每条边 e 反向边 e'，残余容量 r(e) = c(e) - f(e)，反向边 r(e') = f(e)。
- **增广路径**：残余网络中 s 到 t 的路径，沿此路径可增加流量。

---

## Ford-Fulkerson 方法

重复在残余网络中找增广路径并增广，直到不存在增广路径。

---

## Edmonds-Karp 算法

用 BFS 找增广路径（最短路径）。

```
int n;
int capacity[MAXN][MAXN];
int parent[MAXN];

bool bfs(int s, int t) {
    fill(parent, parent + n, -1);
    queue<int> q;
    q.push(s);
    parent[s] = s;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n; v++) {
            if (parent[v] == -1 && capacity[u][v] > 0) {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int maxFlow(int s, int t) {
    int flow = 0;
    while (bfs(s, t)) {
        int aug = INF;
        for (int v = t; v != s; v = parent[v])
            aug = min(aug, capacity[parent[v]][v]);
        for (int v = t; v != s; v = parent[v]) {
            capacity[parent[v]][v] -= aug;
            capacity[v][parent[v]] += aug;
        }
        flow += aug;
    }
    return flow;
}
```

**时间复杂度：** O(n × m²)

---

## 最大流最小割定理

最大流值 = 最小割容量。

**割（Cut）：** 将顶点分为 S 和 T 两个集合（s ∈ S, t ∈ T），割的容量是所有从 S 到 T 的边的容量之和。
