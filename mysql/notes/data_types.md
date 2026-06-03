# MySQL 常见数据类型

## 整型

| 类型 | 字节 | 有符号范围 |
|------|------|-----------|
| TINYINT | 1 | -128 ~ 127 |
| SMALLINT | 2 | -32768 ~ 32767 |
| MEDIUMINT | 3 | ±838万 |
| INT / INTEGER | 4 | ±21亿 |
| BIGINT | 8 | ±2^63 |

- `INT UNSIGNED` 主键自增最常用，范围 0 ~ 42亿。
- `INT(n)` 的 n 仅为显示宽度（配合 ZEROFILL），不代表存储长度。

## 定点数（精确）

内部去掉小数点，使用十进制整数存的。

Q：为什么`DECIMAL(10, 2)`最大值是99999999.99
A：整数部分最大值是10个9 加两个小数点。

```sql
price DECIMAL(10, 2)   -- 总10位，小数2位
```

- **金额必须用 DECIMAL**，禁止用 FLOAT/DOUBLE。

## 浮点数（近似）

浮点数有两个，单精度和双精度 Float，4字节，Double，8字节。都是IEE754.

```sql
score DOUBLE   -- 科学计算、统计场景
```

## 字符串

```sql
code    CHAR(6)        -- 定长，值等长时用（如身份证号、国家代码）
name    VARCHAR(100)   -- 变长，绝大多数文本字段的首选
content TEXT           -- 长文本，不能有默认值，索引需指定前缀
```

| 类型 | 最大长度 | 适用 |
|------|---------|------|
| CHAR | 255 字符 | 固定长度编码 |
| VARCHAR | 65535 字节 | 短字段：姓名、邮箱、标题 |
| TEXT | 65535 字节 | 文章、描述、评论 |
| MEDIUMTEXT | 16MB | 长文档 |
| LONGTEXT | 4GB | 超长文本 |

## 日期时间

```sql
create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP                          -- 1970-2038，时区感知
update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
birthday    DATE                                                         -- 'YYYY-MM-DD'
order_time  DATETIME                                                     -- 'YYYY-MM-DD HH:MM:SS'，1000-9999
```

- 记录行创建/更新时间用 **TIMESTAMP**。
- 业务日期（如生日、预约时间）用 **DATETIME**，无时区干扰。

`update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP` 插入时默认是当前时间戳，当行数据更新时，自动更新成当前时间戳。

`ON UPDATE` 更新时触发

### TIMESTAMP

时间戳

TIMESTAMP = 1970-01-01 00:00:00 UTC 到现在的秒数（存储）+ 自动时区转换（行为）

存储：4字节整数（秒数）
显示：根据时区自动转换成日期时间

Q：插入2038年之后的报错
A：
-- 原因：存储秒数的4字节有符号整数最大是 2147483647
-- 对应：2038-01-19 03:14:07 UTC

只有在需要时区转换系统场景用
节省空间的常见4B vs 8B

其他场景直接使用DATETIME 年月日时分秒
#### 存储原理

```
-- 当你插入 '2024-01-15 10:00:00' 时：

-- 步骤1：MySQL 认为你输入的是"当前时区的时间"
假设当前时区 = '+08:00'（北京时间）

-- 步骤2：转成 UTC 时间
UTC时间 = 2024-01-15 10:00:00 - 8小时 = 2024-01-15 02:00:00 UTC

-- 步骤3：转成 Unix 时间戳（秒数）
Unix时间戳 = 从1970-01-01 00:00:00 UTC 到 2024-01-15 02:00:00 UTC 的秒数
= 1705298400 秒

-- 步骤4：存储这个整数
实际存的是：1705298400（4字节整数）

-- 读取时反过来：
1705298400 → UTC时间 2024-01-15 02:00:00 → 当前时区 2024-01-15 10:00:00
```
```sql
-- 建表
CREATE TABLE test_time (
    dt DATETIME,
    ts TIMESTAMP
);

-- 设置时区为北京时间
SET time_zone = '+08:00';
INSERT INTO test_time VALUES ('2024-01-15 10:00:00', '2024-01-15 10:00:00');

-- 查看（北京时间）
SELECT * FROM test_time;
-- dt: 2024-01-15 10:00:00（原样）
-- ts: 2024-01-15 10:00:00（显示为北京时间）

-- 改成美国时区
SET time_zone = '-05:00';
SELECT * FROM test_time;
-- dt: 2024-01-15 10:00:00（不变！）
-- ts: 2024-01-14 21:00:00（自动减了13小时！）

-- 查看存储的原始秒数
SELECT UNIX_TIMESTAMP(ts) FROM test_time;  -- 1705298400（固定值）
```
## ENUM

```sql
status ENUM('pending', 'approved', 'rejected')   -- 单选，内部按整数存储
```

## JSON

```sql
extra JSON   -- 半结构化扩展字段
-- 查询：SELECT extra->>'$.key' FROM t;
```

## 速查

| 场景 | 类型 |
|------|------|
| 主键/自增ID | INT/BIGINT UNSIGNED |
| 金额 | DECIMAL |
| 短文本（名、邮箱） | VARCHAR |
| 长文本（正文、描述） | TEXT |
| 创建/更新时间 | TIMESTAMP |
| 布尔标记 | TINYINT(1) |
| 定长编码 | CHAR |
| 动态扩展字段 | JSON |
