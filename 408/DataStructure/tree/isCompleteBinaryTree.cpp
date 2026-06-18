#include<iostream>
#include<queue>
#include "TreeNode.h"

bool isCompleteBinaryTree(TreeNode *root) {
    if(root == nullptr) {
        return true;
    }

    std::queue<TreeNode*> q;
    q.push(root);
    bool flag = false; 
    
    while(!q.empty()) {
        TreeNode* currentNode = q.front();
        q.pop();

        if(currentNode == nullptr) {
            flag = true;
            continue;
        }

        //前一个节点为空但后节点不空
        if(flag) {
            return false;
        }

        //当前节点的左节点为空，但右节点不为空
        if(currentNode->left == nullptr && currentNode->right != nullptr) {
            return false;
        }

        q.push(currentNode->left); //左右节点均入队，空节点也入队
        q.push(currentNode->right);
    }

    return true;
}

void destroyTreeNode(TreeNode *root){
    if(root == nullptr) return;

    destroyTreeNode(root->left);
    destroyTreeNode(root->right);
    delete root;
}

int main() {
    TreeNode* root1 = new TreeNode(1);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(3);
    root1->left->left = new TreeNode(4);
    root1->right->right = new TreeNode(5);
    root1->right->left = new TreeNode(6);

    std::cout << "Test 1 is completeBinaryTree? " << (isCompleteBinaryTree(root1) ? "Yes" : "No") << std::endl;
    return 0;
}