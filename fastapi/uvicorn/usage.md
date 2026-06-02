# Uvicorn

## 是什么

Uvicorn 是一个 **ASGI 服务器**，FastAPI 的"发动机"。

```
你的 FastAPI 代码  →  Uvicorn（启动） →  接收 HTTP 请求
```

没有 Uvicorn，FastAPI 代码只是一个 Python 对象，不会监听端口、不会处理请求。

---

## 最常用法

### 基础启动

```bash
uvicorn main:app
#        └──┘└─┘
#        文件  app 实例名
```

### 热重载（开发必开）

```bash
uvicorn main:app --reload
```

改代码自动重启，不--reload 每次手动关掉再起。

### 改端口和主机

```bash
uvicorn main:app --reload --host 0.0.0.0 --port 8000
```

- `--host 0.0.0.0` → 局域网其他设备也能访问（默认 127.0.0.1 只能本机）
- `--port 8888` → 改端口

---

## 完整命令速查

```bash
# 最简
uvicorn main:app

# 开发
uvicorn main:app --reload --host 0.0.0.0 --port 8000

# 生产（需配合 --workers，仅 Linux）
uvicorn main:app --host 0.0.0.0 --port 8000 --workers 4
```

## 小贴士

- `--reload` 会消耗更多资源，生产环境**不要加**
- `--workers 4` 启动 4 个 worker 进程提升并发，`--reload` 和 `--workers` **不能同时用**
- 如果代码入口不叫 `app`（比如叫 `application`），就写 `main:application`
- 指定 ASGI 实例的变量名灵活，不是必须叫 `app`，通常约定 `app` 或 `application`
