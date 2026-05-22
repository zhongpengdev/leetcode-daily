/**
 * 使用栈模拟队列
 *
 * 入栈之前先将内部的所有元素都出栈
 *
 * 放到一个临时栈倒腾一次
 *
 */

#include <stack>

class MyQueue
{
public:
    std::stack<int> stack;
    std::stack<int> transit_stack;

    MyQueue() {};

    void push(int number)
    {
        int n = stack.size(); // 单独定义n，pop()操作导致stack.size()一直在变
        for(int i = 0; i < n; i++) {
            transit_stack.push(stack.top());
            stack.pop();
        }
        
        // 新节点入栈低
        stack.push(number);

        // 倒腾回来
        n = transit_stack.size();
        for(int i = 0; i < n; i++) {
            stack.push(transit_stack.top());
            transit_stack.pop();
        }
    }

    int front()
    {
        return stack.top();
    }

    void pop()
    {
        stack.pop();
    }

    bool empty()
    {
        return stack.empty();
    }
};