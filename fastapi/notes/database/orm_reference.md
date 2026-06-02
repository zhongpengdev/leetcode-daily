# SQLAlchemy ORM 映射语法详解

## 一、字段类型映射

---

### 数字类型

| SQLAlchemy | 对应 MySQL | 对应 PostgreSQL | 说明 |
|---|---|---|---|
| `Integer` | `INT` | `INTEGER` | 整数（-21亿 ～ +21亿） |
| `BigInteger` | `BIGINT` | `BIGINT` | 大整数，id 超 21 亿时用 |
| `SmallInteger` | `SMALLINT` | `SMALLINT` | 小整数（-32768 ～ 32767） |
| `Float` | `FLOAT` | `FLOAT` | 浮点数，有精度误差 |
| `Numeric(10,2)` | `DECIMAL(10,2)` | `NUMERIC(10,2)` | 精确小数，金额必须用这个 |
| `Boolean` | `TINYINT(1)` | `BOOLEAN` | 布尔值，存 0 或 1 |

**金额用 `Numeric` 不用 `Float`：**

```
Float:   0.1 + 0.2 = 0.30000000000000004  ← 精度误差
Numeric: 0.1 + 0.2 = 0.30                  ← 精确
```

---

### 字符串类型

| SQLAlchemy | 对应 MySQL | 说明 |
|---|---|---|
| `String(255)` | `VARCHAR(255)` | 变长字符串，必须指定长度 |
| `Text` | `TEXT` | 不限长度，存大段文章 |
| `Unicode(255)` | `VARCHAR(255)` | Unicode 字符串 |
| `UnicodeText` | `TEXT` | 不限长度 Unicode |

**什么时候用 `String` 什么时候用 `Text`：**

```
String(100) → 用户名、邮箱、手机号、标题（有限长度）
Text        → 文章内容、商品描述、评论（长度不确定）
```

---

### 时间类型

| SQLAlchemy | 对应 MySQL | 说明 |
|---|---|---|
| `DateTime` | `DATETIME` | 年月日时分秒 |
| `Date` | `DATE` | 仅年月日 |
| `Time` | `TIME` | 仅时分秒 |
| `Interval` | 无 | 时间间隔 |

**`DateTime` 最常用的两个配置：**

```python
created_at = Column(DateTime, server_default=func.now())
# 创建时自动填当前时间，只填一次

updated_at = Column(DateTime, server_default=func.now(), onupdate=func.now())
# 创建时填，每次更新自动重填
```

---

### 其他类型

| SQLAlchemy | 对应 MySQL | 说明 |
|---|---|---|
| `Enum("a","b")` | `ENUM("a","b")` | 枚举，只能取固定几个值 |
| `JSON` | `JSON` | 存 JSON 格式数据（MySQL 5.7+） |
| `PickleType` | `BLOB` | 存 Python 对象（自动 pickle 序列化） |

---

## 二、字段选项

---

### 约束选项

| 选项 | 作用 |
|---|---|
| `primary_key=True` | 主键，唯一标识一条记录 |
| `autoincrement=True` | 自动递增，id 专用 |
| `nullable=False` | 不能为空 |
| `nullable=True` | 可以为空（默认） |
| `unique=True` | 值不能重复 |
| `default=值` | Python 端默认值 |
| `server_default=表达式` | MySQL 端默认值 |
| `index=True` | 建索引，查得快 |
| `comment="备注"` | 字段注释 |

---

### 常用组合

```python
# 自增主键
id = Column(Integer, primary_key=True, autoincrement=True)

# 唯一索引字段
email = Column(String(255), unique=True, index=True)

# 必填字段
name = Column(String(100), nullable=False)

# 有默认值
is_active = Column(Boolean, default=True)

# 数据库自动填时间
created_at = Column(DateTime, server_default=func.now())

# 更新时自动重填时间
updated_at = Column(DateTime, server_default=func.now(), onupdate=func.now())

# 金额
price = Column(Numeric(10, 2), nullable=False)

# 大段内容
description = Column(Text, default="")
```

---

## 三、表选项

---

### \_\_tablename\_\_

```python
class User(Base):
    __tablename__ = "users"    # MySQL 里的表名
```

### \_\_table_args\_\_

