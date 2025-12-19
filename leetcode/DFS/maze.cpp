#include <iostream>
#include <vector>
#include <iomanip> // 用于输出格式化

using namespace std;

// 定义迷宫大小
const int ROWS = 4;
const int COLS = 4;

// 迷宫地图：0=通路，1=墙壁
// 路径从 (0, 0) 开始
int maze[ROWS][COLS] = {
    {0, 1, 0, 0},
    {0, 0, 0, 1},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
};

// 标记数组：0=未访问，1=已访问 (当前路径)
int visited[ROWS][COLS] = {0};

// 定义四个可能的移动方向 (下, 上, 右, 左)
// d_row[k] 和 d_col[k] 对应第 k 个方向的坐标变化
int d_row[] = {1, -1, 0, 0};
int d_col[] = {0, 0, 1, -1};

/**
 * @brief 使用回溯法（DFS）寻找从 (r, c) 到终点 (end_r, end_c) 的路径
 * @param r 当前行
 * @param c 当前列
 * @param end_r 终点行
 * @param end_c 终点列
 * @return bool 如果找到路径，返回 true
 */
bool findPath(int r, int c, int end_r, int end_c) {
    
    // 1. 终止条件 / 成功条件
    if (r == end_r && c == end_c) {
        // 到达终点，路径找到
        visited[r][c] = 1; // 标记终点
        return true;
    }

    // 2. 剪枝 / 失败条件
    // 检查边界 and 检查是否为墙 and 检查是否已在当前路径中访问过
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS || maze[r][c] == 1 || visited[r][c] == 1) {
        return false;
    }

    // --- 回溯核心：标记当前路径 ---
    visited[r][c] = 1; // 标记当前位置已在路径中，相当于“压入栈”

    // 3. 尝试所有可能的方向 (下, 上, 右, 左)
    for (int i = 0; i < 4; ++i) {
        int next_r = r + d_row[i];
        int next_c = c + d_col[i];

        // 递归深入：尝试从下一个位置寻找路径
        if (findPath(next_r, next_c, end_r, end_c)) {
            return true; // 只要一个方向成功，立即返回
        }
    }

    // --- 回溯核心：撤销标记 ---
    // 如果所有方向都失败了，说明当前位置 (r, c) 无法通向终点
    // 必须撤销标记，返回上一步，相当于“弹出栈”
    visited[r][c] = 0; 
    
    return false; // 当前路径失败
}

// 打印路径（只打印访问数组）
void printPath() {
    cout << "\n找到的路径 (1 表示路径):\n";
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cout << setw(3) << visited[i][j];
        }
        cout << endl;
    }
}

int main() {
    int start_r = 0, start_c = 0; // 起点 (0, 0)
    int end_r = 3, end_c = 3;     // 终点 (3, 3)

    if (findPath(start_r, start_c, end_r, end_c)) {
        cout << "成功找到从 (" << start_r << ", " << start_c << ") 到 (" << end_r << ", " << end_c << ") 的路径。\n";
        printPath();
    } else {
        cout << "无法找到路径。\n";
    }

    return 0;
}