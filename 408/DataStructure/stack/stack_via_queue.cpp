/**
 * 使用队列来模拟栈
 *
 * 1. 让push费劲，pop轻松：入队一个新元素之后，将队列之前的所有元素都移动到队尾
 *
 * 2. pop费劲， push轻松：push正常入队，pop的时候将队列前n-1个元素移动到队尾，再弹出
 */

#include <queue>
#include <vector>
#include <iostream>
using namespace std;

/**
 * 使用方案一 push麻烦
 */

class myStack
{
public:
    std::queue<int> q;

    myStack() {}

    void push(int x)
    {
        q.push(x);

        // 自动实现当只有一个元素的时候跳过for循环。
        // q.size() is a long unsigned int
        for (size_t i = 0; i < q.size() - 1; i++)
        {
            // 将前n-1个元素全部都移动到末尾
            int tmp = q.front();
            q.pop();

            q.push(tmp);
        }
    }

    int top()
    {
        return q.front();
    }

    void pop()
    {
        q.pop();
    }

    bool empty()
    {
        return q.empty();
    }
};

int main()
{
    myStack st;

    st.push(1);
    st.push(2);
    st.push(3);

    cout << "top: " << st.top() << endl;    // 3

    st.pop();
    cout << "top: " << st.top() << endl;    // 2

    st.push(4);
    cout << "top: " << st.top() << endl;    // 4

    st.pop();
    st.pop();
    st.pop();
    cout << "empty: " << st.empty() << endl; // 1

    return 0;
}