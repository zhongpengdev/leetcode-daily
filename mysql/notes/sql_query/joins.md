# 多表连接

## 1. INNER JOIN（内连接）

取两表**共有**的行。

```sql
SELECT u.name, o.order_no
FROM users u
INNER JOIN orders o ON u.id = o.user_id;
```

**工程用法：** 关联查询的主力，90% 的 JOIN 都是 INNER JOIN。

---

## 2. LEFT JOIN（左外连接）

左表**全部保留**，右表无匹配填 NULL。

```sql
-- 所有用户 + 其订单（无订单的用户也要显示）
SELECT u.name, o.order_no
FROM users u
LEFT JOIN orders o ON u.id = o.user_id;

-- 只取左表独有：找没有订单的用户
SELECT u.*
FROM users u
LEFT JOIN orders o ON u.id = o.user_id
WHERE o.id IS NULL;
```

**工程用法：** 主表 + 可选附属信息的标准写法。

**陷阱：** 右表的过滤条件写在 WHERE 会导致 LEFT JOIN 退化为 INNER JOIN。

```sql
-- 错误：WHERE 把右表 NULL 行滤掉了
SELECT u.name, o.order_no
FROM users u
LEFT JOIN orders o ON u.id = o.user_id
WHERE o.status = 'paid';   -- 没订单的用户被干掉了

-- 正确：过滤条件放 ON 里
SELECT u.name, o.order_no
FROM users u
LEFT JOIN orders o ON u.id = o.user_id AND o.status = 'paid';
```

---

## 3. FULL OUTER JOIN（全外连接）

两表**所有行都保留**，无匹配方填 NULL。

MySQL **不直接支持** FULL OUTER JOIN，用 UNION 模拟：

```sql
SELECT u.name, o.order_no
FROM users u
LEFT JOIN orders o ON u.id = o.user_id

UNION   -- 去重

SELECT u.name, o.order_no
FROM users u
RIGHT JOIN orders o ON u.id = o.user_id;
```

**工程用法：** 极少用。真有需求时考虑是否设计有问题。

---

## 4. 自连接（Self Join）

表**自己连自己**，用别名区分。

```sql
-- 找每个员工的上级名字（上级也在同一张 employee 表里）
SELECT e.name AS employee, m.name AS manager
FROM employee e
LEFT JOIN employee m ON e.manager_id = m.id;
```

**工程用法：** 树形结构（组织架构、分类层级、回复链）。

---

## 5. CROSS JOIN（交叉连接）

笛卡尔积，两表每行都配对。**不加 ON 条件**。

```sql
-- 生成所有城市+月份的组合（报表矩阵用）
SELECT c.city, m.month
FROM cities c
CROSS JOIN months m;
```

**工程用法：** 生成组合矩阵、补全缺失行。生产数据慎用，行数 = N × M。

---

## 6. USING 语法糖

当两表关联列同名时：

```sql
-- 等价于 INNER JOIN ... ON u.id = o.id
SELECT * FROM users u INNER JOIN orders o USING (id);
```

**建议不要用。** 可读性不如显式 ON，改名/重构容易出问题。

---

## JOIN 速查

| 类型 | 行为 | 结果行数 |
|------|------|---------|
| INNER JOIN | 取交集 | ≤ min(左, 右) |
| LEFT JOIN | 左全保留 | ≥ 左表行数 |
| RIGHT JOIN | 右全保留 | ≥ 右表行数 |
| FULL JOIN | 两边全保留 | ≤ 左 + 右 |
| CROSS JOIN | 笛卡尔积 | 左 × 右 |

---

## 性能要点

- **ON 条件的列必须建索引。** JOIN 列无索引 = 全表扫描 × N。
- 多表 JOIN 先缩小中间结果集，再连下一表。
- EXPLAIN 看执行计划：`type` 列最好是 ref/eq_ref，避免 ALL。

---

## 替代方案

| JOIN 场景 | 替代 | 适用情况 |
|-----------|------|---------|
| 反复连同一张维度表 | 子查询 / EXISTS | 只需判断"有无"而非取字段 |
| 多对多 JOIN 导致行膨胀 | 应用层分两次查，代码组装 | 数据量大、业务逻辑复杂 |
| 深度嵌套 JOIN（3层+） | 视图、CTE(8.0+) 分层拆解 | 可读性优先 |
| 大数据量关联 | 冗余字段（反范式） | 读多写少，用空间换 JOIN |
| 维度表 JOIN | 应用层缓存（Redis） | 维度表小且变化少 |

**原则：能用 INNER JOIN 不用子查询；只需判断存在用 EXISTS；关联字段必加索引。**

```sql
-- 例：EXISTS 替代 JOIN（只判断"有订单的用户"）
SELECT * FROM users u
WHERE EXISTS (SELECT 1 FROM orders o WHERE o.user_id = u.id);
```
