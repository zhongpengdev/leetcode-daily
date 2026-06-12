# 存储过程

存储过程是预编译好的一段 SQL，存在数据库里，按需调用。

---

## 1. 创建与调用

```sql
-- 创建
DELIMITER //   -- 临时改分隔符，因为过程体内有分号
CREATE PROCEDURE get_users_by_city(IN city_name VARCHAR(50))
BEGIN
    SELECT * FROM users WHERE city = city_name;
END //
DELIMITER ;    -- 恢复分号

-- 调用
CALL get_users_by_city('北京');

-- 删除
DROP PROCEDURE IF EXISTS get_users_by_city;
```

**DELIMITER** 的作用：默认 `;` 结束一条语句，但存储过程体内有多条 `;`，如果不改分隔符，MySQL 会在第一个 `;` 处就结束整个 CREATE PROCEDURE，语法就断了。

---

## 2. 参数：IN、OUT、INOUT

```sql
-- IN：传入，只读（默认就是这个，可以省略 IN）
CREATE PROCEDURE calc_total(IN uid INT, IN status VARCHAR(20))
BEGIN
    SELECT SUM(amount) FROM orders WHERE user_id = uid AND status = status;
END //

-- OUT：传出
CREATE PROCEDURE get_user_count(OUT total INT)
BEGIN
    SELECT COUNT(*) INTO total FROM users;
END //

CALL get_user_count(@cnt);   -- @cnt 是会话变量
SELECT @cnt;                 -- 拿到结果

-- INOUT：传入并传出
CREATE PROCEDURE double_it(INOUT num INT)
BEGIN
    SET num = num * 2;
END //

SET @val = 10;
CALL double_it(@val);
SELECT @val;   -- 20
```

---

## 3. 变量

```sql
CREATE PROCEDURE demo_variables()
BEGIN
    -- DECLARE 声明局部变量（必须在 BEGIN 后最前面）
    DECLARE done INT DEFAULT 0;
    DECLARE user_name VARCHAR(50);
    DECLARE total_salary DECIMAL(10,2) DEFAULT 0.00;

    -- SET 赋值
    SET user_name = 'Alice';
    SET total_salary = total_salary + 5000;

    -- SELECT INTO 赋值
    SELECT name INTO user_name FROM users WHERE id = 1;
    SELECT COUNT(*) INTO done FROM orders;

    SELECT user_name, total_salary, done;
END //
```

| 变量类型 | 写法 | 作用域 |
|---------|------|--------|
| 局部变量 | `DECLARE name INT DEFAULT 0;` | BEGIN...END 内 |
| 会话变量 | `@name` | 当前连接 |
| 系统变量 | `@@var_name` | 全局/会话级 |

---

## 4. 条件判断

### IF...ELSEIF...ELSE

```sql
CREATE PROCEDURE check_age(IN age INT, OUT level VARCHAR(20))
BEGIN
    IF age < 18 THEN
        SET level = '未成年';
    ELSEIF age < 60 THEN
        SET level = '成年人';
    ELSE
        SET level = '老年人';
    END IF;
END //
```

### CASE

```sql
CREATE PROCEDURE get_status_name(IN code INT, OUT name VARCHAR(20))
BEGIN
    CASE code
        WHEN 0 THEN SET name = '待审核';
        WHEN 1 THEN SET name = '已通过';
        WHEN 2 THEN SET name = '已拒绝';
        ELSE SET name = '未知';
    END CASE;
END //
```

---

## 5. 循环

### WHILE

```sql
-- 插入 100 条测试数据
CREATE PROCEDURE seed_data(IN cnt INT)
BEGIN
    DECLARE i INT DEFAULT 1;
    WHILE i <= cnt DO
        INSERT INTO users (name) VALUES (CONCAT('user_', i));
        SET i = i + 1;
    END WHILE;
END //
```

### REPEAT（至少执行一次）

