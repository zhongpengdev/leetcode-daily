/**
 * 哈夫曼树的创建
 * 
 * 每次选出两个最小的元素组成新的一个节点
 * 
 * 新节点的左右孩子就是刚才的那两个节点，顺序无所谓
 * 直到最后只剩下一个节点
 * 
 * 1. 数据结构：快速找到最小节点：小顶堆 priority_queue
 * 
 * 2. 每个哈夫曼节点的解构：左右孩子的指针，当前节点的值val，父节点指针（不需要）
 * 
 * 3. 递归终止条件：堆里只剩下一个根节点
 */

# include<iostream>
# include "HuffmanNode.h"
# include<queue>
# include<vector>

/**
 * 1. createMinHeap() 创建小顶堆
 * 
 * 2. updateMinheap()更新小顶堆
 * 
 * 1. 2. 是priority_queue自动维护的
 * 
 * 3. buildFuffmanTree() 每次取出堆顶的两个元素生成一个新的元素放回MinHeap
 */

// 坑：std::vector<int> nodes是值传递 -> const std::vector<int>& nodes
HuffmanNode* buildHuffmanTree(const std::vector<int>& nodes) {
    /**
     * 1. 维护一棵结果树
     * 
     * 2. 维护一个小顶堆，每次存HUffmanNode
     * 
     * 2. while()取出最小的两个
     * 
     */

     std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> heap;

     HuffmanNode* root = nullptr;

     // 创建最小堆
     // 坑：这里不应该使用sizeof(array)这里的array是指针，得到的是指针的长度
     // 使用 vector<int> 或者传入一个n
     for(int i = 0; i < nodes.size(); i++) { 
        heap.push(new HuffmanNode(nodes[i]));
     }

     /**
      * 创建的过程是倒着创建的
      * 
      * while的条件是 heap.size() > 1直接取大小就好
      */
     while (heap.size() > 1)
     {
        HuffmanNode* tempLeft = heap.top();
        heap.pop();

        HuffmanNode* tempRight = heap.top();
        heap.pop();

        // 创建父节点
        HuffmanNode* parent = new HuffmanNode(tempLeft, tempRight);

        // 将父节点加入heap
        heap.push(parent);
     }

     // 循环结束 只剩下根节点
     root = heap.top();

     return root;
}

