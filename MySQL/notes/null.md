**NULL 不是值，而是一个"未知的标记"**。

---

## 一、NULL 到底是什么？

**NULL ≠ 0**  
**NULL ≠ 空字符串 ''**  
**NULL ≠ false**

NULL 的意思是：**"我不知道"** 或 **"没有值"**。

想象一个表格：
| 姓名 | 年龄 | 手机号 |
|------|------|--------|
| 张三 | 25 | 138xxx |
| 李四 | NULL | (没填) |

"李四的年龄"不是 0 岁，也不是 -1 岁，而是**我不知道他几岁**。这就是 NULL。

---

## 二、NULL 的第一个大坑：任何运算结果都是 NULL

```sql
SELECT NULL + 1;   -- 结果：NULL
SELECT NULL * 100; -- 结果：NULL
SELECT NULL = 0;   -- 结果：NULL（不是 true，也不是 false）
```

**这意味着**：如果你有个字段是 NULL，对它做任何计算，结果都是 NULL。

```sql
-- 假设 salary 列有 NULL
SELECT salary + 1000 FROM employees;
-- 原来 salary 是 NULL 的行，结果是 NULL，不是 1000
```

---

## 三、第二个大坑：比较操作也返回 NULL

```sql
SELECT NULL = NULL;   -- 结果：NULL（不是 true！）
SELECT NULL != NULL;  -- 结果：NULL
SELECT NULL > 10;     -- 结果：NULL
```

**核心理解**：两个"未知"比较，结果还是"未知"。

---

## 四、第三个大坑：WHERE 条件只认 true

WHERE 子句里，只有结果为 **true** 的行会被选中。  
**NULL 和 false 都会被过滤掉**。

```sql
-- 这张表
id | name   | age
1  | 张三   | 25
2  | 李四   | NULL

-- 这条查不出李四！
SELECT * FROM users WHERE age = NULL;   -- 结果为空

-- 这条也查不出李四！
SELECT * FROM users WHERE age != NULL;  -- 结果为空

-- 正确写法
SELECT * FROM users WHERE age IS NULL;   -- 查到李四
```

**这就是最经典的 NULL 坑**：用 `= NULL` 查不到东西，必须用 `IS NULL`。

---

## 五、第四个坑：NOT IN 遇到 NULL 会"炸"

这是生产环境出过无数次的严重 bug。

```sql
-- 表 A：所有用户
id | name
1  | 张三
2  | 李四
3  | 王五

-- 表 B：黑名单用户
user_id
1
NULL

-- 想找"不在黑名单里的用户"
SELECT * FROM users WHERE id NOT IN (SELECT user_id FROM blacklist);
```

**你猜结果是什么？**

结果是：**空集，什么都查不到！**

**原因**：
- `NOT IN (1, NULL)` 等价于 `id != 1 AND id != NULL`
- `id != NULL` 的结果是 NULL
- `id != 1 AND NULL` = NULL（因为 AND 只要有一个未知，整体未知）
- WHERE 只认 true，NULL 被过滤掉

**正确做法**：
```sql
SELECT * FROM users WHERE id NOT IN (
    SELECT user_id FROM blacklist WHERE user_id IS NOT NULL
);
```

---

## 六、第五个坑：COUNT 和 NULL

```sql
SELECT COUNT(*) FROM users;     -- 统计所有行，NULL 算
SELECT COUNT(age) FROM users;   -- 统计 age 非 NULL 的行数
SELECT COUNT(1) FROM users;     -- 同 COUNT(*)
SELECT COUNT(NULL);             -- 结果是 0
```

| 函数 | 是否统计 NULL |
|------|--------------|
| `COUNT(*)` | ✅ 统计 |
| `COUNT(列)` | ❌ 不统计 NULL |
| `COUNT(1)` | ✅ 统计 |
| `COUNT(NULL)` | 结果是 0 |

---

## 七、第六个坑：UNIQUE 约束允许多个 NULL

```sql
CREATE TABLE t (email VARCHAR(50) UNIQUE);

INSERT INTO t (email) VALUES (NULL);  -- ✅ 成功
INSERT INTO t (email) VALUES (NULL);  -- ✅ 还是成功！两个 NULL
```

**理解**：NULL 不是值，所以不参与唯一性比较。两个 NULL 不视为重复。

---

## 八、第七个坑：排序时 NULL 的位置

```sql
SELECT * FROM users ORDER BY age ASC;
-- NULL 默认排在最前面（升序时）
```

| 排序方向 | NULL 的位置 |
|----------|------------|
| `ASC` | 最前面 |
| `DESC` | 最后面 |

```sql
-- 如果想改 NULL 的位置
SELECT * FROM users ORDER BY age ASC NULLS LAST;  -- MySQL 8.0 支持
```

---

## 九、NULL 的专用操作符

| 操作符 | 作用 |
|--------|------|
| `IS NULL` | 判断是否为 NULL |
| `IS NOT NULL` | 判断是否不是 NULL |
| `<=>` | NULL 安全的等于（NULL <=> NULL 返回 true）|

```sql
SELECT NULL <=> NULL;   -- 1（true）
SELECT 1 <=> NULL;      -- 0（false）
```

---

## 十、实战规则总结

| 场景 | 正确做法 | 错误做法 |
|------|---------|---------|
| 判断是否为空 | `WHERE col IS NULL` | `WHERE col = NULL` |
| 判断是否不为空 | `WHERE col IS NOT NULL` | `WHERE col != NULL` |
| 设置默认值 | `COALESCE(col, 0)` | `col + 0`（有 NULL 结果变 NULL）|
| NOT IN 子查询 | 子查询里过滤 NULL | 不处理 NULL |
| 字符串拼接 | `CONCAT(col1, col2)` 有 NULL 结果变 NULL | 直接用 `+` |
| 唯一索引 | 知道允许多个 NULL | 以为会像普通值一样唯一 |

---

## 十一、设计建议（防止被坑）

1. **能用 NOT NULL 就用 NOT NULL**  
   列设成 `NOT NULL` + 默认值，可以避免 80% 的 NULL 坑。

2. **区分"业务上的空"和"未知"**
   - 空字符串 `''`：表示"知道是空"，如"中间名没有"
   - NULL：表示"不知道"

3. **索引列尽量 NOT NULL**  
   NULL 列上建索引，某些查询无法优化。

---

## 十二、一句话总结

> **NULL = "未知"，不是值。任何与 NULL 的运算、比较都返回 NULL。WHERE 只认 true，NULL 会被过滤。判断 NULL 必须用 `IS NULL`，不能用 `= NULL`。**