```sql
CREATE PROCEDURE demo_repeat()
BEGIN
    DECLARE i INT DEFAULT 1;
    REPEAT
        INSERT INTO log (msg) VALUES (CONCAT('repeat_', i));
        SET i = i + 1;
    UNTIL i > 5 END REPEAT;
END //
```

### LOOP + LEAVE（无限循环，手动跳出）

```sql
CREATE PROCEDURE demo_loop()
BEGIN
    DECLARE i INT DEFAULT 0;
    my_loop: LOOP
        SET i = i + 1;
        IF i > 10 THEN
            LEAVE my_loop;   -- 跳出
        END IF;
        INSERT INTO log (msg) VALUES (CONCAT('loop_', i));
        ITERATE my_loop;     -- 跳到下一次循环（相当于 continue）
    END LOOP;
END //
```

| 循环 | 特点 |
|------|------|
| WHILE | 先判断后执行，最常用 |
| REPEAT | 先执行后判断，至少跑一次 |
| LOOP | 不判断，靠 LEAVE 跳出 |

---

## 6. 游标（处理结果集逐行）

```sql
CREATE PROCEDURE process_orders()
BEGIN
    DECLARE done INT DEFAULT 0;
    DECLARE order_id INT;
    DECLARE order_amount DECIMAL(10,2);
    -- 声明游标
    DECLARE cur CURSOR FOR SELECT id, amount FROM orders WHERE status = 'pending';
    -- 游标走完后设置 done = 1
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;

    OPEN cur;
    read_loop: LOOP
        FETCH cur INTO order_id, order_amount;
        IF done = 1 THEN
            LEAVE read_loop;
        END IF;
        -- 逐行处理
        UPDATE orders SET status = 'processed' WHERE id = order_id;
    END LOOP;
    CLOSE cur;
END //
```

---

## 7. 异常处理

```sql
CREATE PROCEDURE safe_insert(IN user_name VARCHAR(50), IN user_email VARCHAR(100))
BEGIN
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        SELECT '插入失败' AS result;
        ROLLBACK;
    END;

    START TRANSACTION;
    INSERT INTO users (name, email) VALUES (user_name, user_email);
    INSERT INTO user_stats (user_id, created_at) VALUES (LAST_INSERT_ID(), NOW());
    COMMIT;
    SELECT 'OK' AS result;
END //
```

---

## 8. 工程中的利弊

| 利 | 弊 |
|----|-----|
| 减少网络传输（多次 SQL 变一次 CALL） | 调试困难，没有断点，看不到中间值 |
| 预编译，首次执行后缓存执行计划 | 版本管理难，不像代码有 git diff |
| 封装业务逻辑，减少应用层代码 | 数据库成为性能瓶颈时不好拆 |
| 权限控制：只给 EXECUTE 权限 | 不同数据库语法不兼容，迁移代价高 |

**现状：** 大厂几乎不写存储过程，逻辑放应用层。传统银行/ERP 系统仍然大量使用。学习它主要为了读老项目代码和维护遗留系统。

---

## 9. 函数 vs 存储过程

| | 存储过程 | 函数 |
|------|---------|------|
| 返回值 | 可以有多个 OUT / 不返回 | 必须返回一个值 |
| 调用方式 | `CALL proc(...)` | `SELECT func(...)` |
| 事务 | 可以使用 COMMIT/ROLLBACK | 不能 |
| 用途 | 批量操作、数据处理 | 计算、转换，嵌入 SQL 中 |

```sql
-- 函数示例
CREATE FUNCTION get_user_name(uid INT) RETURNS VARCHAR(50)
    DETERMINISTIC   -- 相同输入恒定输出，允许用于索引
    READS SQL DATA  -- 只读
BEGIN
    DECLARE name VARCHAR(50);
    SELECT name INTO name FROM users WHERE id = uid;
    RETURN name;
END //

SELECT get_user_name(1);
```
