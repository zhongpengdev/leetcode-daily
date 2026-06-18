#include<iostream>
#include<queue>
#include "TreeNode.h"

int DoubleBranchNodeCount(TreeNode *root){
    int count = 0;
    
    //初始化队列
    std::queue<TreeNode*> q;
    q.push(root);

    while(!q.empty()) {
        TreeNode *currentNode = q.front();
        q.pop();

        //左右节点均不空
        if (currentNode->left && currentNode->right) {
            count++;
        }

        //入队前先判断nullptr
        if(currentNode->left) q.push(currentNode->left);
        if(currentNode->right) q.push(currentNode->right);
    }

    return count;
}

void destroyNodeTree(TreeNode *root) {
    if(root == nullptr) return;
    destroyNodeTree(root->left);
    destroyNodeTree(root->right);
    delete root;
}

int main() {
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);

    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    root->right->left = new TreeNode(6);

    int count = DoubleBranchNodeCount(root);

    std::cout << "count of double node is " << count << std::endl;

    destroyNodeTree(root);
    return 0;
}