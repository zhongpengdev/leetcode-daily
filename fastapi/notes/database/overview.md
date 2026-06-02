# 数据库连接

所有数据库的连接串、依赖安装和基本 CRUD。

---

## MySQL

[mysql.md](mysql.md)

```python
# 连接串格式：
mysql_url = "mysql+pymysql://用户名:密码@主机地址:端口/数据库名"

# 实例：
mysql_url = "mysql+pymysql://root:123456@localhost:3306/mydb"
#                     ↑用户名 ↑密码   ↑主机  ↑端口 ↑库名

# 需要的包：pip install pymysql
# 不需要 import pymysql，SQLAlchemy 自动调用
```

## PostgreSQL

[postgresql.md](postgresql.md)

```python
# 同步：
postgres_url = "postgresql://用户名:密码@主机:端口/数据库名"
# 实例：
postgres_url = "postgresql://postgres:123456@localhost:5432/mydb"

# 异步：
postgres_async_url = "postgresql+asyncpg://postgres:123456@localhost:5432/mydb"

# 包：pip install psycopg2-binary  （异步装 asyncpg）
```

## Redis

[redis.md](redis.md)

```python
redis_url = "redis://:密码@主机:端口/库编号"
# 无密码：
redis_url = "redis://localhost:6379/0"
# 有密码：
redis_url = "redis://:123456@localhost:6379/0"

# 包：pip install redis
```

## config.py 统一管理

```python
from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    mysql_url: str = "mysql+pymysql://root:123456@localhost:3306/mydb"
    redis_url: str = "redis://localhost:6379/0"

    class Config:
        env_file = ".env"

settings = Settings()
```

`.env` 文件（不上传 git）：

```env
mysql_url=mysql+pymysql://root:123456@localhost:3306/mydb
redis_url=redis://localhost:6379/0
```
