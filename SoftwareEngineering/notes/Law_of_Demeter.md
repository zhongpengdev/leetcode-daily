# 迪米特法则 (Law of Demeter)

## 来历

1987年由美国东北大学（Northeastern University）的 **Ian Holland** 提出。又叫"最少知识原则"（Principle of Least Knowledge）。

核心思想：**一个对象应该对其他对象有最少的了解。只和你的"朋友"说话，不和"陌生人"说话。**

## "朋友"是谁

在方法内部，只允许调用：
1. 当前对象自身的方法
2. 传入参数的方法
3. 当前对象内部创建的对象的方法
4. 当前对象的直接成员（字段）的方法

**不允许**通过链式调用访问深层对象：`a.getB().getC().doSomething()` — 这就是"和陌生人说话"。

## Go 代码示例

### 错误示范：链式调用，违反迪米特法则

```go
type User struct {
    Name   string
    Wallet Wallet
}

type Wallet struct {
    Balance float64
    Cards   []Card
}

type Card struct {
    Number string
    Bank   Bank
}

type Bank struct {
    Name string
}

// 违反迪米特法则：深入穿透多层结构
func processPayment(user *User) {
    bankName := user.Wallet.Cards[0].Bank.Name  // 太了解 User 的内部结构了
    fmt.Println(bankName)
}
```

### 正确示范：通过方法封装

```go
type User struct {
    Name   string
    Wallet Wallet
}

func (u *User) GetPaymentBankName() string {
    return u.Wallet.PrimaryCardBankName()
}

type Wallet struct {
    Balance float64
    Cards   []Card
}

func (w *Wallet) PrimaryCardBankName() string {
    if len(w.Cards) == 0 {
        return ""
    }
    return w.Cards[0].BankName()
}

type Card struct {
    Number   string
    BankName string
}

func (c *Card) BankName() string {
    return c.BankName
}

// 调用者只需要知道 User
func processPayment(user *User) {
    bankName := user.GetPaymentBankName()
    fmt.Println(bankName)
}
```

## 最佳实践

1. **不要链式调用超过两层**：`obj.Method()` 可以，`obj.A.B.C.Method()` 不行
2. **封装内部结构**：外部不应该知道你的成员对象的成员对象
3. **传参而非穿透**：需要深层数据？让上层取出来当参数传给你
4. **和 SRP 配合**：职责清晰的模块，自然不需要深入其他模块的内部

## 一句话总结

> **只和直接朋友通信，不和陌生人说话。你的方法不应该了解对象内部的深层结构。**
