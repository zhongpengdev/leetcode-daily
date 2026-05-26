/**
 *
 * simulation of stack and sequeue
 */

#include <iostream>
#include <stack>
#include <queue>
#include <string>

int main()
{
    int N;
    std::cin >> N;

    // 总次数
    for (int i = 0; i < N; i++)
    {
        int count;
        std::string structure_type;
        std::cin >> count >> structure_type;
        

        std::queue<int> que;
        std::stack<int> stk;

        for (int i = 0; i < count; i++)
        {
            std::string state;
            std::cin >> state;

            if (structure_type == "FIFO")
            {
                if (state == "IN")
                {
                    int num;
                    std::cin >> num;
                    que.push(num);
                }
                else
                {
                    if (!que.empty())
                    {
                        std::cout << que.front() << "\n";
                        que.pop();
                    }
                    else
                    {
                        std::cout << "None" << "\n";
                    }
                }
            }
            else  // FILO
            {
                if (state == "IN")
                {
                    int num;
                    std::cin >> num;
                    stk.push(num);
                }
                else
                {
                    if (!stk.empty())
                    {
                        std::cout << stk.top() << "\n";
                        stk.pop();
                    }
                    else
                    {
                        std::cout << "None" << "\n";
                    }
                }
            }
        }
    }
}