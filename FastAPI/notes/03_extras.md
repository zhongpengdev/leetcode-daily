# 补充功能

## CORS（前后端分离必配）

```python
from fastapi.middleware.cors import CORSMiddleware

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000"],   # 前端地址
    allow_methods=["*"],
    allow_headers=["*"],
)
```

开发时 `allow_origins=["*"]`，上线换具体域名。

---

## 错误处理

```python
from fastapi import HTTPException

@app.get("/items/{id}")
def get_item(id: int):
    if id not in items:
        raise HTTPException(status_code=404, detail="Item not found")
    return items[id]
```

---

## 表单数据

```python
from fastapi import Form

@app.post("/login")
def login(username: str = Form(), password: str = Form()):
    return {"username": username}
```

`Form()` 读取 `application/x-www-form-urlencoded` 格式（传统表单），不是 JSON。

---

## 文件上传

```python
from fastapi import File, UploadFile

@app.post("/upload")
async def upload(file: UploadFile = File()):
    content = await file.read()
    return {"filename": file.filename, "size": len(content)}
```

`UploadFile` 比 `bytes` 好用：自带文件名、类型，大文件不撑内存。

---

## 响应模型（过滤返回字段）

```python
class ItemOut(BaseModel):
    name: str
    price: float
    # 不包含 status 等内部字段

@app.get("/items", response_model=List[ItemOut])
def list_items():
    return db.query(models.Item).all()    # 自动只返回 name, price
```

---

这些功能按需查阅，核心是前三篇的内容。
