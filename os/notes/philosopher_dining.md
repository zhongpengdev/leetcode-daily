# 哲学家就餐问题 - 状态机方案

## 问题描述

5 个哲学家围坐在圆桌旁，每人之间放一根筷子，共 5 根。哲学家的行为是交替的：思考 和 吃饭。吃饭时需要同时拿到左右两根筷子。

核心矛盾：如果每个哲学家都先拿左边筷子再拿右边筷子，5 个人同时行动时会每人手里拿着一根筷子，全部等待对方释放，形成**死锁**。

## 方案三：状态机 + 私有信号量

将"拿两根筷子"这个可能造成死锁的操作，拆解为**状态管理** + **条件检查**，用信号量控制同步。

### 核心数据结构

```c
PhilState state[N];    // 每个哲学家的状态：THINKING / HUNGRY / EATING
sem_t mutex;           // 保护 state 数组的互斥锁
sem_t ph_enum[N];      // 每个哲学家的私有信号量，用于"能不能吃"的同步
```

### 关键理解：ph_enum 的含义

`ph_enum[i]` **不是筷子**，而是一个**通知机制**：

- 初始值为 **0**（不可用）
- 哲学家 `i` 想吃饭时，先把自己标记为 HUNGRY，然后 `sem_wait(&ph_enum[i])` **阻塞等待**
- 当某个检查函数判定 `i` 可以吃时，调用 `sem_post(&ph_enum[i])` **唤醒他**
- `sem_wait` 和 `sem_post` 之间是**解耦**的：谁检查、谁通知，哲学家只需要等通知就行

### take_fork(拿筷子)

```c
void take_fork(int i) {
    sem_wait(&mutex);          // ① 加锁，进入临界区

    state[i] = HUNGRY;         // ② 标记自己饿了
    test(i);                   // ③ 检查自己能不能吃

    sem_post(&mutex);          // ④ 解锁
    sem_wait(&ph_enum[i]);     // ⑤ 阻塞，等通知
}
```

逐步分析：

1. **`sem_wait(&mutex)`** — 锁住 state 数组，防止其他哲学家同时修改状态
2. **`state[i] = HUNGRY`** — 告诉大家"我饿了"，不是直接抢筷子，而是发出信号
3. **`test(i)`** — 检查条件（见下文），如果左右邻居都没在吃，直接让自己进入 EATING 并 `sem_post(&ph_enum[i])`（此时 ph_enum 从 0 变 1，第 ⑤ 步不会阻塞）
4. **`sem_post(&mutex)`** — 释放互斥锁，让其他哲学家也能检查
5. **`sem_wait(&ph_enum[i])`** — 关键一步：
   - 如果第 ③ 步 `test` 通过了，ph_enum 已经是 1，这里**直接通过**，哲学家开始吃饭
   - 如果 `test` 没通过（邻居在吃），ph_enum 还是 0，这里**阻塞**，哲学家等待被唤醒

**为什么 `sem_wait` 放在 `sem_post(&mutex)` 之后？**

如果先 `sem_wait` 再 `sem_post(&mutex)`，哲学家会持锁阻塞，其他哲学家无法释放筷子或调用 `test`，造成死锁。**先放锁再等通知**是关键。

### put_fork(放筷子)

```c
void put_fork(int i) {
    sem_wait(&mutex);          // ① 加锁

    state[i] = THINKING;       // ② 标记自己吃完了
    test(LEFT);                // ③ 检查左邻居能不能吃
    test(RIGHT);               // ④ 检查右邻居能不能吃

    sem_post(&mutex);          // ⑤ 解锁
}
```

逐步分析：

1. **`state[i] = THINKING`** — 自己不再占着筷子
2. **`test(LEFT)` 和 `test(RIGHT)`** — 告诉左右邻居：我释放了筷子，你们看看现在能不能吃

这才是整个方案最精妙的地方：**哲学家不是自己拿筷子，而是由放筷子的人帮他检查**。

- 哲学家 `A` 在 `take_fork` 里 `sem_wait(&ph_enum[A])` 阻塞
- 哲学家 `B`（A 的邻居）吃完后 `put_fork`，调用 `test(A)`
- `test(A)` 发现 A 饿了且左右没人吃，`sem_post(&ph_enum[A])` 唤醒 A
- A 的 `sem_wait` 通过，开始吃饭

### test(检查函数)

```c
void test(int i) {
    if (state[i] == HUNGRY &&
        state[LEFT] != EATING &&
        state[RIGHT] != EATING) {
        state[i] = EATING;
        sem_post(&ph_enum[i]);  // 通知哲学家 i：你可以吃了
    }
}
```

三个条件缺一不可：
- `state[i] == HUNGRY` — 他在等筷子
- `state[LEFT] != EATING` — 左邻居没在吃（左筷子可用）
- `state[RIGHT] != EATING` — 右邻居没在吃（右筷子可用）

## 完整时序示例

假设哲学家 0 想吃饭，邻居 1 和 4 都在吃：

```
哲学家 0: take_fork(0)
  → state[0] = HUNGRY
  → test(0): state[1]==EATING → 不满足 → 不 post
  → sem_wait(&ph_enum[0]): 阻塞（ph_enum[0]==0）

... 等待 ...

哲学家 1: put_fork(1)
  → state[1] = THINKING
  → test(0): state[0]==HUNGRY ✓, state[4]!=EATING?, state[1]!=EATING ✓
     如果 state[4] 也不在吃 → 满足 → state[0]=EATING → sem_post(&ph_enum[0])

哲学家 0: sem_wait 通过 → 开始吃饭
```

## 为什么不会死锁

经典死锁场景：每人拿左边筷子，等右边筷子。

本方案的差异：
- **哲学家不直接拿筷子**，而是标记 HUNGRY 后等待通知
- **由邻居释放筷子时触发检查**，邻居释放时已经有至少一根筷子空闲
- 不存在"每个人都拿着一根等另一根"的状态，因为根本不存在"拿着一根筷子"这个中间状态
- 哲学家要么**同时拿到两根**（test 通过），要么**一根都不拿**（阻塞等待）

## 总结

| 信号量 | 初始值 | 作用 |
|--------|--------|------|
| mutex  | 1      | 保护 state 数组的互斥锁 |
| ph_enum[i] | 0  | 哲学家 i 的"能不能吃"通知信号 |

| 函数 | 行为 |
|------|------|
| take_fork | 标记饥饿 → 检查条件 → 放锁 → 等通知 |
| put_fork  | 标记思考 → 通知邻居检查 → 放锁 |
| test      | 条件满足则 post 对应哲学家的私有信号量 |
