# 完整项目结构（生产级）

前面几篇讲了每个部件怎么用，这篇讲**真实项目完整布局**——数据库、Redis、鉴权、业务逻辑各放哪，为什么。

---

## 完整目录树

```
my-project/
├── app/
│   ├── main.py                  # FastAPI 实例 + 启动
│   │
│   ├── core/                    # 基础设施层
│   │   ├── __init__.py
│   │   ├── config.py            # 配置（环境变量）
│   │   ├── database.py          # 数据库引擎 + Session
│   │   ├── security.py          # JWT / 密码加密 / OAuth
│   │   └── redis.py             # Redis 连接
│   │
│   ├── models/                  # 数据库表模型（ORM）
│   │   ├── __init__.py
│   │   ├── user.py
│   │   └── item.py
│   │
│   ├── schemas/                 # 请求/响应数据结构（Pydantic）
│   │   ├── __init__.py
│   │   ├── user.py
│   │   └── item.py
│   │
│   ├── routers/                 # API 路由（只负责路由转发）
│   │   ├── __init__.py
│   │   ├── auth.py              # 登录/注册
│   │   ├── users.py
│   │   └── items.py
│   │
│   ├── services/                # 业务逻辑层（核心）
│   │   ├── __init__.py
│   │   ├── user_service.py
│   │   ├── item_service.py
│   │   └── auth_service.py
│   │
│   ├── dependencies.py          # 全局共享依赖（get_db、get_current_user）
│   │
│   └── utils/                   # 工具函数
│       ├── __init__.py
│       └── pagination.py
│
├── tests/                       # 测试
│   ├── conftest.py
│   ├── test_users.py
│   └── test_items.py
│
├── alembic/                     # 数据库迁移
│   └── versions/
│
├── .env                         # 环境变量（不上传 git）
├── .gitignore
├── requirements.txt
└── Dockerfile
```

---

## 各层职责

### core/ — 基础设施

整个项目的"地基"，不依赖业务。

**config.py** — 所有配置集中管理：

```python
from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    database_url: str = "sqlite:///./test.db"
    redis_url: str = "redis://localhost:6379"
    jwt_secret: str = "change-me"
    jwt_expire_minutes: int = 60

    class Config:
        env_file = ".env"

settings = Settings()   # 全局单例
```

**database.py** — 只负责创建引擎和 session：

```python
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, declarative_base
from app.core.config import settings

engine = create_engine(settings.database_url)
SessionLocal = sessionmaker(bind=engine)
Base = declarative_base()
```

**security.py** — 加密、JWT 等所有安全相关：

```python
from datetime import datetime, timedelta
from jose import jwt
from passlib.context import CryptContext
from app.core.config import settings

pwd_context = CryptContext(schemes=["bcrypt"])

def hash_password(password: str) -> str:
    return pwd_context.hash(password)

def verify_password(plain: str, hashed: str) -> bool:
    return pwd_context.verify(plain, hashed)

def create_access_token(user_id: int) -> str:
    expire = datetime.utcnow() + timedelta(minutes=settings.jwt_expire_minutes)
    return jwt.encode({"sub": str(user_id), "exp": expire}, settings.jwt_secret)

def decode_token(token: str) -> dict:
    return jwt.decode(token, settings.jwt_secret, algorithms=["HS256"])
```

**redis.py** — Redis 连接：

```python
import redis.asyncio as redis
from app.core.config import settings

redis_client = redis.from_url(settings.redis_url)
```

### models/ — 数据库表

只放 ORM 模型，不放业务逻辑：

```python
# app/models/user.py
from sqlalchemy import Column, Integer, String, Boolean
from app.core.database import Base

class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, index=True)
    email = Column(String, unique=True, index=True)
    hashed_password = Column(String)
    is_active = Column(Boolean, default=True)
```

### schemas/ — 数据接口

定义客户端和服务器之间传什么数据：

```python
# app/schemas/user.py
from pydantic import BaseModel, EmailStr

class UserCreate(BaseModel):
    email: EmailStr
    password: str

class UserResponse(BaseModel):
    id: int
    email: str
    is_active: bool
```

### routers/ — 路由层

**只做路由转发**，不写业务逻辑：

```python
# app/routers/users.py
from fastapi import APIRouter, Depends
from app.services import user_service
from app.dependencies import get_db, get_current_user
from app.schemas.user import UserResponse

router = APIRouter(prefix="/users", tags=["users"])

@router.get("/me", response_model=UserResponse)
def get_me(current_user: User = Depends(get_current_user)):
    return current_user

@router.get("/{user_id}", response_model=UserResponse)
def get_user(user_id: int, db: Session = Depends(get_db)):
    return user_service.get_by_id(db, user_id)
```

### services/ — 业务逻辑层

**核心业务代码都在这**，可以被路由调用，也可以被测试直接调用：

```python
# app/services/user_service.py
from sqlalchemy.orm import Session
from app.models.user import User
from app.schemas.user import UserCreate
from app.core.security import hash_password

def get_by_id(db: Session, user_id: int) -> User:
    return db.query(User).filter(User.id == user_id).first()

def create(db: Session, data: UserCreate) -> User:
    user = User(email=data.email, hashed_password=hash_password(data.password))
    db.add(user)
    db.commit()
    db.refresh(user)
    return user
```

### dependencies.py — 全局依赖

```python
from app.core.database import SessionLocal
from app.core.security import decode_token
from app.models.user import User

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

def get_current_user(token: str = Header(...), db: Session = Depends(get_db)):
    payload = decode_token(token)
    user = db.query(User).filter(User.id == int(payload["sub"])).first()
    if not user:
        raise HTTPException(status_code=401)
    return user
```

---

## 数据流

```
客户端请求
    │
    ▼
routers/       ← 路由解析参数、调用 service
    │
    ▼
services/      ← 业务逻辑：校验、计算、调用外部
    │
    ├── models/    ← 读写数据库
    ├── core/      ← JWT、Redis 等基础设施
    └── schemas/  ← 定义数据结构
```

**为什么 routers 和 services 要分开？**

```
错误写法：             正确写法：
router 里直接调 DB     router → service → DB
                       service 不依赖 router
                       测试可以单独测 service
```

---

## 各组件位置速查

| 组件 | 位置 |
|---|---|
| 数据库连接 | `core/database.py` |
| 表模型 ORM | `models/*.py` |
| Redis | `core/redis.py` |
| JWT / 密码 | `core/security.py` |
| 鉴权依赖 | `dependencies.py`（get_current_user） |
| 配置/环境变量 | `core/config.py` |
| 增删改查业务 | `services/*.py` |
| 路由转发 | `routers/*.py` |

---

## 怎么从零建这个项目

```bash
# 1. 创建目录
mkdir -p my-project/app/{core,models,schemas,routers,services}

# 2. 每个目录放一个 __init__.py
touch my-project/app/{core,models,schemas,routers,services}/__init__.py
touch my-project/app/dependencies.py

# 3. 从 core/config.py 开始写，然后是 database.py、models、
#    schemas、services、routers，最后 main.py 串起来
```

顺序：**config → database → models → schemas → services → dependencies → routers → main**

---

这篇讲完了。回到 [overview.md](overview.md) 看完整路线。
