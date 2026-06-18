# MySQL 完整教程

从零到跑通，每个文件完整代码，遵循项目结构规范。

---

## 最终项目结构

```
my-project/
├── .env                    # 配置文件（不上传 git）
├── app/
│   ├── main.py             # 入口，启动项目
│   ├── core/
│   │   ├── __init__.py
│   │   ├── config.py       # 统一配置
│   │   └── database.py     # 数据库连接
│   ├── models/
│   │   ├── __init__.py
│   │   └── user.py         # 用户表
│   ├── schemas/
│   │   ├── __init__.py
│   │   └── user.py         # 请求/响应数据结构
│   ├── services/
│   │   ├── __init__.py
│   │   └── user_service.py # 增删改查业务逻辑
│   └── routers/
│       ├── __init__.py
│       └── users.py        # 用户相关路由
```

---

## 第 1 步：安装

```bash
pip install fastapi uvicorn sqlalchemy pymysql pydantic-settings
```

## 第 2 步：.env — 存放敏感配置

```ini
# .env（放在项目根目录）
MYSQL_URL=mysql+pymysql://root:123456@localhost:3306/test_db
```

`.gitignore` 里加 `.env`，不要上传到 git。

## 第 3 步：core/config.py — 统一管理所有配置

```python
# app/core/config.py

from pydantic_settings import BaseSettings


class Settings(BaseSettings):
    # 从 .env 文件读取 MYSQL_URL，不写的话用默认值
    mysql_url: str = "mysql+pymysql://root:123456@localhost:3306/test_db"

    class Config:
        env_file = ".env"          # 告诉 pydantic 从 .env 读


# 全局单例，其他地方 from app.core.config import settings 即可使用
settings = Settings()
```

**为什么这样写：**

- 所有配置集中在 `config.py`，数据库、Redis、JWT 密钥等都写在这
- 敏感信息放 `.env` 不上传 git，生产环境在服务器上设环境变量即可
- 其他地方只需要 `from app.core.config import settings` 就能拿到配置

## 第 4 步：core/database.py — 连接 MySQL

```python
# app/core/database.py

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, declarative_base

from app.core.config import settings    # 从 config.py 获取连接串


# 1. 创建引擎（连接池）
engine = create_engine(
    settings.mysql_url,        # 从 config 拿连接串
    pool_size=5,               # 连接池保持 5 个连接
    max_overflow=10,           # 繁忙时最多额外开 10 个
    pool_pre_ping=True,        # 每次取连接前 ping 一下，断开自动重连
    echo=True,                 # 打印 SQL 日志（调试用，上线关掉）
)

# 2. Session 工厂
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

# 3. 模型基类
Base = declarative_base()
```

## 第 5 步：models/user.py — 定义表结构

```python
# app/models/user.py

from sqlalchemy import Column, Integer, String, Boolean, DateTime, func

from app.core.database import Base


class User(Base):
    # 数据库中的表名
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(100), nullable=False)
    email = Column(String(255), unique=True, index=True)
    is_active = Column(Boolean, default=True)
    created_at = Column(DateTime, server_default=func.now())
```

## 第 6 步：schemas/user.py — 定义接口数据结构

```python
# app/schemas/user.py

from pydantic import BaseModel
from datetime import datetime


# 创建用户时客户端传什么
class UserCreate(BaseModel):
    name: str
    email: str


# 服务器返回什么给客户端（不含敏感字段）
class UserResponse(BaseModel):
    id: int
    name: str
    email: str
    is_active: bool
    created_at: datetime
```

## 第 7 步：services/user_service.py — 增删改查

