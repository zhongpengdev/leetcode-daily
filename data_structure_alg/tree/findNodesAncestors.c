#include<stdio.h>
#include<stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;


//创建新节点
TreeNode* createNode(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = val;
    node->left = node->right = NULL;
    return node;
}

int findAndPrintAncestor(TreeNode* root, int x){
    if(root == NULL) return 0;

    //递归左子树
    if(findAndPrintAncestor(root->left, x)){
        printf("%d", root->value);
        return 1;
    }

    //递归右子树
    if(findAndPrintAncestor(root->right, x)) {
        printf("%d", root->value);
        return 1;
    }

    if(root->value == x) {
        return 1;
    }

    //没找到返回上层0
    return 0;
}

int printAncestors(TreeNode *root, int value) {
    //1.root == NULL
    if(root == NULL) {
        printf("BinaryTree is NULL");
    }

    int found = findAndPrintAncestor(root, value);

    if(!found) printf("not found");
    else printf("\n");
}

int main() {
    // 构建测试二叉树：
    //        1
    //       / \
    //      2   3
    //     / \   \
    //    4   5   6
    //       /
    //      7
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->right = createNode(6);
    root->left->right->left = createNode(7);

    printAncestors(root, 7);

    return 0;
}