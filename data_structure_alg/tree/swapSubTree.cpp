/*
递归实现交换所有节点左右子树

root == nullptr 或者 !root 是安全的，不访问内存。
root->left，如果root是空指针则崩溃
*/
#include<iostream>
#include<queue>
#include "TreeNode.h"

void swapSubTree(TreeNode *root) {
    if(!root) return;

    TreeNode *temp = root->left;
    root->left = root->right;
    root->right = root->left;

    //递归调用
    swapSubTree(root->left);
    swapSubTree(root->right);
}