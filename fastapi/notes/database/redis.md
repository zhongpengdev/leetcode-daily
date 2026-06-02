# Redis 完整教程

Redis 在 FastAPI 里最常见的用途是**做缓存**——查询 MySQL 前先看 Redis 有没有，有就直接返回，没有再去查库并写入 Redis。

---

## 最终项目结构

```
my-project/
├── .env
├── app/
│   ├── main.py
│   ├── core/
│   │   ├── config.py       # Redis 连接串放这
│   │   └── redis.py        # Redis 客户端
│   └── routers/
│       └── items.py        # 演示：缓存商品查询
```

Redis 不涉及 ORM 表，所以不需要 models/ 和 services/。

---

## 第 1 步：安装

```bash
pip install redis
```

## 第 2 步：.env

```ini
# .env
REDIS_URL=redis://localhost:6379/0
# 有密码则：redis://:你的密码@localhost:6379/0
```

最后一个 `/0` 是 Redis 的库编号（0-15），默认用 0。

## 第 3 步：config.py

```python
# app/core/config.py

from pydantic_settings import BaseSettings


class Settings(BaseSettings):
    redis_url: str = "redis://localhost:6379/0"

    class Config:
        env_file = ".env"


settings = Settings()
```

## 第 4 步：core/redis.py — 创建 Redis 客户端

```python
# app/core/redis.py

import redis.asyncio as aioredis

from app.core.config import settings


# Redis 客户端，全局单例
redis_client = aioredis.from_url(
    settings.redis_url,
    decode_responses=True,      # 返回字符串而不是 bytes（默认是 bytes）
)
```

## 第 5 步：在路由里使用

```python
# app/routers/items.py

from fastapi import APIRouter, Depends
from redis.asyncio import Redis

from app.core.redis import redis_client
from app.core.database import SessionLocal


# ── 依赖：拿 Redis 客户端 ──
def get_redis():
    return redis_client


# ── 依赖：拿数据库 session（从 MySQL 篇复制过来） ──
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


router = APIRouter(prefix="/items", tags=["items"])


@router.get("/{item_id}")
async def get_item(item_id: int, redis: Redis = Depends(get_redis)):
    """
    GET /items/1

    先查 Redis 缓存，有就直接返回，没有就查数据库。
    """
    # 1. 先查 Redis
    cache_key = f"item:{item_id}"
    cached = await redis.get(cache_key)

    if cached:
        # 缓存命中，直接返回
        return {"from_cache": True, "data": cached}

    # 2. 缓存没命中，查 MySQL（伪代码示意）
    # item = db.query(Item).filter(Item.id == item_id).first()
    item_data = '{"id": 1, "name": "macbook", "price": 12999}'

    # 3. 写入 Redis，有效期 300 秒，下次就不走 DB 了
    await redis.set(cache_key, item_data, ex=300)

    return {"from_cache": False, "data": item_data}


@router.delete("/cache/{item_id}")
async def clear_cache(item_id: int, redis: Redis = Depends(get_redis)):
    """
    DELETE /items/cache/1

    数据更新时清除缓存，避免脏数据。
    """
    await redis.delete(f"item:{item_id}")
    return {"message": "cache cleared"}
```

---

## Redis 三个核心操作

| 操作 | 代码 | 说明 |
|---|---|---|
| 存 | `await redis.set(key, value, ex=秒)` | 存键值对，设过期时间 |
| 取 | `await redis.get(key)` | 有值返回 value，没有返回 None |
| 删 | `await redis.delete(key)` | 删除 key |

开发时可以用 `redis.get(key)` 检查某个缓存是否存在。

---

## Redis 装在哪个环境

选项 A：**本地装 Redis**（推荐开发用）

```bash
# Ubuntu / WSL
sudo apt install redis-server
sudo service redis-server start

# Mac
brew install redis
brew services start redis
```

选项 B：**Docker**

```bash
docker run -d --name redis -p 6379:6379 redis
```

---

## 和 MySQL 配合的典型流程

```
请求 GET /items/42
    │
    ▼
  查 Redis（key = "item:42"）
    │
    ├── 有结果 → 直接返回（几毫秒）
    │
    └── 没结果 → 查 MySQL → 写入 Redis（设过期时间）→ 返回
```

等遇到"这个接口查了好多次 MySQL 太慢了"的时候再加 Redis。
