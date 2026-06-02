# 路由

## 路径参数

```python
@app.get("/items/{item_id}")
def get_item(item_id: int):     # 类型标注 → 自动校验 + 转换
    return {"item_id": item_id}
```

类型标注不只是文档装饰，传非 int 自动 422。

## 查询参数

```python
@app.get("/items")
def list_items(page: int = 1, size: int = 10):     # 有默认值 → 可选
    return {"page": page, "size": size}

@app.get("/items/search")
def search(q: str):                                 # 无默认值 → 必填
    return {"query": q}
```

访问 `/items?page=2&size=5` → page=2, size=5。

## 多方法

```python
@app.get("/resources")       # 查
@app.post("/resources")      # 增 → status_code=201
@app.put("/resources/{id}")  # 全量改
@app.delete("/resources/{id}")  # 删 → status_code=204
```

---

## 工程化：APIRouter 拆模块

项目大了，路由不能全堆在 `main.py`。

### 目录结构

```
app/
├── main.py               # FastAPI 实例
└── routers/
    ├── __init__.py
    ├── items.py
    └── users.py
```

### 定义子路由

`app/routers/items.py`：

```python
from fastapi import APIRouter

router = APIRouter(prefix="/items", tags=["items"])

items_db = [
    {"id": 1, "name": "macbook", "price": 12999},
    {"id": 2, "name": "iphone", "price": 8999},
]

@router.get("/")                  # → GET /items/
def list_items():
    return items_db

@router.get("/{item_id}")         # → GET /items/1
def get_item(item_id: int):
    return items_db[item_id - 1]
```

### 注册到 main.py

`app/main.py`：

```python
from fastapi import FastAPI
from app.routers.items import router as items_router
from app.routers.users import router as users_router

app = FastAPI()

app.include_router(items_router)
app.include_router(users_router)
```

`prefix` 让路由不用重复写 `/items`，`tags` 让 `/docs` 页面按组显示。

### 什么时候拆

| main.py 行数 | 做法 |
|---|---|
| < 100 行 | 全写 main.py |
| 100-200 行 | 拆成 2-3 个 APIRouter |
| > 200 行 | 按业务模块拆分目录 |

### 模块级鉴权

```python
router = APIRouter(
    prefix="/admin",
    tags=["admin"],
    dependencies=[Depends(verify_token)]   # 所有路由先过鉴权
)

@router.get("/stats")
def stats(): ...         # 自动走 verify_token
```

---

**完成了 → 下一步 [02_database.md](02_database.md)**
