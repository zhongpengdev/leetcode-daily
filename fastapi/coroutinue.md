# Python 协程（Coroutine）详解

## 目录

1. [什么是协程](#1-什么是协程)
2. [同步 vs 异步：直观对比](#2-同步-vs-异步直观对比)
3. [核心语法：async/await](#3-核心语法asyncawait)
4. [可等待对象（Awaitable）](#4-可等待对象awaitable)
5. [事件循环（Event Loop）](#5-事件循环event-loop)
6. [Task：并发执行的单元](#6-task并发执行的单元)
7. [异步上下文管理器与迭代器](#7-异步上下文管理器与迭代器)
8. [同步原语与并发控制](#8-同步原语与并发控制)
9. [底层原理：生成器到协程的演进](#9-底层原理生成器到协程的演进)
10. [FastAPI 中的协程实战](#10-fastapi-中的协程实战)
11. [常见陷阱与最佳实践](#11-常见陷阱与最佳实践)
12. [性能调优与调试](#12-性能调优与调试)

---

## 1. 什么是协程

### 1.1 定义

协程（Coroutine）是一种**用户态的、协作式的、可暂停和恢复的**执行单元。它不是操作系统线程，不需要内核参与上下文切换，由程序自己控制切换时机。

1. 协程函数：`async def` 调用协程函数并不会立即执行，而是返回一个协程函数
2. 协程对象：`coro = my_coro()` 创建一个协程对象
3. 事件循环
4. await 交出控制权，执行其他的协程
5. Future `from asyncio import Future`
   
```py
"""
Future生命周期：创建 Future → 状态 pending（未完成）→ set_result() 被调用 → 状态 done（已完成）
"""
f = Future()

print(f.done())
# print(f.result()) # 报错，Result is not set

f.set_result(42)

print(f.done())
print(f.result())
```

### 1.2 协程的核心特征

| 特征 | 说明 |
|------|------|
| **协作式调度** | 自己决定让出 CPU（通过 `await`），不由操作系统抢占 |
| **用户态切换** | 切换开销极小（纳秒级 vs 线程切换的微秒级） |
| **共享内存空间** | 所有协程在同一个线程中运行，共享同一份内存 |
| **非阻塞 I/O** | 等待 I/O 时让出控制权，让其他协程运行 |

### 1.3 为什么需要协程

线程模型的问题

1. 每个线程大约需要 8MB的栈空间
2. 上下文切换开销大

协程解决的问题是高 **I/O** 并发：当程序大量时间花在等待网络、数据库、文件系统时，协程让单个线程在等待期间去做其他事。

理论上可以使用多线程实现同样的协程效果，但维护成本高。
---

## 2. 同步 vs 异步：直观对比

> **核心问题：为什么同步 3 秒，异步 1 秒？"挂起"到底是什么意思？**

### 2.1 同步版本（阻塞）— 为什么花 3 秒

只有一个线程，当执行到 `time.sleep(1)` 时，整个线程都会被挂起，导致全部被阻塞。

```python
import time

def fetch_data():
    time.sleep(1)          # 阻塞线程 1 秒
    return "data"

def main():
    start = time.time()
    for _ in range(3):
        result = fetch_data()   # 每次调用阻塞 1 秒
        print(result)
    print(f"耗时: {time.time() - start:.2f}s")   # ≈ 3 秒

main()
```

```
Python: time.sleep(1)
  → C 库: sleep(1)
    → 系统调用: nanosleep()
      → 内核: 把当前线程从 CPU 上摘下来
      → 内核: 将线程状态从 TASK_RUNNING 改为 TASK_INTERRUPTIBLE
      → 内核: 把线程放入"睡眠等待队列"
      → 内核: 调用 schedule() 切换到其他进程/线程
      → ... 1 秒后硬件定时器中断 ...
      → 内核: 定时器中断处理 → 把线程从睡眠队列移到就绪队列
      → 内核: 线程状态恢复为 TASK_RUNNING
      → 内核: 调度器选中它 → 重新放上 CPU
      → C 库: sleep(1) 返回
    → Python: time.sleep(1) 返回
```

从 `nanosleep()` 调用到返回的这 1 秒内，这个线程不在 CPU 上，不在就绪队列里，它在"睡眠等待队列"里。这就是"挂起"。


### 2.2 异步版本（非阻塞）— 为什么只需要 1 秒

```python
import asyncio
import time

async def fetch_data():
    """异步 I/O """
    await asyncio.sleep(1)   # 让出控制权，不阻塞线程
    return "data"

async def main():
    start = time.time()

    # 并发执行 3 个协程
    tasks = [fetch_data() for _ in range(3)]
    results = await asyncio.gather(*tasks)

    for r in results:
        print(r)
    print(f"耗时: {time.time() - start:.2f}s")   # ≈ 1 秒！

asyncio.run(main())
```

#### `await asyncio.sleep(1)` 做了什么

```
Python: await asyncio.sleep(1)

① asyncio.sleep(1) 创建一个 Future 对象（一个"承诺盒子"）
② 向事件循环注册一个定时器："1 秒后把这个 Future 标记为完成"
③ await 暂停当前协程，把控制权交还给事件循环
   └── 注意：这一步没有任何系统调用！线程没有睡觉！
       只是 Python 层面的"我不干了，你去干别人吧"
④ 事件循环立即去执行下一个已就绪的协程
```

**和 `time.sleep()` 的本质区别**：

| | `time.sleep(1)` | `await asyncio.sleep(1)` |
|---|---|---|
| 是否调用系统调用 | ✅ `nanosleep()` | ✅ `epoll_wait()` / `select()` |
| 线程被挂起了吗？ | ✅ 线程从 CPU 移除 | ❌ 线程仍在运行事件循环 |
| 谁在"等待"？ | **操作系统内核**在等 | **事件循环**在等（线程照常运转） |
| 等待期间能干嘛？ | 什么都不能干 | 执行其他协程 |

#### 异步版本的精确时间线

```
时间轴 →   (所有代码在同一个线程中执行)

t=0.0:  asyncio.run(main())
          事件循环启动
          创建 3 个 Task (协程 fetch_data 被包装成 Task)

t=0.0:  事件循环执行 Task-1:
          → 遇到 await asyncio.sleep(1)
          → 注册定时器: "Task-1 要在 1.0s 后唤醒"
          → 暂停 Task-1，交还控制权

t=0.0:  事件循环执行 Task-2:
          → 遇到 await asyncio.sleep(1)
          → 注册定时器: "Task-2 要在 1.0s 后唤醒"
          → 暂停 Task-2，交还控制权

t=0.0:  事件循环执行 Task-3:
          → 遇到 await asyncio.sleep(1)
          → 注册定时器: "Task-3 要在 1.0s 后唤醒"
          → 暂停 Task-3，交还控制权

t=0.0:  没有就绪的协程了，事件循环执行 await asyncio.gather()
          实际上 gather 内部在等待所有 Task 完成
          事件循环检查: "有 3 个定时器在 1.0s 后触发"
          事件循环等在此处...
          ┌─────────────────────────────────────────────┐
          │  事件循环等待就绪事件                            │
          │  (epoll_wait / select 等 I/O 多路复用)         │
          │  线程没有被挂起！事件循环仍在运行，只是没有可做    │
          └─────────────────────────────────────────────┘

t=1.0:  内核定时器触发 → 事件循环被唤醒
          → 检查定时器队列: Task-1 到期
          → 执行 Task-1.__step(): 从 await 处恢复
          → return "data" → Task-1 完成

t=1.0:  检查定时器队列: Task-2 到期
          → 执行 Task-2.__step(): 从 await 处恢复
          → return "data" → Task-2 完成

t=1.0:  检查定时器队列: Task-3 到期
          → 执行 Task-3.__step(): 从 await 处恢复
          → return "data" → Task-3 完成

t=1.0:  asyncio.gather() 返回 ["data","data","data"]
          → print → 结束

总耗时: ≈ 1.0 秒
```

---

## 3. 核心语法：async/await

### 3.1 `async def` — 定义协程函数

```python
import asyncio

def normal():
    print("hello")
    
nore1 = normal
print(type(nore1)) # <class 'function'>

nore2 = normal() # 执行

async def asy_func():
    return 42
    
asyfunc1 = asy_func
print(type(asyfunc1)) # <class 'function'>

asyfunc2 = asy_func()
print(type(asyfunc2)) # <class 'coroutine'>

result = asyncio.run(asyfunc2)
print(result)
```

### 3.2 `await`

await就是交出控制权的时刻

控制权：CPU执行哪段代码

交给了事件循环

去控制下一个协程函数

当被 await 的对象完成后，事件循环恢复当前协程
 
`await` 只能用在 `async def` 中

---

## 4. 可等待对象（Awaitable）

`await` 后面只能跟**可等待对象**。Python 中有三种可等待对象：

### 4.1 协程对象（Coroutine）

```python
async def nested():
    return 42

async def main():
    # 直接 await 协程对象
    result = await nested()     # 等价于嵌套调用
    print(result)

    # 或者先创建协程对象，再 await
    coro = nested()
    result = await coro
```

### 4.2 Task（任务）

Task 用来**并发调度**协程：多个 Task 可以同时在事件循环中运行。

```python
async def say_after(delay, what):
    await asyncio.sleep(delay)
    print(what)

async def main():
    # 创建 Task 的那一刻，协程就被提交到事件循环中排队了
    task1 = asyncio.create_task(say_after(1, "hello"))
    task2 = asyncio.create_task(say_after(2, "world"))

    print(f"任务启动时间: {time.time()}")

    # 现在 task1 和 task2 已经在并发运行了
    # await 只是等待它们完成
    await task1
    await task2

    # 总耗时 2 秒（取最大延迟），而不是 3 秒
```

**Task 的生命周期**：

```
创建 → 调度 → 运行 →（暂停 / 恢复）→ 完成
                  ↓
                报错 → 完成（含异常）
```

### 4.3 Future（底层原语）

`Future` 是一个低级的可等待对象，表示一个异步操作的**最终结果**。Task 是 Future 的子类。

```python
async def main():
    loop = asyncio.get_running_loop()
    future = loop.create_future()

    # 安排 1 秒后设置结果
    loop.call_later(1, future.set_result, "done!")

    result = await future   # 等待 future 被 set_result
    print(result)           # "done!"
```

**三者关系**：

```
Future (底层容器，存结果/异常)
  ↑ 继承
Task  (Future + 驱动协程执行)
  ↑ 包装
Coroutine (用户写的 async def 函数)
```

---

## 5. 事件循环（Event Loop）

### 5.1 事件循环是什么

它是一个无限循环，不断检查：

1. 有哪些 I/O 已经就绪？
2. 有哪些定时任务到了执行时间？
3. 有哪些协程可以被唤醒了？

然后选择其中一个去执行。

### 5.2 极简事件循环实现

```python
import selectors
import socket
import time
from collections import deque

class SimpleEventLoop:
    """一个最小的事件循环实现，帮助你理解原理"""

    def __init__(self):
        self.ready = deque()         # 就绪的回调队列
        self.selector = selectors.DefaultSelector()  # I/O 多路复用
        self.current_time = time.monotonic

    def call_soon(self, callback, *args):
        """将一个回调放入就绪队列"""
        self.ready.append((callback, args))

    def call_later(self, delay, callback, *args):
        """定时回调：delay 秒后执行"""
        deadline = self.current_time() + delay
        self.selector.register(
            _TimerFd(deadline),
            selectors.EVENT_READ,
            (callback, args)
        )

    def add_reader(self, fd, callback, *args):
        """注册 I/O 可读事件"""
        self.selector.register(fd, selectors.EVENT_READ, (callback, args))

    def run_forever(self):
        """事件循环主循环"""
        while True:
            # 步骤 1: 执行所有就绪的回调
            while self.ready:
                callback, args = self.ready.popleft()
                callback(*args)

            # 步骤 2: 等待 I/O 事件（有就绪回调则立即返回）
            timeout = 0 if self.ready else None
            events = self.selector.select(timeout=timeout)

            # 步骤 3: 将触发的 I/O 回调放入就绪队列
            for key, mask in events:
                callback, args = key.data
                self.ready.append((callback, args))
```

### 5.3 事件循环的执行模型

```
                     ┌─────────────────┐
                     │   Event Loop    │
                     └────────┬────────┘
                              │
                ┌─────────────┼─────────────┐
                ▼             ▼             ▼
         ┌──────────┐ ┌──────────┐ ┌──────────┐
         │ 检查就绪  │ │ 执行回调  │ │ I/O 等待 │
         │   任务    │ │           │ │ (epoll)  │
         └──────────┘ └──────────┘ └──────────┘
                │             │             │
                └─────────────┴─────────────┘
                              │
                              ▼
                       回到循环开头
```

### 5.4 `asyncio.run()` 做了什么

```python
# asyncio.run(coro) 等价于：
def asyncio_run(coro):
    loop = asyncio.new_event_loop()      # 1. 创建新的事件循环
    try:
        asyncio.set_event_loop(loop)     # 2. 设为当前线程的默认循环
        return loop.run_until_complete(coro)  # 3. 运行协程直到完成
    finally:
        loop.close()                     # 4. 清理资源
```

### 5.5 获取当前的事件循环

```python
async def main():
    # 方式 1：推荐
    loop = asyncio.get_running_loop()

    # 方式 2：不推荐（如果在没有循环的线程中调用会创建新的）
    loop = asyncio.get_event_loop()

    # 在同步代码中安全获取或创建
    try:
        loop = asyncio.get_running_loop()
    except RuntimeError:
        loop = asyncio.new_event_loop()
        asyncio.set_event_loop(loop)
```

---

## 6. Task：并发执行的单元

### 6.1 `asyncio.create_task()`

```python
async def fetch(url):
    print(f"开始抓取: {url}")
    await asyncio.sleep(1)  # 模拟网络请求
    print(f"完成: {url}")
    return f"result-{url}"

async def main():
    # 创建 Task：立即调度到事件循环
    t1 = asyncio.create_task(fetch("a"))
    t2 = asyncio.create_task(fetch("b"))
    t3 = asyncio.create_task(fetch("c"))

    # 此时三个 Task 已经在并发运行了！

    # 等待所有完成
    r1 = await t1
    r2 = await t2
    r3 = await t3
```

**关键点**：`create_task()` 调用后，协程就被提交到事件循环中排队了，不需要 `await` 它就会开始运行。`await task` 只是等待结果。

### 6.2 `asyncio.gather()` — 批量并发

```python
async def main():
    urls = ["a", "b", "c"]

    # gather 帮你创建任务并等待所有完成
    results = await asyncio.gather(
        fetch("a"),
        fetch("b"),
        fetch("c"),
    )
    print(results)  # ["result-a", "result-b", "result-c"]

    # 也可以传列表
    tasks = [fetch(url) for url in urls]
    results = await asyncio.gather(*tasks)

    # return_exceptions=True：单个任务报错不会影响其他
    results = await asyncio.gather(*tasks, return_exceptions=True)
```

### 6.3 `asyncio.wait()` — 更灵活的控制

```python
async def main():
    tasks = {asyncio.create_task(fetch(url)) for url in ["a", "b", "c"]}

    # 等待所有完成
    done, pending = await asyncio.wait(tasks)

    # 等待第一个完成
    done, pending = await asyncio.wait(
        tasks, return_when=asyncio.FIRST_COMPLETED
    )

    # 等待第一个异常
    done, pending = await asyncio.wait(
        tasks, return_when=asyncio.FIRST_EXCEPTION
    )

    # 设置超时
    try:
        done, pending = await asyncio.wait(tasks, timeout=5.0)
    except asyncio.TimeoutError:
        # pending 里是还没完成的任务
        pass
```

### 6.4 `asyncio.as_completed()` — 逐个处理结果

```python
async def main():
    tasks = [fetch(url) for url in ["a", "b", "c"]]

    # 哪个先完成就先处理哪个
    for coro in asyncio.as_completed(tasks):
        result = await coro   # 等待最早完成的那个
        print(f"最早完成: {result}")
```

### 6.5 Task 的类型关系

```python
@asyncio.coroutine          # 旧式写法（不推荐）
def old_style():
    yield from asyncio.sleep(1)

async def new_style():      # 新式写法
    await asyncio.sleep(1)

# 两者都返回协程对象，行为一致
```

---

## 7. 异步上下文管理器与迭代器

### 7.1 异步上下文管理器（`async with`）

```python
import asyncio

class AsyncConnection:
    """异步数据库连接（模拟）"""

    async def __aenter__(self):
        print("异步打开连接...")
        await asyncio.sleep(0.1)  # 模拟连接建立
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        print("异步关闭连接...")
        await asyncio.sleep(0.1)  # 模拟关闭清理
        return False  # 不抑制异常

    async def query(self, sql):
        await asyncio.sleep(0.5)
        return f"result of '{sql}'"

async def main():
    async with AsyncConnection() as conn:
        result = await conn.query("SELECT 1")
        print(result)
    # 退出 async with 时自动调用 __aexit__ 清理

# 也可以用 asynccontextmanager 装饰器
from contextlib import asynccontextmanager

@asynccontextmanager
async def managed_connection():
    conn = AsyncConnection()
    try:
        print("打开...")
        await asyncio.sleep(0.1)
        yield conn
    finally:
        print("关闭...")
        await asyncio.sleep(0.1)

async def main2():
    async with managed_connection() as conn:
        print("使用中...")
```

### 7.2 异步迭代器（`async for`）

```python
class AsyncRange:
    """异步迭代器"""

    def __init__(self, start, end):
        self.start = start
        self.end = end
        self.current = start

    def __aiter__(self):
        """返回异步迭代器自身"""
        return self

    async def __anext__(self):
        """异步获取下一个元素"""
        if self.current >= self.end:
            raise StopAsyncIteration
        await asyncio.sleep(0.1)  # 模拟异步操作
        val = self.current
        self.current += 1
        return val

async def main():
    results = []
    async for num in AsyncRange(0, 5):
        results.append(num)
    print(results)  # [0, 1, 2, 3, 4]

# 异步生成器（更简洁）
async def async_range(start, end):
    for i in range(start, end):
        await asyncio.sleep(0.1)
        yield i

async def main2():
    async for num in async_range(0, 5):
        print(num)
```

### 7.3 列表推导中的 `async for`

```python
async def main():
    # 异步列表推导（Python 3.6+）
    results = [num async for num in async_range(0, 5)]

    # 带条件的异步列表推导
    results = [num async for num in async_range(0, 10) if num % 2 == 0]

    # await 也可以出现在列表推导中
    results = [await fetch(url) for url in ["a", "b", "c"]]
    # ⚠️ 注意：这是在列表推导中 await，仍然是顺序执行的！
```

---

## 8. 同步原语与并发控制

虽然协程在单线程中运行，但仍需要同步控制来保护共享状态。asyncio 提供了全套同步原语：

### 8.1 Lock（互斥锁）

```python
import asyncio

shared_counter = 0
lock = asyncio.Lock()

async def increment():
    global shared_counter
    async with lock:              # 隐式 await lock.acquire() / lock.release()
        current = shared_counter
        await asyncio.sleep(0.001)  # 模拟计算延迟
        shared_counter = current + 1

async def main():
    tasks = [increment() for _ in range(100)]
    await asyncio.gather(*tasks)
    print(shared_counter)  # 100（而不是 < 100 的竞争结果）
```

**为什么单线程也需要锁？**

因为 `await` 点会交出控制权：
```
协程 A: read counter (0) → await → 恢复 → write counter (1)
协程 B:                → read counter (0) → write counter (1)  ← 丢失了 A 的更新！
```

### 8.2 Semaphore（信号量）— 限制并发数

```python
# 限制同时最多 3 个并发请求
semaphore = asyncio.Semaphore(3)

async def fetch_with_limit(url):
    async with semaphore:
        return await fetch(url)  # 最多 3 个同时 fetch

async def main():
    urls = [f"url_{i}" for i in range(100)]
    tasks = [fetch_with_limit(url) for url in urls]
    results = await asyncio.gather(*tasks)
```

这是**限流**的最常用模式，防止对下游服务造成过大压力。

### 8.3 Event（事件）

```python
event = asyncio.Event()

async def waiter():
    print("等待事件...")
    await event.wait()    # 阻塞直到 event.set()
    print("事件触发！")

async def setter():
    await asyncio.sleep(1)
    print("触发事件")
    event.set()           # 唤醒所有 waiters

async def main():
    await asyncio.gather(waiter(), setter())
```

### 8.4 Condition（条件变量）

```python
condition = asyncio.Condition()
queue = []

async def producer():
    for i in range(5):
        await asyncio.sleep(0.5)
        async with condition:
            queue.append(i)
            print(f"生产: {i}")
            condition.notify(1)  # 唤醒 1 个等待的消费者

async def consumer(name):
    while True:
        async with condition:
            await condition.wait_for(lambda: len(queue) > 0)
            item = queue.pop(0)
            print(f"{name} 消费: {item}")

async def main():
    producers = [producer()]
    consumers = [consumer("A"), consumer("B")]
    await asyncio.gather(*producers, *consumers)
```

### 8.5 Queue（队列）— 生产者消费者模式

```python
async def producer(queue):
    for i in range(10):
        await asyncio.sleep(0.1)
        await queue.put(i)
        print(f"生产: {i}")
    await queue.put(None)  # 毒丸，通知消费者退出

async def consumer(queue, name):
    while True:
        item = await queue.get()
        if item is None:   # 收到毒丸，退出
            queue.task_done()
            break
        print(f"{name} 消费: {item}")
        await asyncio.sleep(0.2)  # 模拟处理
        queue.task_done()

async def main():
    queue = asyncio.Queue(maxsize=5)  # 容量 5，满了 producer 会阻塞
    await asyncio.gather(
        producer(queue),
        consumer(queue, "A"),
        consumer(queue, "B"),
    )
    await queue.join()  # 等待队列中所有项处理完毕
```

### 8.6 所有同步原语对比

| 原语 | 用途 | 关键方法 |
|------|------|---------|
| `Lock` | 互斥，保护临界区 | `acquire()`, `release()` |
| `Semaphore` | 限制并发数 | `acquire()`, `release()` |
| `Event` | 一次性通知 | `set()`, `wait()`, `clear()` |
| `Condition` | 条件等待/通知 | `wait()`, `notify()`, `wait_for()` |
| `Queue` | 生产者-消费者 | `put()`, `get()`, `join()` |
| `BoundedSemaphore` | 同上，检测 release 次数 | 同 Semaphore |

---

## 9. 底层原理：生成器到协程的演进

### 9.1 Python 生成器回顾

```python
def simple_generator():
    print("step 1")
    yield 1                 # 暂停，返回值
    print("step 2")
    yield 2                 # 再次暂停
    print("step 3")

gen = simple_generator()
print(next(gen))  # step 1 → 1
print(next(gen))  # step 2 → 2
print(next(gen))  # step 3 → StopIteration
```

生成器的关键能力：**可以暂停、可以恢复、可以在暂停时保持局部状态**。这正是协程需要的。

### 9.2 `yield from` — 委托生成器

```python
def sub_gen():
    yield 1
    yield 2

def main_gen():
    yield from sub_gen()   # 委托给子生成器
    yield 3

for val in main_gen():
    print(val)  # 1, 2, 3
```

`yield from` 建立了**双向通道**：调用者 ↔ 主生成器 ↔ 子生成器，可以双向传递值和异常。

### 9.3 `send()` 和协程的雏形

```python
def coroutine_prototype():
    """协程的原型：用 send() 向生成器注入值"""
    print("启动协程")
    while True:
        received = yield         # yield 在等号右边！接收来自外部的值
        print(f"收到: {received}")

co = coroutine_prototype()
next(co)             # 预激：推进到第一个 yield
co.send("hello")     # 发送 "hello" 进去
co.send("world")     # 发送 "world" 进去
co.close()           # 关闭协程
```

### 9.4 `@asyncio.coroutine` — 基于生成器的协程（Python 3.4）

```python
@asyncio.coroutine
def old_coroutine():
    print("old style")
    yield from asyncio.sleep(1)   # yield from + Future
    return "done"
```

### 9.5 `async def` / `await` — 原生协程（Python 3.5+）

```python
# Python 3.5+ 将协程提升为语言级特性
async def modern_coroutine():
    print("modern style")
    await asyncio.sleep(1)    # await 替代了 yield from
    return "done"
```

### 9.6 协程对象的底层结构

```python
async def demo():
    await asyncio.sleep(1)

coro = demo()
print(type(coro))                   # <class 'coroutine'>
print(coro.__await__)               # <method-wrapper '__await__' ...>
print(coro.cr_running)              # False — 是否在运行
print(coro.cr_frame)                # None — 当前栈帧（运行时有值）
print(coro.cr_code.co_name)         # 'demo' — 编译后的代码对象名
print(coro.cr_await)                # None — 正在 await 的对象
print(coro.__class__.__mro__)       # 继承链
```

**关键属性**：

| 属性 | 含义 |
|------|------|
| `cr_code` | 编译后的代码对象 |
| `cr_frame` | 当前执行栈帧（暂停时为最后一个 `await` 的位置） |
| `cr_running` | 是否正在运行 |
| `cr_await` | 当前正在 `await` 的对象（如果有） |
| `cr_origin` | 创建该协程的调用栈追踪 |
| `__await__()` | 返回一个迭代器，这是事件循环驱动协程的方式 |

### 9.7 事件循环如何驱动协程

```python
async def task(name):
    print(f"{name}: 开始")
    await asyncio.sleep(0.1)
    print(f"{name}: 继续")
    await asyncio.sleep(0.1)
    print(f"{name}: 结束")

# 手动驱动协程（揭示 asyncio 内部做了什么）
coro = task("test")

# 步骤 1: 获取协程的 __await__ 迭代器
iterator = coro.__await__()

# 步骤 2: 第一次 send(None) 推进到第一个 await
try:
    result = iterator.send(None)   # 打印 "test: 开始"
    print(f"协程暂停，await 的是: {result}")  # <Future pending>
except StopIteration as e:
    pass

# 步骤 3: 等待 Future 完成后，再次 send() 恢复
# 这通常由事件循环的 _step() 方法自动完成
```

### 9.8 Task 内部如何工作（`__step`）

```python
class SimplifiedTask:
    """简化版 Task，展示核心机制"""

    def __init__(self, coro, loop):
        self.coro = coro
        self.loop = loop
        self._done = False
        self._result = None
        self._exception = None
        # 立即将 _step 放入事件循环执行
        loop.call_soon(self._step)   # 关键：第一次 step() 在这里触发

    def _step(self, exc=None):
        """每一步推进协程执行"""
        try:
            if exc is None:
                # 正常推进
                result = self.coro.send(None)
            else:
                # 把异常抛入协程
                result = self.coro.throw(exc)
        except StopIteration as e:
            # 协程执行完毕
            self._done = True
            self._result = e.value
            return
        except Exception as e:
            self._done = True
            self._exception = e
            return

        # 协程还没结束，result 是它 await 的对象（通常是 Future）
        # 给这个 Future 添加回调：完成后再次调用 _step
        if hasattr(result, 'add_done_callback'):
            result.add_done_callback(
                lambda fut: self.loop.call_soon(self._step)
            )
        else:
            # 不应该发生
            self.loop.call_soon(self._step)

    def __await__(self):
        """让 Task 本身也是可等待的"""
        yield self  # 等待自己被标记为 done
        return self._result
```

**整个流程可视化**：

```
asyncio.create_task(coro)
    │
    ▼
Task.__init__()
    │
    ▼
loop.call_soon(self._step)     ← 排队执行
    │
    ▼
_step(): coro.send(None)       ← 协程开始运行
    │
    │ 直到遇到 await future
    ▼
future.add_done_callback(      ← 注册回调
    lambda f: loop.call_soon(   ← future 完成后，将 _step 重新排入事件循环
        self._step
    )
)
    │
    │ ... I/O 完成后 ...
    ▼
loop.call_soon(self._step)     ← 恢复执行
    │
    ▼
_step(): coro.send(None)       ← 从上次暂停处继续
    │
    │ 重复直到协程返回
    ▼
StopIteration                  ← 协程结束
```

### 9.9 底层 I/O 多路复用

Python asyncio 底层使用操作系统的 I/O 多路复用机制：

| 操作系统 | 机制 | 说明 |
|----------|------|------|
| Linux | `epoll` | 事件驱动，O(1) 复杂度 |
| macOS/BSD | `kqueue` | FreeBSD 内核的事件通知 |
| Windows | `IOCP` | I/O 完成端口（Proactor 模式） |
| 通用回退 | `select` | POSIX 标准，O(n) 复杂度 |

```python
# 验证当前使用的 selector
import asyncio
import selectors

# 查看 asyncio 使用的 selector
selector = selectors.DefaultSelector()
print(type(selector).__name__)   # Linux: EpollSelector
```

**epoll 工作流程**：

```
① 将 socket fd 注册给 epoll
② epoll_wait() 阻塞，直到有 fd 就绪
③ 内核只返回就绪的 fd（不是全部遍历）
④ 处理就绪 fd → 触发对应的 Future → 恢复对应的协程
```

---

## 10. FastAPI 中的协程实战

### 10.1 FastAPI 的协程支持

FastAPI 的每个路由处理函数**默认运行在线程池中**，但定义为 `async def` 时就在事件循环中执行：

```python
from fastapi import FastAPI
import asyncio
import time

app = FastAPI()

# 同步路由 — 在线程池中执行（不会阻塞事件循环）
@app.get("/sync")
def sync_endpoint():
    time.sleep(5)  # 只阻塞这个线程池线程
    return {"message": "sync"}

# 异步路由 — 在事件循环中执行
@app.get("/async")
async def async_endpoint():
    await asyncio.sleep(5)  # 不阻塞，让出控制权
    return {"message": "async"}

# ⚠️ 危险的混用！async def 中调用同步阻塞函数会卡死整个事件循环
@app.get("/dangerous")
async def dangerous_endpoint():
    time.sleep(5)  # ❌ 阻塞了整个事件循环！
    return {"message": "blocked"}
```

**FastAPI 的执行模型**：

```
请求到达
    │
    ├── async def 路由 → 在事件循环中执行
    │       └── 所有异步操作保持不阻塞
    │
    └── def 路由 → submit 到线程池 ThreadPoolExecutor
            └── 不会阻塞事件循环（但受线程池大小限制）
```

### 10.2 异步数据库操作

```python
# 使用 asyncpg（异步 PostgreSQL 驱动）
import asyncpg
from fastapi import FastAPI

app = FastAPI()

# 应用启动时创建连接池
@app.on_event("startup")
async def startup():
    app.state.pool = await asyncpg.create_pool(
        dsn="postgresql://user:pass@localhost/db",
        min_size=5,
        max_size=20,
    )

@app.on_event("shutdown")
async def shutdown():
    await app.state.pool.close()

@app.get("/users")
async def get_users():
    async with app.state.pool.acquire() as conn:
        rows = await conn.fetch("SELECT id, name FROM users LIMIT 10")
    return [dict(row) for row in rows]

# 使用 SQLAlchemy 异步模式
from sqlalchemy.ext.asyncio import create_async_engine, AsyncSession
from sqlalchemy.orm import sessionmaker

engine = create_async_engine("postgresql+asyncpg://user:pass@localhost/db")
AsyncSessionLocal = sessionmaker(engine, class_=AsyncSession, expire_on_commit=False)

@app.get("/users-sa")
async def get_users_sa():
    async with AsyncSessionLocal() as session:
        result = await session.execute(
            "SELECT id, name FROM users LIMIT 10"
        )
        rows = result.fetchall()
    return [{"id": r.id, "name": r.name} for r in rows]
```

### 10.3 并发外部 API 调用

```python
import httpx
from fastapi import FastAPI

app = FastAPI()

@app.get("/aggregate")
async def aggregate_data():
    """并发调用多个外部 API，聚合结果"""
    async with httpx.AsyncClient() as client:
        # 三个请求并发出去，总时间 ≈ 最慢的那个
        responses = await asyncio.gather(
            client.get("https://api1.example.com/data"),
            client.get("https://api2.example.com/data"),
            client.get("https://api3.example.com/data"),
        )
    return {
        "api1": responses[0].json(),
        "api2": responses[1].json(),
        "api3": responses[2].json(),
    }

@app.get("/search-all")
async def search_all(query: str):
    """并发搜索，只要有一个返回就行（竞速模式）"""
    async with httpx.AsyncClient() as client:
        tasks = [
            client.get(f"https://service1.com/search?q={query}"),
            client.get(f"https://service2.com/search?q={query}"),
            client.get(f"https://service3.com/search?q={query}"),
        ]
        # 哪个先返回用哪个
        done, pending = await asyncio.wait(
            tasks, return_when=asyncio.FIRST_COMPLETED
        )
        # 取消还没完成的
        for task in pending:
            task.cancel()
        result = done.pop().result()
        return result.json()
```

### 10.4 Background Tasks（后台任务）

```python
from fastapi import BackgroundTasks

async def send_email(email: str, message: str):
    await asyncio.sleep(2)  # 模拟发送
    print(f"邮件发送完成: {email}")

async def log_analytics(user_id: int):
    await asyncio.sleep(1)
    print(f"分析日志已记录: {user_id}")

@app.post("/register")
async def register(
    email: str,
    background_tasks: BackgroundTasks,
):
    # 注册用户...
    user_id = 42

    # 添加后台任务：不阻塞响应，请求返回后后台执行
    background_tasks.add_task(send_email, email, "欢迎注册")
    background_tasks.add_task(log_analytics, user_id)

    return {"message": "注册成功", "user_id": user_id}
    # 响应先返回，后台任务继续执行
```

### 10.5 WebSocket 中的协程

```python
from fastapi import WebSocket, WebSocketDisconnect

class ConnectionManager:
    def __init__(self):
        self.active_connections: list[WebSocket] = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def broadcast(self, message: str):
        """广播消息给所有连接"""
        tasks = [
            connection.send_text(message)
            for connection in self.active_connections
        ]
        await asyncio.gather(*tasks)

manager = ConnectionManager()

@app.websocket("/ws/{client_id}")
async def websocket_endpoint(websocket: WebSocket, client_id: int):
    await manager.connect(websocket)
    try:
        while True:
            data = await websocket.receive_text()
            await manager.broadcast(f"Client #{client_id}: {data}")
    except WebSocketDisconnect:
        manager.disconnect(websocket)
        await manager.broadcast(f"Client #{client_id} left")
```

### 10.6 生命周期管理（Lifespan）

```python
from contextlib import asynccontextmanager
from fastapi import FastAPI

@asynccontextmanager
async def lifespan(app: FastAPI):
    # 启动时：初始化资源
    print("正在启动...")
    app.state.client = httpx.AsyncClient()
    app.state.db_pool = await asyncpg.create_pool(...)
    print("启动完成")

    yield  # ← 应用运行期间

    # 关闭时：清理资源
    print("正在关闭...")
    await app.state.client.aclose()
    await app.state.db_pool.close()
    print("关闭完成")

app = FastAPI(lifespan=lifespan)
```

---

## 11. 常见陷阱与最佳实践

### 11.1 ❌ 陷阱 1：在 async 函数中调用同步阻塞代码

```python
# ❌ 错误：time.sleep() 阻塞事件循环
async def bad():
    time.sleep(10)  # 整个事件循环被卡住 10 秒

# ✅ 方案 1：使用异步版本
async def good1():
    await asyncio.sleep(10)

# ✅ 方案 2：放到线程池执行
async def good2():
    loop = asyncio.get_running_loop()
    await loop.run_in_executor(None, time.sleep, 10)
    #                          ↑ 默认线程池

# ✅ 方案 3：如果有大量阻塞调用，创建专用线程池
from concurrent.futures import ThreadPoolExecutor
executor = ThreadPoolExecutor(max_workers=4)

async def good3():
    loop = asyncio.get_running_loop()
    await loop.run_in_executor(executor, some_blocking_function, arg1, arg2)
```

### 11.2 ❌ 陷阱 2：忘记 await

```python
async def fetch_data():
    await asyncio.sleep(1)
    return "data"

async def main():
    # ❌ 错误：没有 await，协程没有被执行
    # RuntimeWarning: coroutine 'fetch_data' was never awaited
    fetch_data()

    # ❌ 错误：创建了协程对象但没有执行
    coro = fetch_data()
    # 什么都没发生，coro 被垃圾回收

    # ✅ 正确
    result = await fetch_data()
```

### 11.3 ❌ 陷阱 3：在同步代码中调用异步函数

```python
# ❌ 错误：await 只能出现在 async def 中
def sync_function():
    result = await async_function()  # SyntaxError

# ✅ 方案 1：用 asyncio.run()
def sync_function():
    result = asyncio.run(async_function())

# ✅ 方案 2：如果已经在事件循环中，用 create_task 或直接 await
# （但 sync_function 做不到，因为事件循环是线程绑定的）
```

### 11.4 ❌ 陷阱 4：在循环中顺序 await

```python
# ❌ 慢：顺序执行
async def slow():
    results = []
    for url in urls:
        result = await fetch(url)   # 每次等一个完成再开始下一个
        results.append(result)

# ✅ 快：并发执行
async def fast1():
    tasks = [fetch(url) for url in urls]
    results = await asyncio.gather(*tasks)

# ✅ 也可以边走边收集（但仍是顺序）
async def fast2():
    tasks = [asyncio.create_task(fetch(url)) for url in urls]
    results = [await t for t in tasks]
```

### 11.5 ❌ 陷阱 5：Task 引用丢失导致被 GC 取消

```python
async def main():
    # ❌ 危险：task 没有保存引用，可能被垃圾回收
    asyncio.create_task(long_running_task())

    # 做一些其他事情...
    await asyncio.sleep(0.1)
    # long_running_task 可能已经被取消了！

    # ✅ 正确：保存引用
    task = asyncio.create_task(long_running_task())
    # 或者用 task_group（Python 3.11+）
    async with asyncio.TaskGroup() as tg:
        tg.create_task(long_running_task())
```

### 11.6 ❌ 陷阱 6：取消和清理不完整

```python
async def main():
    task = asyncio.create_task(do_work())
    try:
        result = await task
    except asyncio.CancelledError:
        # CancelledError 是 BaseException 的子类，不是 Exception
        # 如果不捕获，它会向上传播，正确取消
        pass
    finally:
        # 确保取消未完成的任务
        if not task.done():
            task.cancel()
            try:
                await task
            except asyncio.CancelledError:
                pass
```

### 11.7 ✅ 最佳实践总结

```python
# 1. 用 asyncio.timeout() 设置超时（Python 3.11+）
async def with_timeout():
    try:
        async with asyncio.timeout(5.0):
            result = await slow_operation()
    except asyncio.TimeoutError:
        result = "default"

# Python 3.10 及以下：
async def with_timeout_legacy():
    try:
        result = await asyncio.wait_for(slow_operation(), timeout=5.0)
    except asyncio.TimeoutError:
        result = "default"


# 2. 用 TaskGroup 管理一组任务（Python 3.11+）
async def with_task_group():
    async with asyncio.TaskGroup() as tg:
        task1 = tg.create_task(job1())
        task2 = tg.create_task(job2())
    # 退出时自动等待所有任务，任何一个报错会取消其他

# Python 3.10 及以下：
async def with_manual_tasks():
    tasks = [asyncio.create_task(job()) for job in jobs]
    await asyncio.gather(*tasks)


# 3. 为阻塞 I/O 创建专用线程池
executor = ThreadPoolExecutor(max_workers=10)

async def handle_blocking_work():
    loop = asyncio.get_running_loop()
    result = await loop.run_in_executor(
        executor, blocking_io_function
    )


# 4. 合理命名 Task，便于调试
task = asyncio.create_task(do_work(), name="my-task")


# 5. 使用 asyncio.shield() 保护关键操作不被取消
async def critical_section():
    try:
        await asyncio.shield(critical_operation())
    except asyncio.CancelledError:
        # critical_operation() 继续执行，只是这个 await 被取消
        pass
```

---

## 12. 性能调优与调试

### 12.1 启用调试模式

```python
import asyncio

# 方法 1：环境变量
# PYTHONASYNCIODEBUG=1 python app.py

# 方法 2：代码中设置
asyncio.run(main(), debug=True)

# 方法 3：直接设置事件循环
loop = asyncio.get_event_loop()
loop.set_debug(True)
```

**调试模式会检测**：
- 协程没有被 await（"coroutine was never awaited" 警告）
- 耗时过长的回调（默认 100ms，会打印警告）
- 事件循环在非主线程中被关闭
- `call_soon()` 和 `call_at()` 的参数错误

### 12.2 设置慢回调检测

```python
loop = asyncio.get_running_loop()
loop.slow_callback_duration = 0.5  # 超过 0.5 秒的回调会打印警告
```

### 12.3 性能分析工具

```python
# asyncio 内置的性能追踪
import asyncio

async def main():
    await asyncio.sleep(1)

loop = asyncio.new_event_loop()
loop.slow_callback_duration = 0.1

# 开启调试模式运行（会输出慢回调警告）
loop.run_until_complete(main())
```

```python
# 使用 pytest-asyncio 测试异步代码
# pip install pytest-asyncio

import pytest

@pytest.mark.asyncio
async def test_async_function():
    result = await my_async_function()
    assert result == expected

@pytest.mark.asyncio
async def test_with_timeout():
    async with asyncio.timeout(1.0):
        result = await my_async_function()
    assert result == expected
```

### 12.4 事件循环中的执行时视线

```python
async def main():
    print("1. 在事件循环中")
    print(f"   当前循环: {asyncio.get_running_loop()}")

    # 查看事件循环内部状态
    loop = asyncio.get_running_loop()
    print(f"2. 是否运行中: {loop.is_running()}")
    print(f"3. 是否已关闭: {loop.is_closed()}")

    # 获取当前 Task
    current_task = asyncio.current_task()
    print(f"4. 当前 Task: {current_task.get_name()}")
    print(f"5. 协程对象: {current_task.get_coro()}")

    # 所有 Task
    all_tasks = asyncio.all_tasks(loop)
    print(f"6. 所有 Task: {len(all_tasks)} 个")
    for t in all_tasks:
        print(f"   - {t.get_name()}: done={t.done()}, cancelled={t.cancelled()}")

asyncio.run(main())
```

### 12.5 理解 asyncio 的事件循环实现步骤

```
最终总结：当你写 await asyncio.sleep(1) 时，底层发生了什么？

1. asyncio.sleep(1) 创建一个 Future
2. await 将当前协程暂停，把 Future 返回给事件循环
3. 事件循环通过 __step 方法获取到这个 Future
4. 事件循环给 Future 注册回调：Future 完成 → 将 Task.__step 放回就绪队列
5. 事件循环用 epoll.select(timeout) 等待 I/O 或定时器
6. timeout 到期 → 定时器 Future 被设为完成 → 回调触发 → __step 被排队
7. 下一轮循环中，__step 被执行 → 协程从 await 处恢复继续

核心思想：
- 协程 = 可暂停可恢复的函数
- 事件循环 = 调度器
- Future/Task = 协程和事件循环之间的桥梁
- I/O 多路复用 = 高效的 I/O 等待
```

---

## 速查表

| 需求 | 方法 |
|------|------|
| 运行协程 | `asyncio.run(coro)` |
| 并发执行 | `asyncio.gather(*coros)` |
| 创建后台任务 | `asyncio.create_task(coro)` |
| 等待单个完成 | `asyncio.wait(tasks, return_when=FIRST_COMPLETED)` |
| 设置超时 | `async with asyncio.timeout(5): ...` (3.11+) / `asyncio.wait_for(coro, timeout=5)` |
| 阻塞代码放线程池 | `await loop.run_in_executor(None, func)` |
| 限流 | `asyncio.Semaphore(n)` + `async with sem:` |
| 互斥 | `asyncio.Lock()` + `async with lock:` |
| 生产者消费者 | `asyncio.Queue(maxsize=n)` |
| 后台任务 | `BackgroundTasks.add_task(func)` (FastAPI) |
| 异步 HTTP | `httpx.AsyncClient` |
| 异步数据库 | `asyncpg` / `aiomysql` / `motor` (MongoDB) |
| 异步 Redis | `redis.asyncio` / `aioredis` |
| 测试异步代码 | `pytest-asyncio` + `@pytest.mark.asyncio` |
| 调试 | `asyncio.run(main(), debug=True)` |
