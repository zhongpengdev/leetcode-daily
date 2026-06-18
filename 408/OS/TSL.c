/**
 * TSL 指令逻辑实现
 * 
 * 原子：读旧值 写新值
 * 
 * 忙等（Busy Waiting），违反“让权等待”
 */

 # include<stdbool.h>

bool TSL(bool *lock) {
    bool old = *lock; // 1. 读取当前锁的状态 false空闲 true忙
    *lock = true; // 2. 强行上锁
    return old; // 3. 返回旧的状态
} 

int lock = 0; // 0空闲 1上锁

void enter_region() {
    while (TSL(&lock) == 1); // 自旋
}

void leave_region() {
    lock = 0;
}