# SELECT 基本查询

ASC 默认升序 小 -> 大
DESC 降序

## 1. WHERE 条件

### 比较运算符：`=` `!=`/`<>` `>` `<` `>=` `<=`

```sql
SELECT * FROM users WHERE age > 18;
SELECT * FROM users WHERE name = 'Alice';
```

### 逻辑运算符：AND、OR、NOT

```sql
-- AND：都满足
SELECT * FROM users WHERE age > 18 AND status = 'active';
-- OR：任一满足
SELECT * FROM users WHERE city = '北京' OR city = '上海';
-- NOT：取反
SELECT * FROM users WHERE NOT status = 'banned';
-- 括号控制优先级
SELECT * FROM users WHERE (age > 18 OR vip = 1) AND status = 'active';
```

**优先级：** AND 高于 OR。拿不准时加括号。

### BETWEEN：范围（含边界）

```sql
SELECT * FROM products WHERE price BETWEEN 100 AND 500;
-- 等价于 price >= 100 AND price <= 500
SELECT * FROM orders WHERE created_at BETWEEN '2024-01-01' AND '2024-12-31';
```

### IN：值在集合内

```sql
SELECT * FROM users WHERE role IN ('admin', 'editor', 'reviewer');
SELECT * FROM users WHERE id IN (1, 3, 5);
-- 子查询场景更常用
SELECT * FROM orders WHERE user_id IN (SELECT id FROM users WHERE vip = 1);
```

**能用 IN 不用多个 OR。**

### LIKE：模糊匹配

```sql
SELECT * FROM users WHERE name LIKE '张%';     -- 以"张"开头
SELECT * FROM users WHERE email LIKE '%@gmail.com'; -- 以"@gmail.com"结尾
SELECT * FROM users WHERE title LIKE '%工程师%';   -- 包含"工程师"
```

- `%` 匹配任意个字符；`_` 匹配单个字符。
- 前缀匹配 `LIKE 'abc%'` 能走索引；**`LIKE '%abc'` 不走索引。**

### IS NULL / IS NOT NULL

```sql
SELECT * FROM users WHERE deleted_at IS NULL;       -- 未删除的
SELECT * FROM users WHERE deleted_at IS NOT NULL;   -- 已删除的
```

**`= NULL` 永远为假，必须用 `IS NULL`。**

---

## 2. 聚合函数

```sql
SELECT COUNT(*)          FROM orders;   -- 总行数（含 NULL）
SELECT COUNT(phone)      FROM users;    -- phone 非 NULL 行数
SELECT COUNT(DISTINCT city) FROM users; -- 去重计数
SELECT SUM(amount)       FROM orders;   -- 总和
SELECT AVG(score)        FROM exams;    -- 平均值
SELECT MAX(price), MIN(price) FROM products; -- 最大/最小值
```

- 聚合函数括号内是列名时忽略 NULL；`COUNT(*)` 不忽略。
- 聚合函数不能出现在 WHERE 中（用 HAVING）。

---

## 3. GROUP BY 与 HAVING

```sql
-- 每个部门的员工数
SELECT dept_id, COUNT(*) AS cnt
FROM employees
GROUP BY dept_id;

-- 订单总额 > 1000 的用户
SELECT user_id, SUM(amount) AS total
FROM orders
GROUP BY user_id
HAVING SUM(amount) > 1000;
```

**执行顺序：** WHERE → GROUP BY → 聚合 → HAVING → ORDER BY → LIMIT

- WHERE 在分组前过滤行；HAVING 在分组后过滤组。
- SELECT 中未聚合的列必须出现在 GROUP BY 中。
- WHERE 能过滤的不要放到 HAVING，WHERE 效率更高。

---

## 4. ORDER BY 与 LIMIT

```sql
-- 降序排列，取前10
SELECT * FROM products ORDER BY sales DESC LIMIT 10;

-- 升序排列，取前5
SELECT * FROM users ORDER BY created_at ASC LIMIT 5;

-- 按多列排序：先按部门升序，部门内按工资降序
SELECT * FROM employees ORDER BY dept_id ASC, salary DESC;

-- 分页：跳过5条，取10条
SELECT * FROM articles ORDER BY id LIMIT 10 OFFSET 5;
-- 等价于
SELECT * FROM articles ORDER BY id LIMIT 5, 10;   -- LIMIT offset, count
```

**大表分页优化：** 避免大 OFFSET。

```sql
-- 差：OFFSET 太大需扫描大量无用行
SELECT * FROM articles ORDER BY id LIMIT 100000, 10;

-- 好：用上次的 id 定位
SELECT * FROM articles WHERE id > 100000 ORDER BY id LIMIT 10;
```

---

## 一条 SELECT 的完整执行顺序

```
FROM → WHERE → GROUP BY → HAVING → SELECT → ORDER BY → LIMIT
```

实际引擎执行顺序不同，但理解这个逻辑顺序足以写对 SQL。
