# MySQL是自带锁的

单个SQL语句天然是原子的，默认有写锁或排他锁，直接DELETE或者UPDATE是安全的

```sql
UPDATE account SET balance = balance - 100 WHERE id = 1
```

但是`SELECT`默认不加锁，先SELECT再根据其结果做操作是不安全的：

```sql
SELECT balance FROM account WHERE id = 1 
```

```sql
// 危险：先 SELECT（无锁），后 UPDATE（有锁）
BigDecimal balance = accountMapper.selectBalance(userId);  // 无锁读
// 这里可能被其他线程修改
accountMapper.updateBalance(userId, balance.subtract(amount)); // 基于过期数据更新
```

## 安全的sql

```sql
// ✅ 模式1：纯更新，不依赖查询
UPDATE account SET balance = balance - 100 WHERE id = 1

// ✅ 模式2：带条件更新（CAS 风格）
UPDATE account SET balance = 900 WHERE id = 1 AND balance = 1000

// ✅ 模式3：插入/删除
INSERT INTO log ... / DELETE FROM ...

// ✅ 模式4：只查询（不改）
SELECT * FROM user WHERE id = 1
```

## 危险的

1. 读写分离
2. 先检查后更新
3. 竞态条件但不加锁

```java
// ❌ 模式1：读-改-写分离
// 可能多个线程全都读到1000元，都在1000元的基础上扣钱
User user = mapper.select(id);  // 读
user.setBalance(user.getBalance() - 100);  // 改
mapper.update(user);  // 写（基于可能过期的数据）

// 安全写法
UPDATE user SET balance = balance - 100 WHERE id = 1

// 安全写法，乐观锁，带调教更新
// ✅ 安全：带条件更新（乐观锁）
User user = mapper.select(id);
user.setBalance(user.getBalance() - 100);
mapper.updateWithVersion(user);  // update ... where id=? and balance=旧值

// ❌ 模式2：先检查后执行
if (accountMapper.selectBalance(id) >= 100) {  // 读
    // 这里可能被其他线程扣到不足100
    accountMapper.updateBalance(id, balance - 100);  // 写
}

// ❌ 模式3：累加/计数（非原子）
visitCount = visitCount + 1;  // 纯 Java 代码，没走 SQL
```
