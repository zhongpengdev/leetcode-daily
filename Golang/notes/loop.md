# 循环

Go 只有一个循环关键字：**for**。没有 while、do-while。

## for 基本形式

```go
// 经典三段式（初始化; 条件; 增量）
for i := 0; i < 10; i++ {
    fmt.Println(i)
}
```

## for 当 while 用

省略初始化和增量，只留条件：

```go
x := 1
for x < 100 {
    x *= 2
}
```

## 死循环

三个部分全省略：

```go
for {
    // 无限循环，用 break 退出
}
```

常见用法：

```go
for {
    msg := <-ch
    if msg == "quit" {
        break
    }
    fmt.Println(msg)
}
```

## for range（遍历）

### 遍历 slice / 数组

```go
nums := []int{10, 20, 30}

// 只要值
for _, v := range nums {
    fmt.Println(v)
}

// 只要下标
for i := range nums {
    fmt.Println(i)
}

// 下标 + 值
for i, v := range nums {
    fmt.Println(i, v)
}
```

### 遍历 map

```go
m := map[string]int{"a": 1, "b": 2}

for k, v := range m {
    fmt.Println(k, v)
}
```

**注意：map 遍历顺序不固定**，每次运行可能不同。

### 遍历 string

按 rune（Unicode 码点）遍历，不是按 byte：

```go
s := "你好world"

for i, ch := range s {
    fmt.Printf("下标%d: %c\n", i, ch)
}
// 下标0: 你
// 下标3: 好
// 下标6: w
// ...
```

### 遍历 channel

```go
ch := make(chan int, 3)
ch <- 1
ch <- 2
ch <- 3
close(ch)

for v := range ch {
    fmt.Println(v)  // 1, 2, 3，channel 关闭后自动退出
}
```

## break 和 continue

```go
for i := 0; i < 10; i++ {
    if i == 3 {
        continue  // 跳过本次
    }
    if i == 7 {
        break     // 退出循环
    }
    fmt.Println(i)
}
// 输出: 0 1 2 4 5 6
```

## 带标签的 break / continue（跳出多层循环）

```go
outer:
for i := 0; i < 5; i++ {
    for j := 0; j < 5; j++ {
        if j == 3 {
            break outer  // 直接跳出最外层循环
        }
        fmt.Println(i, j)
    }
}
```

## 一行写法

Go 允许循环体写在一行（花括号不换行）：

```go
for i := 0; i < 10; i++ { fmt.Println(i) }
```

## 对比总结

| 形式 | 等价于其他语言 |
|------|--------------|
| `for i:=0; i<n; i++ {}` | for 循环 |
| `for x < 100 {}` | while 循环 |
| `for {}` | while(true) 死循环 |
| `for _, v := range s {}` | for-each |
| `for v := range ch {}` | channel 消费循环 |
