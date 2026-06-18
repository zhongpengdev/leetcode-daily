# 常见设计模式 (Design Patterns)

## 来历

由 **Gang of Four (GoF)**（Erich Gamma、Richard Helm、Ralph Johnson、John Vlissides）在 1994年出版的《设计模式：可复用面向对象软件的基础》中总结的 23 种经典模式。

分为三大类：**创建型、结构型、行为型**。

## 创建型模式

### 工厂模式 (Factory)

不直接 `new` 对象，而是通过工厂函数创建。

```go
// 根据类型创建不同的支付方式
type Payment interface {
    Pay(amount float64) error
}

func NewPayment(method string) Payment {
    switch method {
    case "alipay":
        return &AliPay{}
    case "wechat":
        return &WeChatPay{}
    default:
        return &CreditCard{}
    }
}

// 使用
p := NewPayment("alipay")
p.Pay(100)
```

**适用场景**：创建逻辑复杂，或需要根据条件创建不同类型。

### 单例模式 (Singleton)

全局只有一个实例。Go 用 `sync.Once` 实现最优雅。

```go
var (
    instance *Config
    once     sync.Once
)

func GetConfig() *Config {
    once.Do(func() {
        instance = loadConfig()  // 只执行一次
    })
    return instance
}
```

**适用场景**：数据库连接池、配置、日志器。**少用**，全局状态是耦合的来源。

## 结构型模式

### 适配器模式 (Adapter)

把一个接口转换成另一个接口。比如你的系统用 `Logger` 接口，但引入的第三方库是 `logrus.Logger`。

```go
// 系统定义的接口
type Logger interface {
    Info(msg string)
}

// 第三方库的实现，接口不匹配
type LogrusLogger struct {
    log *logrus.Logger
}

// 适配器
func (l *LogrusLogger) Info(msg string) {
    l.log.Info(msg)  // 转换调用
}
```

**适用场景**：集成第三方库、兼容新旧接口。

### 装饰器模式 (Decorator)

动态地给对象添加功能，不修改原有代码。

```go
type Handler func(w http.ResponseWriter, r *http.Request)

// 装饰器：给 handler 加缓存
func WithCache(h Handler) Handler {
    return func(w http.ResponseWriter, r *http.Request) {
        if cached := cache.Get(r.URL.Path); cached != nil {
            w.Write(cached)
            return
        }
        // 调用原始 handler（这里需要捕获响应，简化起见省略）
        h(w, r)
    }
}
```

**适用场景**：给已有功能追加行为（缓存、日志、重试）。HTTP 中间件就是装饰器模式。

## 行为型模式

### 策略模式 (Strategy)

定义一组算法，让它们可以互相替换。

```go
type SortStrategy interface {
    Sort(data []int)
}

type QuickSort struct{}
func (q QuickSort) Sort(data []int) { /* 快排 */ }

type MergeSort struct{}
func (m MergeSort) Sort(data []int) { /* 归并 */ }

type Sorter struct {
    strategy SortStrategy
}

func (s *Sorter) SetStrategy(strategy SortStrategy) {
    s.strategy = strategy
}

func (s *Sorter) Sort(data []int) {
    s.strategy.Sort(data)
}
```

**适用场景**：算法需要灵活切换。Go 中常用函数类型简化：

```go
type SortFunc func([]int)

func Sort(data []int, fn SortFn) {
    fn(data)
}
```

### 观察者模式 (Observer)

一对多依赖，一个对象变化时通知所有依赖者。

```go
type Event struct {
    Type string
    Data interface{}
}

type Observer func(Event)

type EventBus struct {
    observers map[string][]Observer
}

func (e *EventBus) Subscribe(eventType string, obs Observer) {
    e.observers[eventType] = append(e.observers[eventType], obs)
}

func (e *EventBus) Publish(event Event) {
    for _, obs := range e.observers[event.Type] {
        obs(event)
    }
}

// 使用
bus := &EventBus{observers: make(map[string][]Observer)}
bus.Subscribe("order_created", func(e Event) { sendEmail(e) })
bus.Subscribe("order_created", func(e Event) { sendSMS(e) })
bus.Publish(Event{Type: "order_created", Data: order})
```

**适用场景**：事件驱动、解耦发布者和订阅者。

## Go 的特色

Go 不是传统的 OOP 语言，很多模式可以更简洁地实现：
- **接口是隐式实现的**：不需要 implements 关键字
- **函数是一等公民**：策略模式可以用函数类型代替接口
- **组合优于继承**：装饰器用组合，不用继承链
- **goroutine + channel**：观察者模式可以用 channel 替代

## 一句话总结

> **设计模式是前人总结的代码"套路"，不是教条。Go 的哲学是：能用简单方式解决的，不要套模式。**
