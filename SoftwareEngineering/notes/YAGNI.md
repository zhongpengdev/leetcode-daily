# YAGNI - You Ain't Gonna Need It

## 来历

来自 **极限编程（Extreme Programming, XP）** 运动，由 **Kent Beck** 在 1990年代末提出。是 XP 的核心实践之一。

核心思想：**不要实现你现在不需要的功能。"以后可能会用到"不是写代码的理由。**

## 为什么

- 你猜的"未来需求"几乎永远不会发生
- 每一行代码都是成本：写、读、测、维护
- 过度设计让系统变复杂，真正需求来时反而更难改

## Go 代码示例

### 错误示范：过度准备

```go
// 需求：只需要存用户姓名和邮箱
// 但开发者想"以后可能需要"，提前加了一堆字段

type User struct {
    ID           int
    Name         string
    Email        string
    Phone        string     // "以后可能需要"
    Address      string     // "以后可能需要"
    Avatar       string     // "以后可能需要"
    Role         string     // "以后可能需要"
    Preferences  map[string]interface{}  // "万能配置"
    Metadata     []byte     // "留着以后用"
    CreatedAt    time.Time
    UpdatedAt    time.Time
    DeletedAt    *time.Time // "软删除以后会用"
}

type UserService struct {
    repo    UserRepository
    cache   CacheLayer    // "以后性能可能会是问题"
    logger  Logger
    metrics Metrics       // "以后可能要监控"
    events  EventBus      // "以后可能要通知其他服务"
}

func NewUserService(repo UserRepository) *UserService {
    return &UserService{
        repo:    repo,
        cache:   NewRedisCache(),   // 现在根本不需要缓存
        logger:  NewLogger(),
        metrics: NewPrometheus(),
        events:  NewKafkaBus(),
    }
}
```

### 正确示范：只做当下需要的

```go
type User struct {
    ID    int
    Name  string
    Email string
}

type UserService struct {
    repo UserRepository
}

func NewUserService(repo UserRepository) *UserService {
    return &UserService{repo: repo}
}
```

以后真的需要 Phone、缓存、事件？到时候加就是了。**重构比预设计便宜得多。**

## 最佳实践

1. **需求明确时才写代码**，不要为假设的需求编码
2. **先实现最简版本**，能用就不改
3. **"以后会用到" = 不要现在做**
4. **保持代码可改性**比预测未来更重要
5. **写注释可以**：`// TODO: 将来支持多语言`，但别写实现

## 与 KISS / DRY 的关系

三个原则协同工作：

- **KISS**：保持简单
- **DRY**：不要重复
- **YAGNI**：不要提前做

顺序应该是：先 **YAGNI**（决定做什么），再 **KISS**（保持简单），再 **DRY**（消除重复）。

## 一句话总结

> **最好的代码是你没写的代码。实现你需要的，不多写一行。**
