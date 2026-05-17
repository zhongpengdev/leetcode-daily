/**
 * 二叉树的层序遍历
 * 使用队列做存储
 * 
 * 
 * Q1： 最初的root边界检查
 * Q2：入队的条件
 */

# include<iostream>
# include"TreeNode.h"
# include<queue>

void level_order_traversal(TreeNode* root) {
    std::queue<TreeNode*> queue;
    
    //边界检查
    if(root == nullptr) return; 
    
    queue.push(root);

    while (!queue.empty())
    {
        TreeNode* temp = queue.front();

        std::cout << temp->value << std::endl;

        //左右子节点入队
        if(temp->left) {
            queue.push(temp->left);
        }
        if(temp->right) {
            queue.push(temp->right);
        }

        // 弹出队首元素
        queue.pop();
    }
}
