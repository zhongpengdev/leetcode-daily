# SRP - 单一职责原则 (Single Responsibility Principle)

## 来历

由 **Robert C. Martin (Uncle Bob)** 在 2000年前后提出，是 **SOLID** 五大原则的第一个。

他的定义："A class should have only one reason to change."（一个类应该只有一个引起它变化的原因。）

核心思想：**一个模块只应该负责一件事。**

## "一件事"怎么理解

不是说一个结构体只能有一个方法，而是：**只有一个角色/职责**。

- `UserService` 管用户的业务逻辑 → 一个职责
- `UserService` 管用户的业务逻辑 + 发邮件 + 写日志 + 生成报表 → 四个职责

## Go 代码示例

### 错误示范：一个结构体做所有事

```go
type OrderService struct {
    db *sql.DB
}

func (s *OrderService) CreateOrder(userID int, items []Item) error {
    // 1. 验证库存
    for _, item := range items {
        var stock int
        s.db.QueryRow("SELECT stock FROM products WHERE id=?", item.ID).Scan(&stock)
        if stock < item.Qty {
            return errors.New("库存不足")
        }
    }

    // 2. 计算价格
    total := 0.0
    for _, item := range items {
        var price float64
        s.db.QueryRow("SELECT price FROM products WHERE id=?", item.ID).Scan(&price)
        total += price * float64(item.Qty)
    }

    // 3. 扣减库存
    for _, item := range items {
        s.db.Exec("UPDATE products SET stock=stock-? WHERE id=?", item.Qty, item.ID)
    }

    // 4. 创建订单
    s.db.Exec("INSERT INTO orders (user_id, total) VALUES (?, ?)", userID, total)

    // 5. 发短信通知
    sendSMS(userID, "下单成功")

    // 6. 记录操作日志
    log.Printf("用户%d创建订单，金额%.2f", userID, total)

    return nil
}
```

### 正确示范：拆分职责

```go
// 只管库存
type InventoryService struct {
    repo InventoryRepository
}

func (s *InventoryService) CheckAndDeduct(items []Item) error {
    for _, item := range items {
        if err := s.repo.Deduct(item.ID, item.Qty); err != nil {
            return err
        }
    }
    return nil
}

// 只管计价
type PricingService struct {
    repo ProductRepository
}

func (s *PricingService) Calculate(items []Item) (float64, error) {
    total := 0.0
    for _, item := range items {
        price, err := s.repo.GetPrice(item.ID)
        if err != nil {
            return 0, err
        }
        total += price * float64(item.Qty)
    }
    return total, nil
}

// 只管订单流程（协调者）
type OrderService struct {
    inventory *InventoryService
    pricing   *PricingService
    orderRepo OrderRepository
}

func (s *OrderService) CreateOrder(userID int, items []Item) error {
    if err := s.inventory.CheckAndDeduct(items); err != nil {
        return err
    }
    total, err := s.pricing.Calculate(items)
    if err != nil {
        return err
    }
    return s.orderRepo.Create(userID, total)
}
```

## 最佳实践

1. **一个结构体一个职责**：问自己"如果 X 需求变了，要改这个结构体吗？"如果答案是"不一定"，说明职责混了
2. **一个文件一个包**：Go 的 package 机制天然支持按职责分组
3. **函数同理**：一个函数也只做一件事。超过 30 行考虑拆分
4. **命名体现职责**：`EmailService`、`PaymentGateway`、`UserRepository` — 名字就告诉你它干什么

## 与高内聚的关系

SRP 是高内聚的**原因**，高内聚是 SRP 的**结果**。

当你遵循 SRP 时，模块内所有代码都是为了同一个职责服务，自然就是高内聚。

## 一句话总结

> **一个模块只管一件事，改需求时只动一个地方。**
