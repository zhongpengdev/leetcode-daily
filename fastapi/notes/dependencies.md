# 依赖清单（包 → 导入 → 作用）

## 包速查表

| 包名 | `pip install` | 导入什么 | 用来干啥 |
|---|---|---|---|
| FastAPI | `fastapi` | `from fastapi import FastAPI` | 框架本身 |
| ASGI 服务器 | `uvicorn` | 命令行工具，不 import | `uvicorn main:app --reload` 启动项目 |
| Pydantic | `pydantic`（装 FastAPI 时自动带） | `from pydantic import BaseModel` | 定义请求体、数据校验 |
| 配置管理 | `pydantic-settings` | `from pydantic_settings import BaseSettings` | 从环境变量读配置 |
| ORM | `sqlalchemy` | `from sqlalchemy import create_engine, Column, Integer, String` | 操作数据库的框架 |
| **MySQL 驱动** | `pymysql` | 不需要 import，连接串里指定即可 | 让 SQLAlchemy 能连 MySQL |
| **PG 驱动** | `psycopg2-binary` | 不需要 import，连接串里指定即可 | 让 SQLAlchemy 能连 PostgreSQL |
| **PG 异步驱动** | `asyncpg` | 不需要 import，连接串里指定即可 | 让 SQLAlchemy 异步连 PG |
| Redis | `redis` | `import redis.asyncio as aioredis` | 连接 Redis |
| 密码加密 | `passlib[bcrypt]` | `from passlib.context import CryptContext` | hash 密码 |
| JWT | `python-jose[cryptography]` | `from jose import jwt` | 签发和验证 JWT token |
| 表单解析 | `python-multipart` | 不需要 import | FastAPI `Form()` / `File()` 需要它 |

---

## 每条依赖详解

### FastAPI

```python
from fastapi import FastAPI, APIRouter, Depends, HTTPException, Header, File, UploadFile, Form
from fastapi.security import OAuth2PasswordBearer, OAuth2PasswordRequestForm
from fastapi.middleware.cors import CORSMiddleware
```

各导入的作用：

| 导入 | 作用 |
|---|---|
| `FastAPI` | 创建 app 实例 `app = FastAPI()` |
| `APIRouter` | 路由模块化拆文件 |
| `Depends` | 依赖注入 |
| `HTTPException` | 返回错误 `raise HTTPException(404)` |
| `Header` | 从请求头取值 |
| `UploadFile` | 接收上传的文件 |
| `Form` | 接收表单数据 |
| `OAuth2PasswordBearer` | 从请求头提取 Bearer token |
| `OAuth2PasswordRequestForm` | 解析登录表单的 username+password |
| `CORSMiddleware` | 配置跨域 |

### SQLAlchemy

```python
from sqlalchemy import create_engine, Column, Integer, String, Boolean, DateTime, Text, Float
from sqlalchemy.orm import sessionmaker, declarative_base, Session
from sqlalchemy import func, select
```

| 导入 | 作用 |
|---|---|
| `create_engine` | 创建数据库引擎（连接池） |
| `Column` | 定义表的一个字段 |
| `Integer / String / Boolean / DateTime` | 字段类型 |
| `sessionmaker` | 创建 Session 工厂 |
| `declarative_base` | ORM 模型的基类 |
| `Session` | 类型标注用 `db: Session` |
| `func` | 数据库函数如 `func.now()` |
| `select` | 异步查询用 `await execute(select(Model))` |

### Pydantic

```python
from pydantic import BaseModel, Field, EmailStr
```

| 导入 | 作用 |
|---|---|
| `BaseModel` | 定义请求/响应数据结构 |
| `Field` | 字段级别校验 `Field(min_length=1)` |
| `EmailStr` | 自动校验邮箱格式，需装 `pydantic[email]` |

### passlib（密码加密）

```python
from passlib.context import CryptContext

pwd_context = CryptContext(schemes=["bcrypt"])   # 指定用 bcrypt

pwd_context.hash("123456")      # 加密 → "$2b$12$..."
pwd_context.verify("123456", hashed)  # 验证 → True/False
```

### python-jose（JWT）

```python
from jose import jwt

jwt.encode({"sub": "42"}, "secret", algorithm="HS256")   # 签发
jwt.decode(token, "secret", algorithms=["HS256"])         # 解码
```

### redis

```python
import redis.asyncio as aioredis

redis_client = aioredis.from_url("redis://localhost:6379/0", decode_responses=True)

await redis_client.set("key", "value", ex=3600)
await redis_client.get("key")
```

### pydantic-settings（配置）

```python
from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    database_url: str = "mysql+pymysql://root:123456@localhost:3306/mydb"

    class Config:
        env_file = ".env"

settings = Settings()
```

会自动从 `.env` 文件读取值覆盖默认值。字段名 `database_url` 对应环境变量 `DATABASE_URL`。

---

## 安装一条命令

```bash
pip install fastapi uvicorn sqlalchemy pymysql redis passlib[bcrypt] python-jose[cryptography] python-multipart pydantic-settings
```

或者放进 `requirements.txt`：

```
fastapi
uvicorn
sqlalchemy
pymysql
redis
passlib[bcrypt]
python-jose[cryptography]
python-multipart
pydantic-settings
```
