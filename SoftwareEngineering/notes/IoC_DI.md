# IoC 和 DI - 控制反转与依赖注入

## 什么是 IoC

**Inversion of Control（控制反转）**：把对象的创建和依赖关系的管理从代码中"反转"到外部。

传统方式：我在函数里自己创建依赖对象
```go
func NewUserService() *UserService {
    repo := NewMySQLUserRepo()  // 自己创建，自己决定用 MySQL
    return &UserService{repo: repo}
}
```

IoC：我不创建，外部传进来
```go
func NewUserService(repo UserRepository) *UserService {
    return &UserService{repo: repo}  // 用什么 repo，外部决定
}
```

## 什么是 DI

**Dependency Injection（依赖注入）**：IoC 的一种实现方式。说白了就是**通过构造函数/参数把依赖"注入"进来**，而不是在内部自己创建。

```go
// 依赖注入：通过参数注入依赖
type UserService struct {
    repo  UserRepository
    email EmailSender
}

// 构造函数就是"注入点"
func NewUserService(repo UserRepository, email EmailSender) *UserService {
    return &UserService{repo: repo, email: email}
}
```

## 为什么需要

| 没有 DI | 有 DI |
|---------|-------|
| 硬编码依赖，换数据库要改代码 | 传什么用什么，灵活替换 |
| 难以测试（没法 mock） | 可以注入 mock 对象，方便测试 |
| 高耦合 | 低耦合 |

## 属于哪个层面

**设计原则层面**，贯穿所有层。不是某个具体的技术，而是一种编程思想。

## Go 中的实践

Go 没有 DI 容器（不像 Java 的 Spring），推荐手动注入：

```go
// main.go - 组装所有依赖（只在这里创建具体实现）
func main() {
    db := sql.Open("mysql", dsn)
    repo := NewMySQLUserRepo(db)
    email := NewSMTPEmailSender(host)
    userSvc := NewUserService(repo, email)  // 注入

    handler := NewUserHandler(userSvc)
    http.Handle("/users", handler)
    http.ListenAndServe(":8080", nil)
}
```

```go
// service/user.go - 只依赖接口，不依赖具体实现
type UserService struct {
    repo  UserRepository      // 接口
    email EmailSender         // 接口
}
```

## 最佳实践

1. **面向接口编程**：依赖接口而不是具体类型
2. **在 main 函数组装**：只有一个地方知道所有具体实现
3. **不用 DI 框架**：Go 的接口 + 构造函数已经足够，框架反而增加复杂度
4. **和 SRP/SoC 配合**：职责清晰 + 依赖注入 = 松耦合系统

## 一句话总结

> **IoC：不要自己创建依赖，让外部传给你。DI：具体实现方式就是通过构造函数参数注入。**
