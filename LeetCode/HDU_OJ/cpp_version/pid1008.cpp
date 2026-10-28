/**
 * elevator liner list
 *
 * 1. 当前楼层数：current_floor
 *
 * 2. 目标楼层数:target_floor
 *
 * 3. 计算这一趟需要的时间是多少 大于就上升，小于就下降
 */

#include <iostream>
#include <vector>

void calculate_this_time(int &current_floor, int target_floor, int &total_time)
{
    int i = target_floor - current_floor;

    // 判断是上升还是下降
    if (i > 0)
    {
        // 需要上升
        total_time += 6 * i + 5;
    }
    else if (i < 0)
    {
        total_time += 4 * std::abs(i) + 5;
    }
    else
    {
        total_time += 5;
    }
    current_floor = target_floor;
}

int main()
{
    int N;

    while (std::cin >> N && N != 0)
    {
        int total_time = 0;
        int current_floor = 0;
        for (int i = 0; i < N; i++)
        {
            int target_floor;
            std::cin >> target_floor;

            calculate_this_time(current_floor, target_floor, total_time);
        }

        std::cout << total_time << "\n";
    }
}
