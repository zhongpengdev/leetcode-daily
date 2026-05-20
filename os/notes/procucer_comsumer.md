# 生产者-消费者问题

1. 同步：
   + 缓冲区没满 -> 生产者生产
   + 缓冲区不空 -> 消费者消费
2. 互斥：
   + 读写必须互斥访问

## 三信号量实现方案

+ mutex = 1（互斥信号量）
+ empty = n（空闲缓冲区数，同步信号量）
+ full = 0（已占用缓冲区数，同步信号量）

```c
/**
将缓冲区看成n个槽位：
互斥锁
空槽位
有东西的槽

那么最初就是n个空槽，0个有东西的槽
empty + full = n
*/
int mutex = 1;
int empty = n; // 空位置是n
int full = 0; // 有多少是有的

producer() {
    while(true) {
        P(empty); // 减少一个空位/等空位
        P(mutex); // 缓冲区上锁

        produce(); // 有空位，再生产

        put_item();

        V(mutex);
        V(full); // 唤醒消费者
    }
}

consumer() {
    while(true) {
        P(full); // 是否有产品可拿，无则自旋等待
        P(mutex); // 上锁

        get_item();

        V(mutex);
        V(empty);
        consume(); // 消费逻辑
    }
}
```

## 产生死锁的情况

死锁发生情况如下：

```c
producer() {
    while(true) {
        P(mutex); // 先上锁
        P(empty); // 再自旋等空位
        ...
    }
}

consumer() {
    while(true) {
        P(mutex); // 尝试获得锁
        P(full); // 获得锁后才能消费

        ...
    }
}
```

**P操作顺序写反（先加锁再等资源），当缓冲区满或空的时候都会产生死锁。本质是拿着锁等资源，但对方根本进不到缓冲区帮你释放资源：**

### 死锁形成过程（缓冲区满时）

```
初始：empty=0, full=3, mutex=1  （槽位满）

生产者再次进来：
P(mutex)  → mutex=0，拿到锁
P(empty)  → empty=0，没有空位，自旋，锁在自己手里

消费者来了：
P(mutex)  → mutex=0，拿不到锁，也被迫阻塞

死锁：
生产者等 empty（需要消费者腾出空位）
消费者等 mutex（需要生产者释放锁）
互相等待
```

**先申请资源，再申请锁。先释放锁，再释放资源。**

# 多生产者-多消费者问题

生产者多消费者讨论的是：
  + 多个生产者同时往缓冲区放东西  → 写指针冲突
  + 多个消费者同时从缓冲区拿东西  → 读指针冲突

## 经典变体：水果盘问题

一个盘子能放 N 个水果：
- 爸爸：往盘子里放苹果
- 妈妈：往盘子里放橘子
- 女儿：只吃苹果
- 儿子：只吃橘子

### 情况一：plate=1（盘子只能放1个）

`plate=1` 本身就起到了互斥作用。`P(plate)` 后 plate=0，其他进程直接阻塞，不需要额外的 mutex：

```c
爸爸() {
    P(plate);      // plate=0，其他进程进不来
    放苹果();
    V(apple);
}

妈妈() {
    P(plate);
    放橘子();
    V(orange);
}

女儿() {
    P(apple);
    拿苹果();
    V(plate);      // 腾出位置
}

儿子() {
    P(orange);
    拿橘子();
    V(plate);
}
```

`plate=1` 等价于一个互斥锁，谁拿了谁独占盘子。

### 情况二：plate>1（盘子能放多个）

`P(plate)` 放行后，plate 可能还 >0，其他进程也能同时进来操作盘子。**必须加 mutex 保护盘子的操作：**

```c
爸爸() {
    P(plate);      // 还有空位
    P(mutex);      // 上锁，防止多人同时放
    放苹果();
    V(mutex);
    V(apple);
}

妈妈() {
    P(plate);
    P(mutex);
    放橘子();
    V(mutex);
    V(orange);
}

女儿() {
    P(apple);
    P(mutex);      // 上锁，防止多人同时拿
    拿苹果();
    V(mutex);
    V(plate);
}

儿子() {
    P(orange);
    P(mutex);
    拿橘子();
    V(mutex);
    V(plate);
}
```

本质：把基本版的 `full` 拆成了 `apple` 和 `orange`，因为消费者不是谁都能拿，得对号入座。

# 读者-写者问题

+ 允许多个读者可以同时读
+ 只允许一个写者往里写
+ 任一写者在完成写操作之前不允许其他读者或写者工作
+ 写者执行写操作之前应该让所有读者或写者退出
---
1. 读者-读者：不互斥
2. 读者-写者、写者-写者：互斥

## 信号量方案

```c
rwmutex = 1    // 读写互斥
countmutex = 1 // 保证P(rwmutex)和count++/ count-- 的操作是原子的
int reader_count = 0;
```

## 读者进程

```c
reader() {
    P(countmutex);         // 将下面两个过程原子化
    reader_count++;
    if (reader_count == 1) // 第一个读者，锁写者
        P(rwmutex);
    V(countmutex);

    // 读数据...

    P(countmutex);
    reader_count--;
    if (reader_count == 0) // 最后一个读者，放行写者
        V(rwmutex);
    V(countmutex);
}
```

## 写者进程

```c
writer() {
    P(rwmutex); // 写者写时直接上锁

    // 写数据...

    V(rwmutex);
}
```

关键：第一个读者锁写者，最后一个读者解锁写者。中间读者直接读。

## 饥饿问题

上述代码为读者优先策略。写者饥饿的过程：

```
读者A：P(rwmutex) → 拿到锁，开始读
写者：  P(rwmutex) → 阻塞，排队等

读者A走：V(rwmutex)

读者B来了：P(rwmutex) → 拿到锁（抢在写者前面），读
写者继续等

读者B走：V(rwmutex)

读者C来了：P(rwmutex) → 拿到锁，读
写者继续等

→ 写者永远抢不到 rwmutex
```

每次写者刚等到锁，就被新来的读者抢走。读者和写者竞争同一个 `rwmutex`，读者读完立刻释放，写者要写完才释放，速度慢，永远抢不过。

### 解决方案：加门 w

```c
w = 1          // 门
rwmutex = 1
countmutex = 1
reader_count = 0
```

```c
reader() {
    P(w);                  // 过门（写者关着就等）
    P(countmutex);
    reader_count++;
    if (reader_count == 1)
        P(rwmutex);
    V(countmutex);
    V(w);                  // 过了门就放手

    // 读数据...

    P(countmutex);
    reader_count--;
    if (reader_count == 0)
        V(rwmutex);
    V(countmutex);
}

writer() {
    P(w);                  // 关门，后来的读者被挡
    P(rwmutex);            // 等当前读者走完

    // 写数据...

    V(rwmutex);
    V(w);                  // 开门
}
```

加门后的过程：

```
读者A在读，写者在等 rwmutex

写者：P(w) → w=0（关门）

读者B来了：P(w) → 阻塞（门关了，到不了 rwmutex）

读者A走：V(rwmutex) → 写者拿到，开始写

读者B还卡在 P(w)，根本没机会和写者抢 rwmutex
```

门的作用：不让新读者靠近 `rwmutex`，写者就没人和它抢了。
