# 数据库 + 依赖注入

## 核心概念：Depends

FastAPI 的依赖注入（`Depends`）用来**抽离共用逻辑**。最典型的就是数据库 session 的获取和关闭。

先理解基本用法：

```python
from fastapi import Depends

def pagination(page: int = 1, size: int = 10):
    return {"page": page, "size": size}

@app.get("/items")
def list_items(p: dict = Depends(pagination)):   # 自动调用 pagination()
    return p

@app.get("/users")
def list_users(p: dict = Depends(pagination)):   # 复用
    return p
```

`Depends` 把函数返回值注入到路由参数，不用在每个路由里重复写参数解析。

---

## SQLAlchemy 集成

### 目录结构

```
app/
├── main.py
├── database.py        # 数据库引擎 + session
├── models.py          # 表定义
├── schemas.py         # Pydantic 请求/响应模型
└── routers/
    ├── items.py
    └── users.py
```

### database.py — 数据库连接

```python
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, declarative_base

DATABASE_URL = "sqlite:///./test.db"   # 开发用 SQLite

engine = create_engine(DATABASE_URL, connect_args={"check_same_thread": False})
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
Base = declarative_base()
```

### models.py — 定义表

```python
from sqlalchemy import Column, Integer, String, Float
from app.database import Base

class Item(Base):
    __tablename__ = "items"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, index=True)
    price = Column(Float)
```

### schemas.py — 定义 Pydantic 模型

```python
from pydantic import BaseModel

class ItemCreate(BaseModel):       # 客户端 POST 时发什么
    name: str
    price: float

class ItemResponse(BaseModel):     # 服务器返回什么（不含内部字段）
    id: int
    name: str
    price: float
```

### 依赖 — get_db（关键）

```python
# 在 dependencies.py 或 database.py
from app.database import SessionLocal

def get_db():
    db = SessionLocal()
    try:
        yield db               # 注入给路由
    finally:
        db.close()             # 请求结束自动关闭
```

`yield` 是关键：**请求进来时创建 session，请求结束时自动关闭**。

### routers/items.py — 实际使用

```python
from fastapi import APIRouter, Depends
from sqlalchemy.orm import Session
from app.database import get_db
from app import models, schemas

router = APIRouter(prefix="/items", tags=["items"])

@router.get("/")
def list_items(db: Session = Depends(get_db)):
    return db.query(models.Item).all()

@router.post("/", response_model=schemas.ItemResponse, status_code=201)
def create_item(item: schemas.ItemCreate, db: Session = Depends(get_db)):
    db_item = models.Item(name=item.name, price=item.price)
    db.add(db_item)
    db.commit()
    db.refresh(db_item)
    return db_item
```

### main.py — 初始化表

```python
from app.database import engine, Base
from app import models       # 确保 models 被加载，表才会创建

Base.metadata.create_all(bind=engine)    # 创建所有表
```

---

## 为什么这么设计

```
请求进来 → get_db() → [yield db] → 路由函数 → [db.close()]
                     └── 路由用 db  ──┘   ← 请求结束回到这里
```

`get_db` 的 `yield` 前后分别是**分配资源**和**释放资源**。不用在每个路由里手动 try-finally。

---

## 下一步

基础功能够了 → [03_extras.md](03_extras.md) 看 CORS、错误处理、文件上传等补充内容。
