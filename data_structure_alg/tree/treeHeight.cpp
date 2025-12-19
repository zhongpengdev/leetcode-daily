#include<iostream>
#include<algorithm>
#include<queue>
#include "TreeNode.h"

// 递归求解树的高度
int getTreeHeight(TreeNode *root) {
    if (root == nullptr) return 0;

    int leftHeight = getTreeHeight(root->left);
    int rightHeight = getTreeHeight(root->right);

    return std::max(leftHeight, rightHeight) + 1; //当前节点的值需要加1
}

// 非递归队列求解
int getTreeHeight1(TreeNode *root){
    if (root == nullptr) return 0;

    std::queue<TreeNode*> nodeQueue;
    nodeQueue.push(root);
    int height = 0;

    while(!nodeQueue.empty()) {
        int levelSize = nodeQueue.size();

        for(int i = 0; i < levelSize; i++) {
            TreeNode *currentNode = nodeQueue.front();
            nodeQueue.pop(); // 出队列

            if(currentNode -> left != nullptr) nodeQueue.push(currentNode -> left);
            if(currentNode -> right != nullptr) nodeQueue.push(currentNode -> right);
        }

        height ++; // 遍历完树高度加1
    }

    return height;
}

// 释放内存
void destroyTree(TreeNode *root) {
    if(root == nullptr) return;
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
}

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);

    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    
    int height = getTreeHeight(root);
    int height1 = getTreeHeight1(root);

    std::cout << "height of this tree is " << height << " (using recursion)" << std::endl;
    std::cout << "height of this tree is " << height1 << std::endl; 

    // 释放内存
    destroyTree(root);
    
    return 0;
}