/*
求先序序列中第k个元素的值
*/
#include<iostream>
#include <stdexcept>
#include "TreeNode.h"

bool findKNumberInPresort(TreeNode *root, int &count, int k, int &result){  
    if(!root) return false;

    //先让count++，落后1个
    count++;
    if(count == k) {
        result = root->value;
        return true;
    }

    //在左子树中找到
    if(findKNumberInPresort(root->left, count, k, result)) {
        return true;
    }

    //在右子树中找到
    if(findKNumberInPresort(root->right, count, k, result)) {
        return true;
    }

    return false;
}

int findKthInterface(TreeNode *root, int k) {
    int count = 0;
    int result = -1;

    //关键是这个引用count 和 引用result
    bool found = findKNumberInPresort(root, count, k, result);

    if(!found) {
        throw std::out_of_range("out of range");
    }

    return result;
}


void destroyTree(TreeNode* root) {
    if (root == nullptr) return;
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
}

// 测试示例
int main() {
    // 构建测试二叉树：
    //        1
    //       / \
    //      2   3
    //     / \
    //    4   5
    // 先序序列：1 -> 2 -> 4 -> 5 -> 3
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    // 测试不同k值
    try {
        std::cout << "first one:" << findKthInterface(root, 1) << std::endl; // 1
        std::cout << "third one:" << findKthInterface(root, 3) << std::endl; // 4
        std::cout << "forth one:" << findKthInterface(root, 5) << std::endl; // 3
        std::cout << "sixth one:" << findKthInterface(root, 6) << std::endl; // 抛出异常
    } catch (const std::exception& e) {
        std::cout << "exception:" << e.what() << std::endl;
    }

    destroyTree(root);
    return 0;
}
