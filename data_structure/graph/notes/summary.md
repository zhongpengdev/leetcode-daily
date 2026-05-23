# 图论总结

## 常见问题与算法速查

| 问题类型 | 常用算法 |
|---------|---------|
| 无权图最短路 | BFS |
| 非负权最短路 | Dijkstra |
| 负权最短路 | Bellman-Ford / SPFA |
| 所有点对最短路 | Floyd |
| 最小生成树 | Kruskal / Prim |
| 判断有向图环 | DFS / 拓扑排序 |
| 判断无向图环 | 并查集 / DFS |
| 强连通分量 | Tarjan / Kosaraju |
| 二分图判断 | 染色法 |
| 二分图匹配 | 匈牙利算法 |
| 割点/桥 | Tarjan |
| 欧拉路径 | Hierholzer |
| 最大流 | Edmonds-Karp / Dinic |

---

## 复杂度总结

| 算法 | 时间复杂度 | 空间复杂度 |
|------|-----------|-----------|
| DFS | O(n+m) | O(n) |
| BFS | O(n+m) | O(n) |
| Dijkstra (堆) | O((n+m)logn) | O(n+m) |
| Bellman-Ford | O(nm) | O(n+m) |
| Floyd | O(n³) | O(n²) |
| Kruskal | O(m log m) | O(n+m) |
| Prim (堆) | O((n+m)logn) | O(n+m) |
| 拓扑排序 | O(n+m) | O(n+m) |
| Tarjan SCC | O(n+m) | O(n+m) |
| 匈牙利 | O(n×m) | O(n+m) |
| Edmonds-Karp | O(nm²) | O(n²) |

其中 n 为顶点数，m 为边数。
