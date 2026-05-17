# DRY - Don't Repeat Yourself

## 来历

由 **Andy Hunt 和 Dave Thomas** 在 1999年的经典著作《程序员修炼之道》（The Pragmatic Programmer）中提出。

原话："Every piece of knowledge must have a single, unambiguous, authoritative representation within a system."

核心思想：**每一项知识在系统中只应有一个唯一的、明确的表示。不要复制粘贴代码。**

## 为什么重复有害

- 改一处忘改另一处 → bug
- 同样的逻辑散落各处 → 维护噩梦
- 代码膨胀 → 难以理解

## Go 代码示例

### 错误示范：到处重复

```go
func createUser(name, email string) error {
    if email == "" {
        return errors.New("email is required")
    }
    if !strings.Contains(email, "@") {
        return errors.New("invalid email")
    }
    // ... 创建用户
}

func updateUser(id int, email string) error {
    if email == "" {
        return errors.New("email is required")
    }
    if !strings.Contains(email, "@") {
        return errors.New("invalid email")
    }
    // ... 更新用户
}

func resetPassword(email string) error {
    if email == "" {
        return errors.New("email is required")
    }
    if !strings.Contains(email, "@") {
        return errors.New("invalid email")
    }
    // ... 重置密码
}
```

### 正确示范：提取公共逻辑

```go
func validateEmail(email string) error {
    if email == "" {
        return errors.New("email is required")
    }
    if !strings.Contains(email, "@") {
        return errors.New("invalid email")
    }
    return nil
}

func createUser(name, email string) error {
    if err := validateEmail(email); err != nil {
        return err
    }
    // ...
}

func updateUser(id int, email string) error {
    if err := validateEmail(email); err != nil {
        return err
    }
    // ...
}
```

## 注意：DRY 不是绝对的

**WET 原则**（Write Everything Twice）：先重复一次，第二次再考虑抽取。

因为：
- 过早抽象可能方向错了，白费力气
- 两段看似相同的代码，未来需求可能分化
- 强行合并相似但不完全相同的逻辑，反而增加复杂度

## 最佳实践

1. **相同逻辑只写一次**，提取为函数/方法
2. **配置值只定义一次**，用常量或配置文件
3. **数据库 schema 只定义一次**，用 migration 管理
4. **2 次重复可以忍，第 3 次必须抽**
5. **DRY 不等于少写代码**，抽象本身有成本，过度抽象比重复更糟

## 一句话总结

> **代码重复是万恶之源，但不要为了消除重复而制造更大的复杂度。**
