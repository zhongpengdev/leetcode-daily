# 索引

---

## 1. 什么是索引

索引是**排好序的快速查找结构**。类比书的目录：查某个词时翻目录定位页码，而不是翻遍全书。

MySQL InnoDB 默认用 **B+Tree** 存储索引：

```
        [30 | 70]               ← 非叶子节点（只存 key + 指针）
       /    |    \
  [5|12]  [35|50]  [80|90]     ← 非叶子节点
   / | \   / | \    / | \
  [1] [5] [12] ...              ← 叶子节点（存完整数据或主键值），双向链表连接
```

- 叶子节点按 key **顺序存放**，用双向链表串连 → 范围查询极快。
- 树的高度很低（千万级数据只需 3-4 层）→ 查询稳定，磁盘 I/O 次数少。

**一个索引查询的 I/O 次数 ≈ 树的高度。**

---

## 2. 索引类型

### 主键索引（聚簇索引）

```sql
CREATE TABLE t (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY
);
```

- 叶子节点存**整行数据**。
- 一张表只能有一个聚簇索引。
- **没有显式主键时**，InnoDB 用第一个 UNIQUE NOT NULL 列；都没有则创建隐藏的 row_id。

### 二级索引（非聚簇索引 / 辅助索引）

```sql
CREATE INDEX idx_name ON users(name);
```

- 叶子节点存的是索引列 + **主键值**。
- 查到主键后还需**回表**（再去聚簇索引取完整行）。

```
二级索引树:  idx_name
                              [Li]
                             /     \
                        [Chen]    [Wang]
                         / \        / \
                      ... ...     ... ...
叶子节点: (Chen, id=3) → (Chen, id=7) → (Li, id=1) → (Wang, id=2)
                                      ↓
拿到 id=1 后  →  回表到聚簇索引查出完整行
```

### 唯一索引

```sql
CREATE UNIQUE INDEX idx_email ON users(email);
```

- 比普通索引多一层**唯一性校验**，其余相同。

### 联合索引（复合索引）

```sql
CREATE INDEX idx_a_b ON t(a, b);
```

- 按列顺序排序：先按 a，a 相同时按 b。
- **最左前缀原则**：能利用索引的前提是查询条件包含最左列。

```sql
SELECT * FROM t WHERE a = 1;              -- ✅ 走索引
SELECT * FROM t WHERE a = 1 AND b = 2;    -- ✅ 走索引
SELECT * FROM t WHERE b = 2;              -- ❌ 不走索引（不是最左列）
SELECT * FROM t WHERE a = 1 AND b > 2;    -- ✅ 走索引（范围查询后面索引失效）
```

### 前缀索引

```sql
-- 长字符串只索引前 N 个字符
CREATE INDEX idx_title_prefix ON articles(title(20));
```

**权衡：** 前缀越短空间越小，但区分度越低，回表后需要二次判断的越多。

```sql
-- 选取区分度足够高的前缀长度
SELECT COUNT(DISTINCT LEFT(title, 10)) / COUNT(*) FROM articles;  -- > 0.9 即可
```

---

## 3. 执行计划看索引

```sql
EXPLAIN SELECT * FROM users WHERE name = 'Alice';
```

| 关键字段 | 含义 |
|---------|------|
| type | ALL(全表扫描, 最差) → index → range → ref → eq_ref → const(最优) |
| key | 实际使用的索引，NULL 表示没用到 |
| rows | 预估扫描行数 |
| Extra | Using index(覆盖索引, 无需回表) / Using filesort(需额外排序) / Using temporary(需临时表) |

**目标：type 达到 range 及以上，Extra 不出现 filesort 和 temporary。**

---

## 4. 索引失效场景

| 场景 | 原因 |
|------|------|
| `WHERE a LIKE '%xxx'` | 左模糊，无法利用 B+Tree 有序性 |
| `WHERE col / 2 > 10` | 索引列参与运算，优化器放弃索引 |
| `WHERE col IS NULL` | NULL 值不参与索引比较（部分版本） |
| `WHERE a = 1 OR b = 2` | 两列独立索引，OR 可能导致全表扫描 |
| `WHERE a != 1` | 不等于、NOT IN 通常不走索引 |
| 类型隐式转换 `WHERE phone = 13800001111` | phone 是 VARCHAR 但传了数字，索引失效 |
| 联合索引不满足最左前缀 | 跳过了最左列 |

```sql
-- 注意点：
-- 隐式类型转换
WHERE phone = 13800001111     -- 错：phone 是 VARCHAR，传入数字
WHERE phone = '13800001111'   -- 对

-- 函数包裹索引列
WHERE YEAR(created_at) = 2024     -- 不走索引
WHERE created_at >= '2024-01-01' AND created_at < '2025-01-01'  -- 走索引
```

---

## 5. 覆盖索引

**查询所需的列都在索引里，不需要回表。**

```sql
-- idx_name_age 包含 (name, age)
CREATE INDEX idx_name_age ON users(name, age);

SELECT name, age FROM users WHERE name = 'Alice';   -- ✅ 覆盖索引，Extra = Using index
SELECT name, age, email FROM users WHERE name = 'Alice';  -- ❌ email 不在索引中，需回表
```

**工程用法：** 建二级索引时把 SELECT 中高频的列也塞进索引（在索引列后面追加），避免回表。

---

## 6. 建索引的原则

**建前先问：这个列/组合查询频率高吗？区分度够吗？**

```sql
-- 区分度
SELECT COUNT(DISTINCT status) FROM orders;   -- 2 个值 → 不建
SELECT COUNT(DISTINCT order_no) FROM orders; -- 接近总行数 → 建
```

| 该建 | 不该建 |
|------|--------|
| WHERE / JOIN ON / ORDER BY 列 | 区分度极低的列（性别、布尔） |
| 联合索引覆盖高频组合查询 | 频繁更新的列（维护索引有代价） |
| 数据量大的表 | 小表（全表扫描更快） |
| | 已有多余索引覆盖的列 |

**索引不是越多越好**：每次 INSERT/UPDATE/DELETE 都要维护所有索引，写性能下降。

---

## 7. 一句话总结

> 索引用 B+Tree 把随机查找变成有序定位。主键索引存整行，二级索引存主键值（需回表）。建索引看查询频率和区分度，EXPLAIN 验证生效。
