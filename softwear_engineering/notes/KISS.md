# KISS - Keep It Simple, Stupid

## 来历

由美国海军工程师 **Kelly Johnson**（洛克希德·马丁臭鼬工厂首席工程师）在 1960年代提出。他的设计哲学是：战斗机的系统必须足够简单，让一个普通机械师在战场条件下用基本工具就能维修。

核心思想：**能简单就简单，不要过度设计。**

## 为什么

- 简单的代码更容易读、更容易改、更少 bug
- 聪明的代码往往只有作者自己能看懂，三个月后连作者也看不懂
- 复杂度是软件的头号敌人

## Go 代码示例

### 错误示范：过度设计

```go
// 为了"可扩展"，搞了一堆抽象
type Validator interface {
    Validate(value interface{}) error
}

type ValidationRule interface {
    Check(data interface{}) bool
    Message() string
}

type ValidationEngine struct {
    rules []ValidationRule
    validators map[string]Validator
    middleware []func(Validator) Validator
}

func NewValidationEngine() *ValidationEngine {
    return &ValidationEngine{
        rules:      make([]ValidationRule, 0),
        validators: make(map[string]Validator),
        middleware: make([]func(Validator) Validator, 0),
    }
}

// 调用者需要理解一整套抽象
engine := NewValidationEngine()
engine.Register("email", &EmailValidator{})
engine.AddMiddleware(&LoggingMiddleware{})
engine.AddRule(&RequiredRule{Field: "email"})
err := engine.Validate(data)
```

### 正确示范：简单直接

```go
func validateEmail(email string) error {
    if email == "" {
        return errors.New("email is required")
    }
    if !strings.Contains(email, "@") {
        return errors.New("invalid email format")
    }
    return nil
}

// 调用者一眼就懂
if err := validateEmail(input.Email); err != nil {
    return err
}
```

## 最佳实践

1. **先写能工作的最简单方案**，需要时再抽象
2. **函数不超过 30 行**，超过就拆
3. **避免过早抽象**：只有当你看到重复 3 次以上时，才考虑抽取
4. **命名要直白**：`getUserByID` 比 `resolveEntityByPrimaryKey` 好
5. **注释说明 Why，而不是 What**：代码本身应该能表达 What

## 一句话总结

> **简单即美。当你觉得自己的方案很"聪明"时，停下来想想有没有更简单的方式。**
