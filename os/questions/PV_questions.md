# PV Questions

## Overall Notes

`while()` 循环代表持续服务，即使这个进程被挂起，也会一直运行，否则进程直接退出了，进程的动态动态互斥问题就退化成了单次顺序执行。

网络处理请求，或者监听键盘输入请求都是靠`while`顶着。


## Q1：数量差值限制

### 常见题型

1. 仓库容量从无限变成有限
2. 配对出库进程（H2O经典模型）
3. 数学边界突变

## 题目描述

在一个仓库中可以存放 A 和 B 两种产品，要求：
① 每次只能存入一种产品。
② A 产品数量 - B 产品数量 < M，其中 M 是正整数。
③ B 产品数量 - A 产品数量 < N，其中 N 是正整数。

假设仓库的容量是无限的，试用 P, V 操作描述产品 A 和 B 的入库过程。

## 解答

1. 看互斥条件：每次只允许一个进程访问仓库 ——> mutex = 1
2. 寻找A 和 B的初始最大条件：
   + A - B最大差M，也就是说最大差值为M-1（整数嘛）所以最初二者仓库数量是(0, 0)那么A最多入库数量是M-1 -> Sa = M - 1
   + B - A的最大差值同样是N - 1， 所以Sb的最初的初始数量是 Sb = N - 1
3. 当A入库一个时，P(Sa) **那么B的配额也是会跟着变的**：Sb的配额会增加一个，同理Sb变化Sa也会变。
   
```c
semaphore mutex = 1;
semaphore Sa = M - 1;
semaphore Sb = N - 1;

void Process_Put_A() {
    while(1) {
        // 生产产品A

        P(Sa);
        P(mutex);

        // 将A放入仓库

        V(mutex);
        V(Sb); // Sb 的信号量也要增加1
    }
}

void Process_Put_B() {
    while(1) {
        // 生产产品B

        P(Sb);
        P(mutex);

        // 将产品B放入仓库

        V(mutex);
        V(Sa);
    }
}
```

## Q2

面包店有很多面包，由 n 名销售人员推销。每名顾客进店后按顺序取一个号，并且等待叫号，当一名销售人员空闲时，就按顺序叫下一个号。可以用两个整型变量来记录当前的取号值和叫号值，试设计一个使销售人员和顾客同步的算法。

### 变体

1. 增加限制：店内只能坐X人，其余顾客到店外等候。

### 分析

1. 条件变量：calling_number taking_number
2. 互斥条件：多个customers 在改calling_number需要互斥，多个employees在改taking_number也需要互斥
3. 同步条件：只有顾客进来了employees进程才开始工作：资源信号量`customers`
4. 唤醒顾客：顾客取完号之后等叫号，`P(serve)`等 店员`V(serve)`叫醒队列中的front
   
```c
int taking_number = 0; // 待取号人数
int calling_number = 0; // 待叫号人数

semaphore mutex_take = 1; // 多个店员之间的互斥
semaphore mutex_call = 1;
semaphore serve = 0;

void Customer() {
    int my_number;

    P(mutex_take);
    my_number = taking_number;
    taking_number++;
    V(mutex_take);

    V(customers);
    P(serve); // 唤醒待叫号顾客

    // 买面包，离开
}

void Employees() {
    int call_number;
    while(1) {
        P(custmoers);

        P(mutex_call);
        call_number = calling_number;
        calling_number++;
        V(mutex_call);

        V(serve);

        // 推销面包
    }
}
```

### 简单版本

```c
int i, j; // 取号，叫号的人数
semaphore mutex_i = 1, mutex_j = 1;

Consumer() {
    // 进入面包店

    P(mutex_i);
    // 取号
    i++;   
    V(mutex_i);

    // 等待被叫号并买面包
}

Seller() {
    while(1) { // n个服务员进程
        P(mutex_j);
        if(j < i) {
            // 叫号j
            j++;
            V(mutex_j);
            
            //卖面包
        }else {
            V(mutex_j);
            // 退出
        }
    }
}

```