```python
# app/services/user_service.py

from sqlalchemy.orm import Session

from app.models.user import User
from app.schemas.user import UserCreate


def create_user(db: Session, data: UserCreate) -> User:
    """新增用户"""
    user = User(name=data.name, email=data.email)
    db.add(user)
    db.commit()
    db.refresh(user)    # 拿到自增 id
    return user


def get_user(db: Session, user_id: int) -> User | None:
    """按 id 查询"""
    return db.query(User).filter(User.id == user_id).first()


def get_user_by_email(db: Session, email: str) -> User | None:
    """按邮箱查询"""
    return db.query(User).filter(User.email == email).first()


def list_users(db: Session, skip: int = 0, limit: int = 20) -> list[User]:
    """用户列表（分页）"""
    return db.query(User).offset(skip).limit(limit).all()


def update_user(db: Session, user_id: int, name: str) -> User | None:
    """修改用户名"""
    user = get_user(db, user_id)
    if not user:
        return None
    user.name = name
    db.commit()
    db.refresh(user)
    return user


def delete_user(db: Session, user_id: int) -> bool:
    """删除用户"""
    user = get_user(db, user_id)
    if not user:
        return False
    db.delete(user)
    db.commit()
    return True
```

## 第 8 步：main.py — 入口，建表 + 注册路由

```python
# app/main.py

from fastapi import FastAPI

from app.core.database import engine, Base
from app.models import user        # import 让 Python 加载模型定义
from app.routers import users      # import 路由


# 在数据库中创建所有表（表已存在则跳过）
Base.metadata.create_all(bind=engine)


app = FastAPI()

# 注册路由
app.include_router(users.router)
```

## 第 9 步：routers/users.py — 路由

```python
# app/routers/users.py

from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from app.core.database import SessionLocal
from app.schemas.user import UserCreate, UserResponse
from app.services.user_service import (
    create_user,
    get_user,
    list_users,
    update_user,
    delete_user,
)


# ═══════════════════════════════════════════
# 依赖：每个请求拿一个数据库 session
# ═══════════════════════════════════════════
def get_db():
    """
    每次请求进来：
      1. SessionLocal() → 创建一个数据库连接
      2. yield db       → 交给路由用
      3. db.close()     → 请求结束关闭连接
    """
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


router = APIRouter(prefix="/users", tags=["users"])


@router.post("/", response_model=UserResponse, status_code=201)
def create_user_route(data: UserCreate, db: Session = Depends(get_db)):
    """
    POST /users
    Body: {"name": "张三", "email": "a@test.com"}
    """
    # 先检查邮箱是否已存在
    existing = get_user_by_email(db, data.email)
    if existing:
        raise HTTPException(status_code=400, detail="Email already exists")
    return create_user(db, data)


@router.get("/", response_model=list[UserResponse])
def list_users_route(skip: int = 0, limit: int = 20, db: Session = Depends(get_db)):
    """
    GET /users?skip=0&limit=20
    """
    return list_users(db, skip=skip, limit=limit)


@router.get("/{user_id}", response_model=UserResponse)
def get_user_route(user_id: int, db: Session = Depends(get_db)):
    """
    GET /users/1
    """
    user = get_user(db, user_id)
    if not user:
        raise HTTPException(status_code=404, detail="User not found")
    return user


@router.put("/{user_id}", response_model=UserResponse)
def update_user_route(user_id: int, name: str, db: Session = Depends(get_db)):
    """
    PUT /users/1?name=新名字
    """
    user = update_user(db, user_id, name)
    if not user:
        raise HTTPException(status_code=404, detail="User not found")
    return user


@router.delete("/{user_id}")
def delete_user_route(user_id: int, db: Session = Depends(get_db)):
    """
    DELETE /users/1
    """
    if not delete_user(db, user_id):
        raise HTTPException(status_code=404, detail="User not found")
    return {"message": "ok"}
```

## 第 10 步：启动

```bash
uvicorn app.main:app --reload
```

访问 `http://127.0.0.1:8000/docs` 测试接口。

---

## 整体流程回顾

```
启动时：
  main.py 启动
    → Base.metadata.create_all()    在 MySQL 建表
    → app.include_router(users)     注册路由

请求时：
  客户端 POST /users
    → Depends(get_db) → SessionLocal() 创建连接
    → 路由函数处理 → service 层操作数据库
    → 返回响应 → get_db 收尾 → db.close()
```

## 配置走的路径

```
.env 文件                    → 只在本地开发用
  ↓
config.py 读取 .env          → 统一管理所有配置
  ↓
database.py 用 config 的值   → settings.mysql_url 拿连接串
  ↓
其他所有地方用 settings       → from app.core.config import settings
```
