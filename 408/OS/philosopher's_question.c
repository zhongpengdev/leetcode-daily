/**
 * 哲学家就餐问题
 * 
 * 实现方案三（无死锁）：将抢筷子的可能造成死锁的两个状态转换成"是否饥饿"的状态机问题
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define N 5
#define LEFT (i + N - 1) % N // 左邻居编号
#define RIGHT (i + 1) % N // 右邻居编号

// 哲学家状态枚举
typedef enum {
    THINKING = 0,
    HUNGRY = 1,
    EATING = 2
} PhilState;

PhilState state[N]; // 每个哲学家的状态
sem_t mutex; // 全局互斥锁，状态数组的读写互斥
sem_t ph_enum[N]; // 每个哲学家是否能进行吃饭的同步信号量

// 信号量初始化
void init() {
    sem_init(&mutex, 0, 1); // 全局状态数组互斥锁
    for(int i = 0; i < N; i++) sem_init(&ph_enum[i], 0, 0); // 私有信号量
}

// 校验模块
void test(int i) {
    if(i < 0 || i >= N) return;

    // 只有自己饥饿且左右邻居都没有在吃饭时自己才可以吃
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;

        sem_post(&ph_enum[i]); // 自己的私有信号量发信号
    }
}

// 拿起筷子业务
void take_fork(int i) {
    sem_wait(&mutex); // 进入临界区，锁定状态数组

    state[i] = HUNGRY;
    test(i);

    sem_post(&mutex);
    sem_wait(&ph_enum[i]);
}

// 放筷子
void put_fork(int i) {
    sem_wait(&mutex);

    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);

    sem_post(&mutex);
}

void* philosopher(void* arg) {
    int i = *(int*)arg;
    while (1)
    {
        // think_operation(i);

        take_fork(i);

        // eat_operation(i);

        put_fork(i);
    }
    return NULL;
}

int main() {
    init();

    pthread_t tid[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