```python
class User(Base):
    __tablename__ = "users"
    __table_args__ = (
        # 联合唯一：同一用户不能给同一商品重复评分
        UniqueConstraint("user_id", "item_id", name="uk_user_item"),
        # 联合索引
        Index("idx_user_status", "user_id", "status"),
    )
```

---

## 四、关系映射

---

### ForeignKey — 外键

```python
from sqlalchemy import ForeignKey

class Order(Base):
    __tablename__ = "orders"

    id = Column(Integer, primary_key=True)
    user_id = Column(Integer, ForeignKey("users.id"), nullable=False)
    #                                   ↑ "表名.字段名"
```

### relationship — 关联查询

**一对一 / 多对一：**

```python
from sqlalchemy.orm import relationship

class Order(Base):
    __tablename__ = "orders"

    id = Column(Integer, primary_key=True)
    user_id = Column(Integer, ForeignKey("users.id"))
    user = relationship("User", back_populates="orders")
    #     ↑ 通过 order.user 直接拿到 User 对象

class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True)
    orders = relationship("Order", back_populates="user")
    #       ↑ 通过 user.orders 拿到这个用户的所有订单
```

**使用：**

```python
# 不用自己 JOIN，直接 . 过去
order = db.query(Order).first()
print(order.user.name)           # 拿到用户名

user = db.query(User).first()
print(user.orders)               # 拿到该用户所有订单
```

---

## 五、增删改查语法

---

### 增

```python
# 创建对象 → add → commit → refresh
user = User(name="张三", email="a@test.com")
db.add(user)
db.commit()
db.refresh(user)          # 拿到自增 id
```

### 查

```python
# 查全部
db.query(User).all()

# 查一条（按 id）
db.query(User).filter(User.id == 1).first()

# 查一条（按条件，不存在返回 None）
db.query(User).filter(User.email == "a@test.com").first()

# 条件查询
db.query(User).filter(User.name == "张三").all()

# 多条件（AND）
db.query(User).filter(User.name == "张三", User.is_active == True).all()

# 模糊查询
db.query(User).filter(User.name.like("%张%")).all()

# IN 查询
db.query(User).filter(User.id.in_([1, 2, 3])).all()

# 排除
db.query(User).filter(User.name != "张三").all()

# 分页
db.query(User).offset(0).limit(20).all()
#            跳过 0 条 ↑    只取 20 条 ↑

# 排序
db.query(User).order_by(User.id.desc()).all()
db.query(User).order_by(User.created_at.asc()).all()

# 计数
db.query(User).count()

# 取第一条，没有则 None
db.query(User).first()

# 判断是否存在
db.query(User).filter(User.email == "a@test.com").first() is not None
```

### 改

```python
# 方式一：查出来改字段
user = db.query(User).filter(User.id == 1).first()
user.name = "新名字"
db.commit()

# 方式二：批量更新
db.query(User).filter(User.id == 1).update({"name": "新名字"})
db.commit()

# 方式三：自增
db.query(User).filter(User.id == 1).update({User.views: User.views + 1})
db.commit()
```

### 删

```python
user = db.query(User).filter(User.id == 1).first()
db.delete(user)
db.commit()
```

---

## 六、聚合函数

```python
from sqlalchemy import func

# 总数
db.query(func.count(User.id)).scalar()

# 最大值
db.query(func.max(Item.price)).scalar()

# 平均值
db.query(func.avg(Item.price)).scalar()

# 分组统计
db.query(User.role, func.count(User.id)).group_by(User.role).all()
# 返回：[("admin", 3), ("user", 20)]
```

---

## 七、完整表定义模板

```python
# app/models/user.py

from sqlalchemy import Column, Integer, String, Boolean, DateTime, func
from app.core.database import Base


class User(Base):
    __tablename__ = "users"

    # 主键
    id = Column(Integer, primary_key=True, autoincrement=True)

    # 基本信息
    username = Column(String(50), unique=True, nullable=False, index=True)
    email = Column(String(255), unique=True, index=True)
    hashed_password = Column(String(255), nullable=False)
    nickname = Column(String(100), default="")

    # 状态
    is_active = Column(Boolean, default=True)
    role = Column(String(20), default="user")

    # 时间
    created_at = Column(DateTime, server_default=func.now())
    updated_at = Column(DateTime, server_default=func.now(), onupdate=func.now())
```
