# 从零启动一个 FastAPI 项目

## 1. 安装

```bash
pip install fastapi uvicorn
```

就这两个依赖，其他按需加（数据库加 `sqlalchemy`，模板加 `jinja2` 等）。

## 2. 创建第一个文件

项目目录结构：

```
my-project/
└── main.py            # 一个文件就能跑
```

`main.py`：

```python
from fastapi import FastAPI

app = FastAPI()

@app.get("/")
def root():
    return {"message": "hello world"}
```

## 3. 运行

```bash
uvicorn main:app --reload
```

- `main` = 文件名（main.py）
- `app` = FastAPI 实例名
- `--reload` = 改代码自动重启（开发必加）

打开浏览器访问 `http://127.0.0.1:8000`，看到 `{"message":"hello world"}`。

自动文档：`http://127.0.0.1:8000/docs`（Swagger UI）

## 4. 加一个带参数的接口

```python
@app.get("/items/{item_id}")
def get_item(item_id: int):
    return {"item_id": item_id}
```

访问 `http://127.0.0.1:8000/items/42`，返回 `{"item_id":42}`。

传非数字（如 `items/abc`）自动返回 422 错误，不用写校验代码。

## 5. 加一个 POST 接口

```python
from pydantic import BaseModel

class Item(BaseModel):
    name: str
    price: float

@app.post("/items")
def create_item(item: Item):
    return {"name": item.name, "price": item.price}
```

用 `curl` 测试：

```bash
curl -X POST http://127.0.0.1:8000/items \
  -H "Content-Type: application/json" \
  -d '{"name": "键盘", "price": 199}'
```

## 总结

一个 FastAPI 项目的起点就是 **一个 main.py + 一个 uvicorn 命令**。先跑起来，再考虑拆文件。

下一步 → [01_routing.md](01_routing.md) — 路由怎么写、怎么拆成多个文件
