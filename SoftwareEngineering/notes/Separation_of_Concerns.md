# 关注点分离 (Separation of Concerns, SoC)

## 来历

由 **Edsger W. Dijkstra**（图灵奖得主）在 1974年的论文中提出。

核心思想：**把系统按"关注点"（concern）拆分成不同部分，每个部分只处理一个关注点。**

## 和 SRP 的区别

- **SRP**：一个模块只有一个职责（粒度更细）
- **SoC**：系统层面按关注点分层/分模块（粒度更粗，是架构层面的）

SoC 是 SRP 的"上层建筑"。SRP 管一个函数/结构体，SoC 管一个系统怎么划分。

## Go 代码示例

### 错误示范：所有逻辑混在一起

```go
func handleOrder(w http.ResponseWriter, r *http.Request) {
    // 读请求
    body, _ := io.ReadAll(r.Body)
    var req struct {
        UserID int    `json:"user_id"`
        Items  []Item `json:"items"`
    }
    json.Unmarshal(body, &req)

    // 验证
    if req.UserID == 0 {
        http.Error(w, "user_id required", 400)
        return
    }

    // 查数据库
    db, _ := sql.Open("mysql", "dsn")
    var stock int
    db.QueryRow("SELECT stock FROM products WHERE id=?", req.Items[0].ID).Scan(&stock)

    // 业务逻辑
    total := 0.0
    // ... 计算价格

    // 写数据库
    db.Exec("INSERT INTO orders ...")

    // 返回响应
    w.Header().Set("Content-Type", "application/json")
    json.NewEncoder(w).Encode(map[string]interface{}{"order_id": 123})
}
```

### 正确示范：按关注点分层

```
handler/    → 只关心 HTTP（读请求、返回响应）
service/    → 只关心业务逻辑（验证、计算、流程编排）
repository/ → 只关心数据库（CRUD）
model/      → 只关心数据结构定义
```

```go
// handler/order.go - HTTP 层
func (h *OrderHandler) Create(w http.ResponseWriter, r *http.Request) {
    var req CreateOrderRequest
    if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
        respondError(w, 400, "invalid request")
        return
    }
    order, err := h.service.Create(req.UserID, req.Items)
    if err != nil {
        respondError(w, 500, err.Error())
        return
    }
    respondJSON(w, 201, order)
}

// service/order.go - 业务层
func (s *OrderService) Create(userID int, items []Item) (*Order, error) {
    if userID == 0 {
        return nil, errors.New("user_id required")
    }
    // 业务逻辑...
}

// repository/order_repo.go - 数据层
func (r *OrderRepo) Create(order *Order) error {
    _, err := r.db.Exec("INSERT INTO orders ...")
    return err
}
```

## 最佳实践

1. **三层架构**：Handler（表现层）→ Service（业务层）→ Repository（数据层）
2. **每层只和相邻层通信**：Handler 不直接碰数据库，Repository 不管业务逻辑
3. **关注点按维度拆分**：业务 vs 数据 vs 展示 vs 配置 vs 日志 vs 错误处理
4. **前端同理**：HTML（结构）+ CSS（样式）+ JS（行为）就是最经典的 SoC

## 一句话总结

> **一个模块只关心一件事。HTTP 的归 HTTP，业务的归业务，数据库的归数据库。**
