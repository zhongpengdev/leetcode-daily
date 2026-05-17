# 高内聚低耦合

## 来历

这个原则最早由 **Larry Constantine** 在 1960年代末提出，是结构化设计（Structured Design）的核心。后来被 **Ed Yourdon** 发扬光大，成为软件工程的基本原则之一。

核心思想：**一个模块应该做好一件事（高内聚），模块之间应该尽量少地互相依赖（低耦合）。**

## 什么是内聚

内聚衡量的是**一个模块内部各元素之间的关联程度**。

从低到高排列：

| 内聚类型 | 说明 | 质量 |
|---------|------|------|
| 偶然内聚 | 模块内元素毫无关系，只是碰巧放一起 | 最差 |
| 逻辑内聚 | 逻辑上相似但功能不同（比如所有文件操作放一起） | 差 |
| 时间内聚 | 同时执行的操作放一起（比如初始化函数） | 一般 |
| 过程内聚 | 按特定顺序执行 | 一般 |
| 通信内聚 | 操作同一组数据 | 较好 |
| 顺序内聚 | 前一个操作的输出是后一个的输入 | 好 |
| 功能内聚 | 模块内所有元素只做一件事 | 最好 |

## 什么是耦合

耦合衡量的是**模块之间的依赖程度**。

从高到低排列：

| 耦合类型 | 说明 | 质量 |
|---------|------|------|
| 内容耦合 | A 直接修改 B 的内部数据 | 最差 |
| 公共耦合 | A 和 B 共享全局变量 | 差 |
| 外部耦合 | A 和 B 共享外部数据格式 | 差 |
| 控制耦合 | A 传 flag 控制 B 的内部逻辑 | 一般 |
| 数据耦合 | A 只传必要参数给 B | 好 |
| 非直接耦合 | A 和 B 无直接关系，通过接口/事件通信 | 最好 |

## Go 代码示例

### 错误示范：低内聚高耦合

```go
// 一个 God Object，什么都做
type UserService struct {
    db *sql.DB
}

// 注册：既管用户逻辑，又管数据库，又管日志，又管邮件
func (s *UserService) Register(name, email, password string) error {
    // 1. 验证
    if len(password) < 6 {
        return errors.New("密码太短")
    }

    // 2. 加密密码（业务逻辑混入了密码学细节）
    hashed := sha256.Sum256([]byte(password))

    // 3. 直接操作数据库（SQL 耦合）
    _, err := s.db.Exec("INSERT INTO users (name, email, password) VALUES (?, ?, ?)",
        name, email, hex.EncodeToString(hashed[:]))
    if err != nil {
        return err
    }

    // 4. 发邮件（一个用户服务为什么要管发邮件？）
    sendEmail(email, "欢迎注册", "注册成功！")

    // 5. 写日志（日志应该由调用者处理）
    log.Println("用户注册成功:", name)

    return nil
}
```

问题：
- **低内聚**：UserService 既管验证、又管加密、又管数据库、又管邮件、又管日志
- **高耦合**：直接依赖数据库驱动、邮件服务、日志模块

### 正确示范：高内聚低耦合

```go
// 1. 定义接口，不关心具体实现
type UserRepository interface {
    Create(user User) error
}

type EmailSender interface {
    SendWelcome(to string) error
}

// 2. 只管业务逻辑的 UserService
type UserService struct {
    repo  UserRepository
    email EmailSender
}

func (s *UserService) Register(name, email, password string) error {
    // 验证
    if err := validate(password); err != nil {
        return err
    }

    // 加密交给专门的工具
    hashed := hashPassword(password)

    // 操作数据交给 repo
    user := User{Name: name, Email: email, Password: hashed}
    if err := s.repo.Create(user); err != nil {
        return err
    }

    // 发邮件交给 email sender
    return s.email.SendWelcome(email)
}

// 3. 数据库实现，只关心怎么存
type MySQLUserRepo struct {
    db *sql.DB
}

func (r *MySQLUserRepo) Create(user User) error {
    _, err := r.db.Exec("INSERT INTO users ...")
    return err
}

// 4. 邮件实现，只关心怎么发
type SMTPEmailSender struct {
    host string
}

func (e *SMTPEmailSender) SendWelcome(to string) error {
    // smtp 发送逻辑
    return nil
}
```

好处：
- **高内聚**：UserService 只管注册流程，UserRepo 只管存数据，EmailSender 只管发邮件
- **低耦合**：通过接口通信，换数据库、换邮件服务不需要改 UserService

## 最佳实践

1. **单一职责（SRP）**：一个结构体/函数只做一件事。如果一个函数超过 50 行，考虑拆分。

2. **面向接口编程**：依赖接口而不是具体实现。Go 的接口是隐式实现的，天然支持这个模式。

3. **最小知识原则**：一个模块不需要知道另一个模块的内部细节。只暴露必要的方法。

4. **避免全局状态**：全局变量导致公共耦合，改用依赖注入。

5. **分层架构**：

```
Handler (HTTP层)  →  Service (业务层)  →  Repository (数据层)
        ↓                   ↓                    ↓
    只管HTTP请求        只管业务逻辑         只管数据存取
```

每层只和相邻层通信，不跨层调用。

## 一句话总结

> **把相关的东西放在一起（高内聚），把不相关的隔离开（低耦合）。**
