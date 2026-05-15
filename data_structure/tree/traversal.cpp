/**
 * 二叉树的遍历
 */
# include<iostream>
# include<stack>
# include "TreeNode.h"

using namespace std;

// 先序遍历

// 1. 传递的是TreeNode*
void pre_order_traversal(TreeNode *treeNode) {
    // 2. 是 nullptr 不是 NULL
    if(treeNode == nullptr) return;

    // 取值是 ->
    cout << treeNode->value << endl;

    pre_order_traversal(treeNode->left);

    pre_order_traversal(treeNode->right);
}

// 中序
void in_order_traversal(TreeNode *treeNode) {
    if(treeNode == nullptr) return;

    in_order_traversal(treeNode->left);

    cout << treeNode->value << endl;

    in_order_traversal(treeNode->right);
}

// 后续
void last_order_traversal(TreeNode *treeNode) {
    if(treeNode == nullptr) return;

    last_order_traversal(treeNode->left);

    last_order_traversal(treeNode->right);

    cout << treeNode->value << endl;
}


// ----------------------------------------

/**
 * 非递归版本遍历
 * 
 * 栈用来做保存
 * 
 * 先保存谁，谁就先入栈
 */

 /**
  * 1. 先序遍历
  * 根节点弹出
  * 2. 右节点入栈
  * 
  * 3. 左节点入栈
  */
void pre_order_iterative(TreeNode *root) {
    if(root == nullptr) return;

    stack<TreeNode*> s;
    s.push(root);

    while (!s.empty())
    {
        TreeNode *node = s.top(); // 弹出栈顶元素
        s.pop();

        // 访问
        cout << node->value << endl;

        // 右节点先入栈
        if(node->right) {
            s.push(node->right);
        }
        if(node->left) {
            s.push(node->left);
        }
    }
}

void in_order_iterative(TreeNode *root) {
    // 不需要判断
    // if(root == nullptr) return;

    stack<TreeNode*> s;
    TreeNode *cur = root;

    while (cur || !s.empty())
    {
        while (cur)
        {       
            s.push(cur); // 一直往左走
            cur = cur->left;
        }
        
        cur = s.top();
        s.pop();

        cout << cur->value << endl;

        cur = cur->right; // 往右走
    }
}

/**
 * 后序遍历非递归版本
 * 
 * 需要记住上一个节点的TreeNode指针
 */
void last_order_iterative(TreeNode *root) {

    stack<TreeNode*> s;
    TreeNode *cur = root;
    TreeNode *prev = nullptr; // 该节点的前一个节点。

    while (cur || !s.empty())
    {
        while (cur)
        {       
            s.push(cur); // 一直往左走
            cur = cur->left;
        }
        
        cur = s.top();
        
        if (cur->right && cur->right != prev) {
            cur = cur->right;
        }else {
            cout << cur->value << endl;

            prev = cur;
            s.pop();
            cur = nullptr;
        }
    }
}