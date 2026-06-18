/**
 * 二叉排序树的创建，删除节点，添加元素
 */

#include <iostream>

/**
 * 创建，插入元素
 * 依次插入
 */

// struct
struct TreeNode
{
    TreeNode *left;
    TreeNode *right;
    int value;
    TreeNode(int value) : value(value), left(nullptr), right(nullptr) {}
    /* data */
};

// 创建新的Node节点
TreeNode createNewNode(int val)
{
    return TreeNode(val);
}

// 传参 使用引用指针
void createBST(int array[], int n, TreeNode *&root)
{
    // 在一棵树中插入当前节点 val
    /**
     * 1. 树是空的
     *
     * 2. val比当前节点大
     *
     * 3. val比当前节点小
     *
     * 4. val等于当前节点
     */

    // 外层for循环遍历val
    for (int i = 0; i < n; i++)
    {

        int val = array[i];

        TreeNode *temp = root;

        // 父指针指向前一个节点
        TreeNode *parent = root;

        // tag标记为表示左右走 左0 右1
        int tag = 0;

        if (root == nullptr)
        {
            // val为根节点

            // root->value = val;
            // root是空的，不能解引用
            root = new TreeNode(val);

            continue;
        }

        // while循环一直往下走，找到待插入位置
        // 手动break
        while (1)
        {
            // 从根节点遍历
            if (val > temp->value)
            {
                parent = temp;
                temp = temp->right;
                tag = 1;
            }
            else //这里不能使用两个if，第二次的是新的temp。直接使用else
            {
                parent = temp;
                temp = temp->left;
                tag = 0;
            }

            // 最后判断null并插入
            if (temp == nullptr)
            {
                // temp是空的需要new
                // temp = new TreeNode(val);  // 这里不对，temp修改的值无法挂到parent上面，需要有一个parent指针

                /**
                 * 这里不需要tag，直接使用val 和parent->value比较
                 */
                if (tag == 0)
                {
                    parent->left = new TreeNode(val);
                }
                else
                {
                    parent->right = new TreeNode(val);
                }

                // 直接打断 应该是break 跳出while
                break;
            }
        }
    }
}
