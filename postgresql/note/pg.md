# 安装

```bash
sudo apt update
sudo apt install postgresql postgresql-contrib

# 安装成功
sudo systemctl status postgresql

# 切换到postgres用户
sudo -i -u postgres

# 进入命令
psql

# 设置密码
\password postgres

# 退出
\q

```

# psycopg2 基础操作

## 1. 连接数据库

```python
import psycopg2

conn = psycopg2.connect(
    host="localhost",
    port=5432,
    dbname="testdb",
    user="postgres",
    password="your_password"
)

cur = conn.cursor()  # 创建游标，所有操作通过它执行

# 用完必须关闭
cur.close()
conn.close()
```

**推荐用 `with` 自动管理：**

```python
with psycopg2.connect(...) as conn:
    with conn.cursor() as cur:
        cur.execute("SELECT 1")
```

`with conn` 结束时自动 commit 或 rollback（取决于是否有异常）。

---

## 2. 建表

```python
cur.execute("""
    CREATE TABLE IF NOT EXISTS users (
        id SERIAL PRIMARY KEY,
        name VARCHAR(50) NOT NULL,
        age INT,
        created_at TIMESTAMP DEFAULT NOW()
    )
""")
conn.commit()  # DDL 和写操作必须 commit
```

---

## 3. 增 INSERT

```python
# 单条
cur.execute(
    "INSERT INTO users (name, age) VALUES (%s, %s)",
    ("Alice", 25)
)

# 多条批量插入
data = [("Bob", 30), ("Charlie", 22), ("Diana", 28)]
cur.executemany(
    "INSERT INTO users (name, age) VALUES (%s, %s)",
    data
)

# 插入并返回自增 id
cur.execute(
    "INSERT INTO users (name, age) VALUES (%s, %s) RETURNING id",
    ("Eve", 35)
)
new_id = cur.fetchone()[0]  # 拿到新插入的 id
print(f"插入的 id: {new_id}")

conn.commit()
```

**关键：用 `%s` 占位符，不要用 f-string 或字符串拼接（防 SQL 注入）。**

---

## 4. 查 SELECT

```python
cur.execute("SELECT id, name, age FROM users WHERE age > %s", (20,))

# 三种取结果的方式：

row = cur.fetchone()       # 取一条 → 元组 (1, 'Alice', 25) 或 None
rows = cur.fetchmany(3)    # 取 3 条 → 列表 [(...), (...), (...)]
rows = cur.fetchall()      # 取全部 → 列表

# 遍历结果
cur.execute("SELECT id, name, age FROM users")
for row in cur:
    print(row)  # (1, 'Alice', 25)
    # 或用下标：row[0], row[1], row[2]
```

**用字典方式拿结果（更直观）：**

```python
from psycopg2.extras import RealDictCursor

with conn.cursor(cursor_factory=RealDictCursor) as cur:
    cur.execute("SELECT * FROM users WHERE id = %s", (1,))
    row = cur.fetchone()
    print(row)          # {'id': 1, 'name': 'Alice', 'age': 25}
    print(row['name'])  # Alice，用字段名取值
```

---

## 5. 改 UPDATE

```python
cur.execute(
    "UPDATE users SET age = %s WHERE name = %s",
    (26, "Alice")
)
print(f"影响了 {cur.rowcount} 行")  # rowcount 返回受影响行数
conn.commit()
```

---

## 6. 删 DELETE

```python
cur.execute(
    "DELETE FROM users WHERE name = %s",
    ("Bob",)
)
print(f"删除了 {cur.rowcount} 行")
conn.commit()
```

---

## 7. 事务

psycopg2 默认在事务中，**每次 execute 后必须 commit 才生效**。

```python
try:
    cur.execute("UPDATE users SET age = age + 1 WHERE name = %s", ("Alice",))
    cur.execute("UPDATE users SET age = age - 1 WHERE name = %s", ("Bob",))
    conn.commit()  # 两条都成功才提交
except Exception as e:
    conn.rollback()  # 出错全部回滚
    print(f"事务失败: {e}")
```

**关闭自动提交，手动控制事务：**

```python
conn.autocommit = False  # 默认就是 False

# 设为 True 后，每条 execute 立即生效，不需要 commit
conn.autocommit = True
cur.execute("CREATE DATABASE testdb2")  # 建库必须在 autocommit 模式下
```

---

## 8. 参数化查询防注入

```python
# 危险 — 绝对不要这样写
name = "'; DROP TABLE users; --"
cur.execute(f"SELECT * FROM users WHERE name = '{name}'")  # SQL注入！

# 安全 — 用 %s 占位符
cur.execute("SELECT * FROM users WHERE name = %s", (name,))  # 参数是元组
```

**规则：SQL 中永远用 `%s`，数据通过第二个参数传入。**

---

## 速查表

| 操作 | 方法 | 需要 commit？ |
|:----|:-----|:------------|
| 建表 | `cur.execute("CREATE TABLE ...")` | 是 |
| 插入 | `cur.execute("INSERT ...", (vals,))` | 是 |
| 查询 | `cur.execute("SELECT ...", (vals,))` | 否 |
| 更新 | `cur.execute("UPDATE ...", (vals,))` | 是 |
| 删除 | `cur.execute("DELETE ...", (vals,))` | 是 |
| 批量插入 | `cur.executemany("INSERT ...", data)` | 是 |
