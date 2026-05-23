# 二分图

## 性质

- 二分图 **不存在奇数长度的环**。
- 可以用两种颜色给所有顶点染色，相邻顶点颜色不同。

---

## 染色法判断二分图

```
int color[MAXN];  // 0: 未染色, 1 和 2: 两种颜色

bool isBipartite(int start) {
    queue<int> q;
    color[start] = 1;
    q.push(start);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, w] : adj[u]) {
            if (color[v] == 0) {
                color[v] = 3 - color[u];  // 染另一种颜色
                q.push(v);
            } else if (color[v] == color[u]) {
                return false;  // 同色相邻，不是二分图
            }
        }
    }
    return true;
}
```

---

## 二分图最大匹配（匈牙利算法）

**问题：** 在二分图中找到最多的匹配边数。

```
// 左集合 X, 右集合 Y
// matchY[v] 表示右集合顶点 v 匹配的左集合顶点
int matchY[MAXN];
bool visited[MAXN];

bool bpm(int u) {  // 增广路径
    for (auto [v, w] : adj[u]) {
        if (!visited[v]) {
            visited[v] = true;
            if (matchY[v] == -1 || bpm(matchY[v])) {
                matchY[v] = u;
                return true;
            }
        }
    }
    return false;
}

int hungarian() {
    fill(matchY, matchY + n, -1);
    int result = 0;
    for (int u = 0; u < n; u++) {
        fill(visited, visited + n, false);
        if (bpm(u)) result++;
    }
    return result;
}
```

**时间复杂度：** O(n × m)

---

## 应用

- 任务分配
- 匹配问题（如稳定婚姻问题）
- 最小点覆盖（König 定理：最大匹配数 = 最小点覆盖数）
- 最大独立集（最大独立集 = n - 最大匹配数）
