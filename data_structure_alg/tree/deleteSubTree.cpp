/*
删除全部以某个值为根的子树
*/
#include<iostream>
#include "TreeNode.h"

//传递引用
void deleteSubTree(TreeNode *&root) {
    if(!root) return;

    deleteSubTree(root->left);
    deleteSubTree(root->right);
    
    delete root; //只删除了root所指向的内容
    root = nullptr; //删除root本身
}

//传递引用
void findAllNode(TreeNode *&root, int value){
    if(!root) return;

    findAllNode(root->left, value);
    findAllNode(root->right, value);
    
    if(root->value == value) {
        deleteSubTree(root);
    }
}
