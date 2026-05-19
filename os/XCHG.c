/**
 * XCHG指令实现原理
 * 
 * XCHG 同样通过锁总线（Assert LOCK# signal）或缓存锁定来实现绝对的原子性
 * 
 * 忙等（Busy Waiting），违反“让权等待”
 */

# include<stdbool.h>

// 硬件内部原子操作 （交换寄存器r和内存地址x的值）
// XCHG 主要做的就是值对调 
void XCHG(bool *r, bool *x) { 
    bool temp = *r;
    *r = *x;
    *x = temp;
}

// 全局共享状态锁：0空闲 1有人
int lock = 0;

void enter_region() {
    int old_value = 1; // 强行上锁
    
    do
    {
        // 原子交换
        XCHG(&old_value, &lock);

        /**
         * 1. 如果old_value 是0 那么刚才的锁就是空的，直接上新锁，占用
         * 2. 如果old_value 是1 那么继续while()
         */
    } while (old_value == 1);   
}

void leave_region() {
    lock = 0; // 释放锁
}
