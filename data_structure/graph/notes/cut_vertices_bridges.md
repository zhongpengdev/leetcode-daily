# 割点与桥

## 割点（Articulation Point）

**定义：** 删除该顶点后，连通分量数量增加。

**Tarjan 判断：**
- 根节点有 ≥2 个子树则为割点。
- 非根节点 u 满足 `low[v] >= dfn[u]`（v 是 u 的子节点）则 u 为割点。

---

## 桥（Bridge / Cut Edge）

**定义：** 删除该边后，连通分量数量增加。

**Tarjan 判断：** 边 (u, v)（u 是 v 的父节点）是桥，当且仅当 `low[v] > dfn[u]`。

```
void findBridges(int u, int parent) {
    dfn[u] = low[u] = ++timer;

    for (auto [v, w] : adj[u]) {
        if (!dfn[v]) {
            findBridges(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > dfn[u]) {
                // (u, v) 是桥
            }
        } else if (v != parent) {
            low[u] = min(low[u], dfn[v]);
        }
    }
}
```

---

## 关键区别

| | 割点 | 桥 |
|---|------|-----|
| 删除对象 | 顶点 | 边 |
| 判定条件 | low[v] >= dfn[u] | low[v] > dfn[u] |
| 影响 | 连通分量增加 | 连通分量增加 |
