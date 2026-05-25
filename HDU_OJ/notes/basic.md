# Writing basic knowledge of HDU OJ

## EOF

EOF(End Of File) which defined in `<stdio.h>` and the value of it is -1 usually (a specific number).

and `!= EOF` means the input has reached the end of the file.

```c
# include<stdio.h>
// when scanf read two numbers, it will return 2 and finall -1 at the end
while(scanf("%d, %d", &a, &b) != EOF) { 
    // execute the code here until the end of file
}
```

```cpp
while (cin >> a >> b); // Internally determine EOF by C++ itself
```

# Classify

1000 - 1399 号题。 这是最经典的黄金题区。这里没有按技术点分类，而是混杂在一起。但其中包含大量考研/大厂最爱考的经典数据结构基础题。

HDU 1004 (Let the Balloon Rise)：哈希映射/字符串处理（基础）。

HDU 1005 (Number Sequence)：序列递推与周期性（线性表应用）。

HDU 1008 (Elevator)：简单模拟，线性逻辑。

HDU 1702 (ACBOY I): 栈（Stack）与队列（Queue）的模拟，实证 11408 里的 FIFO 和 LIFO 原理。

HDU 1710 (Binary Tree Traversals)：考研 100% 必考点。已知前序遍历和中序遍历，求后序遍历。直接用代码重塑 11408 的二叉树构造逻辑。

HDU 2527 (Safe Or Unsafe)：哈夫曼树（Huffman Tree）与哈夫曼编码，计算最小带权路径长度（WPL）。

HDU 1232 (畅通工程)：并查集（Disjoint Set）的终极入门题。图的连通分量求解，大厂底层微服务拓扑和 K8s 网络计算的根基。

HDU 1863 (畅通工程)：最小生成树。强练 Prim 算法和 Kruskal 算法。

HDU 2544 (最短路)：最短路径。手写 Dijkstra 算法和 Floyd 算法。

HDU 3791 (二叉搜索树)：输入两组序列，判断它们构造出的二叉搜索树（BST）是否是同一棵。

HDU 1166 (敌兵布阵)、HDU 1754 (I Hate It)：杭电镇站之宝，线段树（Segment Tree）入门。虽然 11408 不直接考线段树代码，但这是互联网大厂压榨区间查询性能的高级数据结构事实标准。

HDU 1040 (As Easy As A+B)：基础排序。拿它去练熟手写快排（Quick Sort）和堆排（Heap Sort）。