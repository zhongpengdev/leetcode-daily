# 约束（Constraints）

约束是加在列上的**数据规则**，由数据库引擎强制校验。违反约束的 INSERT/UPDATE 直接报错。

---

## 1. PRIMARY KEY（主键）

**一行数据的唯一身份证。**

```sql
-- 单列主键
CREATE TABLE users (
    id INT UNSIGNED PRIMARY KEY,
    name VARCHAR(50)
);

-- 自增主键（最常用）
CREATE TABLE users (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50)
);

-- 复合主键
CREATE TABLE order_items (
    order_id INT UNSIGNED,
    product_id INT UNSIGNED,
    quantity INT,
    PRIMARY KEY (order_id, product_id)
);
```

**规则：** ≠ NULL + 唯一 + 一张表只能有一个主键。
**工程惯例：** 每张表必须有主键，优先用自增 BIGINT UNSIGNED。

---

## 2. FOREIGN KEY（外键）

**保证两张表之间引用关系正确。**

```sql
CREATE TABLE orders (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    user_id INT UNSIGNED NOT NULL,
    amount DECIMAL(10,2),
    FOREIGN KEY (user_id) REFERENCES users(id)
        ON DELETE CASCADE      -- 用户被删，订单跟着删
        ON UPDATE CASCADE
);
```

### 级联策略

| 策略 | 行为 |
|------|------|
| CASCADE | 父表删/改，子表跟着删/改 |
| SET NULL | 父表删，子表外键列置 NULL（列须允许 NULL） |
| RESTRICT / NO ACTION | 有子记录则禁止删父记录 |
| SET DEFAULT | MySQL 不生效（InnoDB 忽略） |

### 利弊

| 利 | 弊 |
|----|-----|
| 保证引用完整性，不怕脏数据 | 写入时需校验引用，影响性能 |
| 级联删除简洁省心 | 级联删除可能误删大量数据 |
| 自文档化，表关系一目了然 | 分库分表、数据迁移时是负担 |

### 工程决策

**大厂核心业务大多不用外键**，由应用层保证一致性，换来性能与分库分表自由。初创/内部后台/数据一致性要求极高的场景（金融）建议用。

```sql
-- 不上外键时自己查
-- 删用户前确认无关联订单
DELETE FROM users WHERE id = 1 AND NOT EXISTS (
    SELECT 1 FROM orders WHERE user_id = 1
);
```

---

## 3. UNIQUE（唯一约束）

**列的值不能重复，但允许多个 NULL。**

```sql
CREATE TABLE users (
    id INT UNSIGNED PRIMARY KEY,
    email VARCHAR(100) UNIQUE,      -- 列级
    phone VARCHAR(20),
    UNIQUE (phone)                   -- 表级
);

-- 联合唯一
UNIQUE (user_id, role_id)  -- 同一用户不能重复分配同一角色
```

**本质：** UNIQUE 会创建一个唯一索引，查询自动受益。
**与主键区别：** 主键 ≠ NULL + 唯一 + 每表一个；UNIQUE 允许 NULL + 可以有多个。

---

## 4. NOT NULL（非空约束）

**列不允许存 NULL。**

```sql
CREATE TABLE users (
    id INT UNSIGNED PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    email VARCHAR(100) NOT NULL DEFAULT ''
);
```

**为什么避免 NULL：**
- `NULL` ≠ `NULL`（NULL 不等于任何值，包括自己），`WHERE col = NULL` 永远为空。
- 聚合函数忽略 NULL 行为往往不是预期。
- 索引中 NULL 占用额外标记。
- 应用层需要 `nil`/`null` 判断，增加心智负担。

**工程惯例：** 列尽量 NOT NULL，给 DEFAULT 兜底。

---

## 5. DEFAULT（默认值）

**INSERT 没给的列自动填入。**

```sql
CREATE TABLE users (
    status     TINYINT DEFAULT 0,              -- 数字
    role       VARCHAR(20) DEFAULT 'user',      -- 字符串
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 函数
    active     TINYINT(1) DEFAULT 1
);
```

**用途：** 简化 INSERT、配合 NOT NULL 消灭 NULL。

---

## 6. CHECK（检查约束）

**自定义条件校验。** MySQL 8.0.16+ 才真正支持。

```sql
CREATE TABLE employees (
    id INT PRIMARY KEY,
    age TINYINT CHECK (age >= 18 AND age <= 65),
    salary DECIMAL(10,2) CHECK (salary > 0),
    start_date DATE,
    end_date DATE,
    CHECK (end_date > start_date)   -- 表级 CHECK
);
```

**历史包袱：** 老版本 MySQL 语法不报错但**不生效**，约束被静默忽略。使用前确认版本 ≥ 8.0.16。

**替代（老版本）：** 触发器或应用层校验。

---

## 建表时与建表后

```sql
-- 建表后加约束
ALTER TABLE users ADD PRIMARY KEY (id);
ALTER TABLE users ADD UNIQUE (email);
ALTER TABLE users ADD FOREIGN KEY (role_id) REFERENCES roles(id);
ALTER TABLE users MODIFY name VARCHAR(50) NOT NULL;
ALTER TABLE users ALTER COLUMN status SET DEFAULT 0;

-- 删约束
ALTER TABLE users DROP PRIMARY KEY;
ALTER TABLE users DROP INDEX email;          -- UNIQUE 按索引删
ALTER TABLE users DROP FOREIGN KEY fk_name;  -- 删外键需要约束名
ALTER TABLE users MODIFY name VARCHAR(50) NULL;
ALTER TABLE users ALTER COLUMN status DROP DEFAULT;
```

---

## 速查

| 约束 | 作用 | 允许多个NULL | 每表数量 |
|------|------|-------------|---------|
| PRIMARY KEY | 唯一标识一行 | 否 | 1 |
| FOREIGN KEY | 引用另一张表 | 可 | 多个 |
| UNIQUE | 列值不重复 | 可 | 多个 |
| NOT NULL | 列不能为空 | — | 每列 |
| DEFAULT | 默认值 | — | 每列 |
| CHECK | 自定义条件 | — | 每列 / 表级 |
