/**
 *
 * %7
 *
 * 离散数学中的鸽巢原理（抽屉原理）
 *
 * O(N) -> O(1)
 *
 * 结果只可能是0-7
 *
 * 状态对(f_{n-1}, f_{n})最终只有49种结果 -> 周期循环
 *
 * 动态规划 visited[] 有限状态 + 确定性转移
 */
#include <iostream>
#include <vector>

class NumberSequenceSolver
{
public:
    static int solver(int A, int B, int n)
    {
        // (1 <= A, B <= 1000, 1 <= n <= 100,000,000).
        if (n == 1 || n == 2)
            return 1;

        // 存储当前计算结果
        std::vector<int> seq;
        seq.reserve(60);  // 结果最大大小为49，预分配内存减少动态扩容
        seq.push_back(0); // 边界和实际对齐
        seq.push_back(1);
        seq.push_back(1);

        // visited[前一个值][当前值] = 当前索引
        std::vector<std::vector<int>> visited(7, std::vector<int>(7, 0));
        visited[1][1] = 2; // (1, 1) = 2 当前index是2

        for (int i = 3; i <= n; i++)
        {
            int next_val = (A * seq[i - 1] + B * seq[i - 2]) % 7;
            seq.push_back(next_val);

            int prev_val = seq[i - 1];

            // 如果没有进入过状态表
            if (visited[prev_val][next_val] != 0)
            {
                int cycle_start = visited[prev_val][next_val];
                int cycle_length = i - cycle_start;

                //
                int remaining_steps = (n - i) % cycle_length;

                return seq[cycle_start + remaining_steps];
            }

            visited[prev_val][next_val] = i;
        }
        return seq[n];
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int A, B, n;
    while (std::cin >> A >> B >> n && (A != 0 || B != 0 || n != 0))
    {
        std::cout << NumberSequenceSolver::solver(A, B, n) << "\n";
    }

    return 0;
}