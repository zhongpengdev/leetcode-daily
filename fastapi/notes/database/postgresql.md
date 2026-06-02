# PostgreSQL 完整教程

和在 MySQL 篇里用的同一套项目结构，区别只在驱动和连接串。

---

## 最终项目结构

```
my-project/
├── .env
├── app/
│   ├── main.py
│   ├── core/
│   │   ├── config.py       # PG 连接串放这
│   │   └── database.py     # 连接 PG
│   ├── models/
│   │   └── user.py
│   ├── schemas/
│   │   └── user.py
│   ├── services/
│   │   └── user_service.py
│   └── routers/
│       └── users.py
```

跟 MySQL 篇比，只改两个文件：`config.py` 加 PG 配置，`database.py` 换驱动。

---

## 第 1 步：安装

```bash
# 同步方式（和 MySQL 一样用法）
pip install psycopg2-binary

# 或异步方式（性能更好）
pip install asyncpg
```

## 第 2 步：.env

```ini
# .env
PG_URL=postgresql://postgres:123456@localhost:5432/test_db
```

## 第 3 步：core/config.py

```python
# app/core/config.py

from pydantic_settings import BaseSettings


class Settings(BaseSettings):
    # PostgreSQL 连接串
    pg_url: str = "postgresql://postgres:123456@localhost:5432/test_db"

    class Config:
        env_file = ".env"


settings = Settings()
```

## 第 4 步：core/database.py

```python
# app/core/database.py

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, declarative_base

from app.core.config import settings


engine = create_engine(
    settings.pg_url,
    pool_size=5,
    max_overflow=10,
    pool_pre_ping=True,
    echo=True,
)

SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
Base = declarative_base()
```

跟 MySQL 的 database.py **完全一样**，只是 `settings.mysql_url` 变成了 `settings.pg_url`。

## 第 5 步及之后：models / schemas / services / routers

跟 MySQL 篇的**一模一样**，直接复制即可。因为 SQLAlchemy 屏蔽了数据库差异，ORM 代码不区分 MySQL 还是 PG。

---

## 异步方式（PostgreSQL 常见做法）

上面是同步写法，跟 MySQL 完全一致。下面是异步写法——PostgreSQL 用的最多。

### 安装

```bash
pip install asyncpg
```

### 同步 vs 异步差异

| | 同步 | 异步 |
|---|---|---|
| 驱动 | `psycopg2-binary` | `asyncpg` |
| 连接串 | `postgresql://user:pass@host/db` | `postgresql+asyncpg://user:pass@host/db` |
| 引擎 | `create_engine` | `create_async_engine` |
| Session | `sessionmaker` | `async_sessionmaker` |
| 查询 | `db.query(Model)` | `await db.execute(select(Model))` |
| 路由 | `def` | `async def` |

### 异步 database.py

```python
# app/core/database.py

from sqlalchemy.ext.asyncio import create_async_engine, async_sessionmaker, AsyncSession
from sqlalchemy.orm import declarative_base

from app.core.config import settings


engine = create_async_engine(
    settings.pg_url,       # 连接串得是 postgresql+asyncpg://... 格式
    pool_size=5,
    max_overflow=10,
    pool_pre_ping=True,
    echo=True,
)

AsyncSessionLocal = async_sessionmaker(engine, class_=AsyncSession, expire_on_commit=False)
Base = declarative_base()
```

### 异步 get_db 依赖

```python
# 在 routers/users.py 或其他地方

from app.core.database import AsyncSessionLocal

async def get_db():
    async with AsyncSessionLocal() as db:
        yield db
```

### 异步 model

和同步一模一样，不用改：

```python
from sqlalchemy import Column, Integer, String
from app.core.database import Base

class User(Base):
    __tablename__ = "users"
    id = Column(Integer, primary_key=True)
    name = Column(String(100))
```

### 异步 service

```python
# app/services/user_service.py

from sqlalchemy import select
from sqlalchemy.ext.asyncio import AsyncSession

from app.models.user import User
from app.schemas.user import UserCreate


async def create_user(db: AsyncSession, data: UserCreate) -> User:
    user = User(name=data.name, email=data.email)
    db.add(user)
    await db.commit()
    await db.refresh(user)
    return user


async def get_user(db: AsyncSession, user_id: int) -> User | None:
    result = await db.execute(select(User).where(User.id == user_id))
    return result.scalar_one_or_none()


async def list_users(db: AsyncSession) -> list[User]:
    result = await db.execute(select(User))
    return result.scalars().all()
```

### 异步路由

```python
@router.post("/")
async def create_user_route(data: UserCreate, db: AsyncSession = Depends(get_db)):
    return await create_user(db, data)

@router.get("/")
async def list_users_route(db: AsyncSession = Depends(get_db)):
    return await list_users(db)
```

### 同步 vs 异步变化总结

```
同步写法                   异步写法
─────────────────         ─────────────────
def get_user():           async def get_user():
    db.query(User)...         result = await db.execute(select(User)...)
                              return result.scalar_one_or_none()

db.commit()               await db.commit()
db.refresh(user)          await db.refresh(user)
```

其他全部一样。
