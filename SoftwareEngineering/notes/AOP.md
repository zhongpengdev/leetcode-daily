# AOP - 面向切面编程 (Aspect-Oriented Programming)

## 什么是 AOP

有些逻辑**横切**（cross-cut）多个模块，比如日志、权限、事务、性能监控。这些逻辑如果到处写，就是重复代码（违反 DRY）。AOP 就是把这种"横切关注点"抽出来，统一处理。

核心概念：
- **切面（Aspect）**：要统一处理的逻辑（比如"记录每个接口的耗时"）
- **连接点（Join Point）**：程序执行的某个点（比如函数调用）
- **通知（Advice）**：切面在连接点执行的动作（函数执行前、后、异常时）
- **切点（Pointcut）**：匹配哪些连接点（比如"所有 Service 方法"）

## 举个例子

没有 AOP：
```go
func (s *UserService) Create(user User) error {
    start := time.Now()
    log.Println("开始 Create")

    // 权限检查（到处写）
    if !checkPermission() {
        return errors.New("no permission")
    }

    // 实际业务逻辑
    err := s.repo.Create(user)

    // 日志（到处写）
    log.Printf("Create 耗时: %v", time.Since(start))

    return err
}

func (s *UserService) Update(user User) error {
    start := time.Now()
    log.Println("开始 Update")

    if !checkPermission() {    // 又写一遍
        return errors.New("no permission")
    }

    err := s.repo.Update(user)

    log.Printf("Update 耗时: %v", time.Since(start))  // 又写一遍

    return err
}
```

有 AOP：
```go
// 切面：统一处理日志
func WithLogging(fn func() error) func() error {
    return func() error {
        start := time.Now()
        log.Println("开始执行")
        err := fn()
        log.Printf("耗时: %v", time.Since(start))
        return err
    }
}

// 切面：统一处理权限
func WithAuth(fn func() error) func() error {
    return func() error {
        if !checkPermission() {
            return errors.New("no permission")
        }
        return fn()
    }
}

// 业务代码只关心核心逻辑
func (s *UserService) Create(user User) error {
    return s.repo.Create(user)
}

// 使用时组合切面
do := WithAuth(WithLogging(func() error {
    return userService.Create(user)
}))
do()
```

## 属于哪个层面

**横切关注点（Cross-Cutting Concern）**，不属于三层架构中的任何一层，而是贯穿所有层：

```
          ┌─────────────────────────────┐
          │    日志 / 权限 / 事务 / 缓存    │  ← AOP 横切
          ├──────────┬──────────┬───────┤
          │ Handler  │ Service  │ Repo  │
          └──────────┴──────────┴───────┘
```

## Go 中实现 AOP

Go 没有原生 AOP（不像 Java 有 Spring AOP），但可以通过以下方式实现：
- **函数装饰器**（上面的例子）
- **中间件**（HTTP handler 的 middleware 就是 AOP）
- **AOP 代码生成工具**（如 go-aop）

## 一句话总结

> **AOP 把日志、权限、事务这些"到处都要做"的逻辑抽出来统一处理，让业务代码只关心业务。**
