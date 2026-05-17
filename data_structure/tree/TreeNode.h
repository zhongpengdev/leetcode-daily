#ifndef TREENODE_H   // 头文件保护：防止重复包含
#define TREENODE_H

struct TreeNode {
    int value;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int value) : value(value), left(nullptr), right(nullptr) {}
};

/**
 * 2. destroy()声明
 * 需要具体在文件中实现
 */
void destroyTreeNode(TreeNode *root);

#endif