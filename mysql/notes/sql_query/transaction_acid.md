# 事务与 ACID

事务是一组操作，**要么全做，要么全不做**。

---

## 1. 原子性（Atomicity）

事务中的操作是一个**不可分割的整体**。任何一个失败，全部回滚。

```sql
-- 转账：扣钱和加钱是一个原子操作
START TRANSACTION;
UPDATE accounts SET balance = balance - 100 WHERE id = 1;   -- 扣钱
UPDATE accounts SET balance = balance + 100 WHERE id = 2;   -- 加钱
COMMIT;  -- 都成功，提交

-- 如果第二条失败：
START TRANSACTION;
UPDATE accounts SET balance = balance - 100 WHERE id = 1;
UPDATE accounts SET balance = balance + 100 WHERE id = 999; -- 用户不存在，失败
ROLLBACK; -- 第一条也撤销，余额不变
```

**实现机制：** InnoDB 通过 undo log（回滚日志）实现。每修改一行，先把旧值记入 undo log，回滚时用 undo log 恢复。

---

## 2. 一致性（Consistency）

事务前后数据都**满足所有约束**（主键、外键、NOT NULL、CHECK 等）。

```sql
START TRANSACTION;
INSERT INTO orders (user_id, amount) VALUES (99999, -100);  -- 金额为负，CHECK 约束拦下
COMMIT;  -- 失败，事务回滚，数据库保持合法状态
```

**这一条不是 MySQL 实现的，而是 A、I、D 三条共同保证的结论。**

---

## 3. 隔离性（Isolation）

多个并发事务之间**互不干扰**，仿佛独占数据库。

### 并发问题

| 问题 | 描述 |
|------|------|
| 脏读 | 读到别的事务未提交的数据，对方可能回滚 |
| 不可重复读 | 同一事务两次读同一行，结果不同（被别的事务 UPDATE 了） |
| 幻读 | 同一事务两次查询同一条件，结果行数不同（别的事务 INSERT 了新行） |

### 四种隔离级别

```sql
-- 查看当前级别
SELECT @@transaction_isolation;

-- 设置级别
SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;
```

| 级别 | 脏读 | 不可重复读 | 幻读 | 性能 |
|------|------|-----------|------|------|
| READ UNCOMMITTED | 会 | 会 | 会 | 最高 |
| READ COMMITTED | 否 | 会 | 会 | 高 |
| REPEATABLE READ（MySQL 默认） | 否 | 否 | 基本否 | 中 |
| SERIALIZABLE | 否 | 否 | 否 | 最低 |

**工程选择：** MySQL 默认 REPEATABLE READ 即可覆盖绝大多数场景。对一致性要求极高的金融系统用 SERIALIZABLE 或在应用层加锁。

```sql
START TRANSACTION;
SELECT * FROM products WHERE id = 1 FOR UPDATE;  -- 行级锁，事务期间别的事务无法改此行
-- 业务逻辑...
UPDATE products SET stock = stock - 1 WHERE id = 1;
COMMIT;
```

### 实现机制：MVCC（多版本并发控制）

> 读写不互斥，读不加锁，靠版本号看到数据的某个历史快照。

每行数据有两个隐藏列：
- `trx_id`：最近修改这行的事务 ID
- `roll_pointer`：指向 undo log 中的旧版本

SELECT 读数据时：根据事务开始时刻的 ReadView（活跃事务列表），判断哪条 undo log 版本可见。不是永远读同一行，而是读**当时最新且已提交**的版本。

这解释了为什么 REPEATABLE READ 下同一事务多次查询返回相同结果——ReadView 在事务开始时生成一次，整个事务期间不变。

---

## 4. 持久性（Durability）

一旦事务 COMMIT，数据**永久保存**，即使数据库立刻宕机也不丢失。

**实现机制：** redo log（重做日志）。修改先写 redo log，再刷到磁盘。

```
事务修改 → 写 redo log（顺序写，快） → COMMIT 返回（此时已算持久）
                                               ↓
                                          后台线程慢慢刷盘
```

如果宕机：
- redo log 中有记录但数据页未刷盘 → **重启时用 redo log 重放**，恢复至提交后状态。
- redo log 中无记录 → 事务未提交，丢弃。

**binlog 的区别：** binlog 是 Server 层的归档日志，用于主从复制和基于时间点的数据恢复。redo log 是 InnoDB 引擎层的崩溃恢复日志。事务提交时先写 redo log（prepare），再写 binlog，最后 redo log 变成 commit——这就是经典的**两阶段提交**，保证两份日志一致。

---

## 总结

> 原子性：undo log 回滚。隔离性：MVCC + 锁。持久性：redo log 重放。一致性：前三者共同保证。
