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
# snippet

`Strl+Shift+P` -> snippet and open c.json or cpp.json

add temp:
```json
"OJ Template": {
      "prefix": "oj",
      "body": [
        "#include <stdio.h>",
        "#include <stdlib.h>",
        "#include <string.h>",
        "",
        "int main()",
        "{",
        "    ${0}",
        "",
        "    return 0;",
        "}"
      ],
      "description": "HDU OJ C template"
    }
```
in the c file and input `oj` and tab to make it work.


# basic grammar in c

```c
double x1, x2;
scanf("%lf %lf", &x1, &x2);
```

standard bool lib in c:
```c
#include<stdbool.h>
```



# basic resolving order by ai

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

基础题推荐刷题顺序
第一阶段（2天，10题）： 输入输出练习

1000、1089、1090、1091、1092、1093、1094、1095、1096、1001

第二阶段（2-4天，12题）： 简单运算

2000、2001、2002、2003、2004、2005、2006、2007、2008、2009、2010、2011、2039

第三阶段（4-6天，24题）： 一维数组、分支结构

2012-2030、2032、2040、2042、2054、2055

第四阶段： 简单字符串和找规律题

1004、1005、1008、1013、1019、1020、1021、1048、1062、1108