# models/ 文件夹详解

## 一句话说清楚

```
models/ 里面的每个文件 = 数据库里的一张表
```

```
app/models/
├── user.py    ← 对应 MySQL 的 users 表
├── item.py    ← 对应 MySQL 的 items 表
└── order.py   ← 对应 MySQL 的 orders 表
```

每个文件里写的就是**这张表有哪些字段、什么类型、有什么约束**。

---

## 一个文件里写什么

以 `user.py` 为例，拆开来一行行解释：

```python
# app/models/user.py

from sqlalchemy import Column, Integer, String, Boolean, DateTime, func

from app.core.database import Base


class User(Base):                    # 必须继承 Base（来自 database.py）
    __tablename__ = "users"          # 告诉 SQLAlchemy 这张表在 MySQL 里叫 users

    # ── 字段定义 ──
    id = Column(Integer, primary_key=True, autoincrement=True)
    #  ↑  Python 里的变量名           ↑  主键，不能重复、不能为空
    #     SQLAlchemy 自动映射到       ↑  autoincrement 让 MySQL 自动生成 1,2,3...
    #     MySQL 的 id 字段

    name = Column(String(100), nullable=False)
    #              ↑ VARCHAR(100)     ↑ 不能为空

    email = Column(String(255), unique=True, index=True)
    #                              ↑ 不能重复  ↑ 建索引，按邮箱查的时候快

    is_active = Column(Boolean, default=True)
    #                              ↑ 默认值为 True，不传就自动填 True

    created_at = Column(DateTime, server_default=func.now())
    #                                   ↑ MySQL 自动填入当前时间
```

这段 Python 代码等价于在 MySQL 里执行：

```sql
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(255) UNIQUE INDEX,
    is_active TINYINT(1) DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

---

## 字段类型大全

| Python 写法 | 对应 MySQL 类型 | 说明 |
|---|---|---|
| `Integer` | `INT` | 整数，常用自增 id |
| `String(255)` | `VARCHAR(255)` | 变长字符串，括号里写最大长度 |
| `Text` | `TEXT` | 大段文字（几千字那种） |
| `Boolean` | `TINYINT(1)` | 布尔值，存 0 或 1 |
| `Float` | `FLOAT` / `DOUBLE` | 小数（价格、评分） |
| `DateTime` | `DATETIME` | 日期 + 时间 |
| `Date` | `DATE` | 仅日期 |
| `BigInteger` | `BIGINT` | 很大的整数 |
| `JSON` | `JSON` | JSON 数据（MySQL 5.7+） |

## 字段选项大全

```python
Column(Integer, primary_key=True)        # 主键
Column(String(100), nullable=False)      # 不能为空
Column(String(100), default="默认值")     # 默认值
Column(String(255), unique=True)         # 不能重复（如邮箱、手机号）
Column(String(255), index=True)          # 建索引（查询快）
Column(String(100), nullable=False, index=True, unique=True)  # 可以叠加
```

---

## 完整例子：三张表

### user.py — 用户表

```python
# app/models/user.py

from sqlalchemy import Column, Integer, String, Boolean, DateTime, func

from app.core.database import Base


class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, autoincrement=True)
    username = Column(String(50), unique=True, nullable=False, index=True)
    email = Column(String(255), unique=True, index=True)
    hashed_password = Column(String(255), nullable=False)
    nickname = Column(String(100), default="")
    avatar_url = Column(String(500), default="")
    is_active = Column(Boolean, default=True)
    role = Column(String(20), default="user")     # "user" 或 "admin"
    created_at = Column(DateTime, server_default=func.now())
```

### item.py — 商品表

```python
# app/models/item.py

from sqlalchemy import Column, Integer, String, Float, Boolean, DateTime, Text, func

from app.core.database import Base


class Item(Base):
    __tablename__ = "items"

    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(200), nullable=False)
    description = Column(Text, default="")         # 大段描述
    price = Column(Float, nullable=False)          # 价格
    stock = Column(Integer, default=0)             # 库存
    is_published = Column(Boolean, default=False)  # 是否上架
    created_at = Column(DateTime, server_default=func.now())
```

### order.py — 订单表

```python
# app/models/order.py

from sqlalchemy import Column, Integer, String, Float, DateTime, func

from app.core.database import Base


class Order(Base):
    __tablename__ = "orders"

    id = Column(Integer, primary_key=True, autoincrement=True)
    user_id = Column(Integer, nullable=False, index=True)    # 下单用户 id
    item_id = Column(Integer, nullable=False)                 # 商品 id
    quantity = Column(Integer, default=1)                     # 数量
    total_price = Column(Float, nullable=False)               # 总价
    status = Column(String(20), default="pending")            # pending / paid / shipped
    created_at = Column(DateTime, server_default=func.now())
```

---

## models/ 里的文件写完后做什么

写完 models，下一步在 `schemas/` 里写**接口数据结构**，然后在 `services/` 里写**操作这些表的函数**。

```
写完 models/user.py
       ↓
写 schemas/user.py    ← 定义"客户端传什么、返回什么"
       ↓
写 services/user_service.py  ← 用 models 里的 User 操作数据库
```

## 和 database.py 的关系

```
database.py → 定义了 Base（模型基类）
                   ↓
models/user.py → 继承 Base，定义 users 表长什么样
                   ↓
main.py 启动时 → Base.metadata.create_all()
                   ↓
            MySQL 里创建 users 表
```

没有 `Base`，models 里的类就不知道自己是数据库表。没有 models，建表就没有依据。两者缺一不可。
