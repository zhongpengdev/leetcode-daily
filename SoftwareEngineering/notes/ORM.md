# ORM - 对象关系映射 (Object-Relational Mapping)

## 什么是 ORM

数据库的表（table）和编程语言的对象（object）之间存在"阻抗不匹配"（impedance mismatch）：
- 数据库是关系型的（行、列、外键），代码是面向对象的（类、属性、引用）
- ORM 就是**用操作对象的方式来操作数据库**，中间层帮你翻译成 SQL

## 举个例子

没有 ORM：
```go
row := db.QueryRow("SELECT id, name, email FROM users WHERE id = ?", 1)
var id int
var name, email string
row.Scan(&id, &name, &email)
```

有 ORM：
```go
user := db.Find(User{}, 1)
// user.Name, user.Email 直接用
```

## 属于哪个层面

**数据访问层（Data Access Layer）**。在三层架构中属于最底层：

```
Handler (表现层)  →  Service (业务层)  →  Repository (数据层)  →  ORM  →  数据库
```

## Go 中的 ORM

- **GORM**：Go 最流行的 ORM
- **Ent**：Facebook 开源的代码生成型 ORM
- **sqlx**：不是 ORM，是 database/sql 的增强版（推荐轻量使用）

## 优缺点

| 优点 | 缺点 |
|------|------|
| 开发快，少写 SQL | 复杂查询效率低，SQL 难优化 |
| 对象化操作，直观 | N+1 查询问题 |
| 跨数据库兼容 | 学习成本，隐藏了底层细节 |

## 最佳实践

1. **简单 CRUD 用 ORM，复杂查询手写 SQL**
2. 不要让 ORM 代码泄漏到业务层，用 Repository 封装
3. 注意 N+1 问题：查询列表时避免循环里再查关联表

## 一句话总结

> **ORM 让你用操作对象的方式操作数据库，是数据访问层的便利工具，但不能替代对 SQL 的理解。**
