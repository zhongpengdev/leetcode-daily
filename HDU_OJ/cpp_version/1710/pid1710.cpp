/**
 *
 * preL
 * preR
 * inL
 * inR
 * k: root index in in-order sequence
 *
 * 1. 前序对首为根
 * 2. 在中序队列中找到根的位置
 * 3. 根据位置划分左右子树，继续递归
 */

#include <iostream>
#include <vector>

// global output formatter method
class PostOrderPrinter
{
private:
    bool is_first = true;

public:
    void print(int value)
    {
        if (!is_first)
        {
            std::cout << " ";
        }
        std::cout << value;
        is_first = false;
    }

    void reset()
    {
        is_first = true;
    }
};

PostOrderPrinter printer;

void build(std::vector<int> &pre, int preL, int preR,
           std::vector<int> &in, int inL, int inR)
{
    if (preL > preR || inL > inR)
        return;

    // root value
    int root_value = pre[preL];

    // get root index in in list
    int root_in_index = inL;
    while (root_in_index < inR && in[root_in_index] != root_value)
    {
        root_in_index++;
    }

    // 左子树的长度
    int left_tree_len = root_in_index - inL;

    // 递归左子树
    build(pre, preL + 1, preL + left_tree_len,
          in, inL, root_in_index - 1);

    // 递归右子树
    build(pre, preL + left_tree_len + 1, preR,
          in, root_in_index + 1, inR);

    printer.print(root_value);
}

int main() {
    // IO optimize
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int node_count;

    // multiple groups tree set
    while (std::cin >> node_count) 
    {
        std::vector<int> preorder(node_count);
        std::vector<int> inorder(node_count);

        for(int i = 0; i < node_count; i++) std::cin >> preorder[i];
        for(int j = 0; j < node_count; j++) std::cin >> inorder[j];

        printer.reset();

        build(preorder, 0, node_count - 1,
        inorder, 0, node_count - 1);

        std::cout << "\n";
    }

    return 0;
}
