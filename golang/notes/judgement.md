# 条件判断

## if

不需要括号，花括号不能换行。

```go
// 基本形式
if x > 0 {
    fmt.Println("正数")
}

// if-else
if x > 0 {
    fmt.Println("正数")
} else {
    fmt.Println("非正数")
}

// if-else if-else
if x > 0 {
    fmt.Println("正数")
} else if x == 0 {
    fmt.Println("零")
} else {
    fmt.Println("负数")
}
```

### if 带初始化语句

在 if 前可以写一个简短语句（变量只在 if/else 块内有效）。

```go
if err := doSomething(); err != nil {
    fmt.Println("出错了:", err)
}
// 这里访问不到 err
```

这是 Go 最常见的写法，用于处理错误。

```go
// 对比：不带初始化（err 外面还能用）
err := doSomething()
if err != nil {
    fmt.Println(err)
}
// 这里还能用 err
```

## switch

**自动 break**，不会穿透（和 C/Java 不同）。

```go
// 基本形式
switch x {
case 1:
    fmt.Println("一")
case 2:
    fmt.Println("二")
default:
    fmt.Println("其他")
}
```

### switch 带初始化语句

```go
switch os := runtime.GOOS; os {
case "linux":
    fmt.Println("Linux")
case "darwin":
    fmt.Println("Mac")
default:
    fmt.Println(os)
}
```

### 无条件 switch（替代 if-else 链）

```go
switch {
case x > 100:
    fmt.Println("大于100")
case x > 0:
    fmt.Println("正数")
default:
    fmt.Println("非正数")
}
```

### fallthrough（穿透）

显式写 `fallthrough` 才会穿透到下一个 case：

```go
switch x {
case 1:
    fmt.Println("一")
    fallthrough  // 继续执行下一个 case，不判断条件
case 2:
    fmt.Println("会到这里")  // 不管 x 是不是 2
}
```

## select（仅用于 channel）

`select` 是 switch 的 channel 版，用于多路复用。

```go
select {
case msg := <-ch1:
    fmt.Println("从ch1收到:", msg)
case ch2 <- data:
    fmt.Println("发送到ch2")
case <-time.After(time.Second):
    fmt.Println("超时")
}
```

多个 case 同时就绪时，随机选一个执行。

## 对比总结

| 形式 | 用途 |
|------|------|
| `if` | 一般条件判断 |
| `if init; cond` | 判断前需要初始化（最常用于错误处理） |
| `switch x` | 多分支精确匹配 |
| `switch`（无条件） | 替代 if-else 链 |
| `fallthrough` | 显式穿透（很少用） |
| `select` | channel 多路复用 |
