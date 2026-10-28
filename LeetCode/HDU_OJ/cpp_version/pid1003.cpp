/**
 * Max Sum
 *
 *  calculate the max sum of a sub-sequence
 */

#include <iostream>
#include <vector>
using namespace std;

struct Result
{
    int max_sum;
    int start_index;
    int end_index;
};

// 又忘记传地址
Result calculate_max_sum(const vector<int> &array)
{
    /**
     * 1. 记录当前的最大值
     * 2. 记录全局最大值
     *
     * Rule1: 如果当前片段的和已经是负数了，那就扔掉它，从下一个数字重新开始。
     * Rule2: 如果当前片段的和是正数，那就继续往后加（因为正数只会让总和变大）。
     */

    int current = array[0];
    int best = array[0];

    int currentStart = 0;
    int bestStart = 0;
    int bestEnd = 0;

    // start with 1
    for (int i = 1; i < array.size(); i++)
    {
        if (current < 0)
        {
            current = array[i]; // 当前最大值小于0就丢掉
            currentStart = i;
        }
        else
        {
            current += array[i]; // 加上当前的值
        }

        if (current > best)
        {
            best = current;
            bestStart = currentStart;
            bestEnd = i; // current end;
        }
    }

    return {best, bestStart, bestEnd};
}

int main()
{
    int T;
    cin >> T;

    for (int i = 1; i <= T; i++)
    {
        int N;
        cin >> N;

        vector<int> array(N);

        for (int j = 0; j < N; j++)
        {
            cin >> array[j];
        }

        Result result = calculate_max_sum(array);

        // formatted output
        cout << "Case " << i << ":" << "\n";
        cout << result.max_sum << " " << result.start_index + 1 << " " << result.end_index + 1 << "\n";

        if (i < T)
        {
            cout << "\n";
        }
    }

    return 0;
}
