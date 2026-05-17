#include <iostream>
#include "level-order-traversal.cpp"

// void destroy(TreeNode* root) {
//     if (!root) return;
//     destroy(root->left);
//     destroy(root->right);
//     delete root;
// }

void destroyTreeNode(TreeNode* root) {
    std::cout << "will destroy tree node" << std::endl; 
    if (!root) return;
    destroyTreeNode(root->left);
    destroyTreeNode(root->right);
    delete root;
}

int main() {
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    std::cout << "层序遍历:" << std::endl;
    level_order_traversal(root);

    // 测试空树
    std::cout << "空树测试:" << std::endl;
    level_order_traversal(nullptr);

    destroyTreeNode(root);
    return 0;
}
