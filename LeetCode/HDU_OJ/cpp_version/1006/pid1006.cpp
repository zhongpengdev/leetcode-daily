/**
 * 使用角速度计算一个极小的delta时间内那一刻的瞬时的夹角差值
 */

#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
    double D;

    const double delta = 0.001; // 秒
    const double cycle = 12 * 3600; 
    const double speed_h = 360.0 / cycle;     // 时针角速度 
    const double speed_m = 360.0 / 3600.0;    // 分针角速度 
    const double speed_s = 360.0 / 60.0;      // 秒针角速度 

    while (cin >> D && D >= 0) {
        double total_time = 0;
        // 遍历12小时内的所有离散时间点
        for (double t = 0; t < cycle; t += delta) {
            // 计算当前时刻三个指针的角度
            double h_angle = fmod(t * speed_h, 360.0);
            double m_angle = fmod(t * speed_m, 360.0);
            double s_angle = fmod(t * speed_s, 360.0);

            // 计算两两之间的最小夹角
            double diff_hm = fabs(h_angle - m_angle);
            diff_hm = min(diff_hm, 360 - diff_hm);
            double diff_hs = fabs(h_angle - s_angle);
            diff_hs = min(diff_hs, 360 - diff_hs);
            double diff_ms = fabs(m_angle - s_angle);
            diff_ms = min(diff_ms, 360 - diff_ms);

            if (diff_hm >= D && diff_hs >= D && diff_ms >= D) {
                total_time += delta;
            }
        }

        double percentage = total_time / cycle * 100.0;

        cout << fixed << setprecision(3) << percentage << endl;
    }
    return 0;
}