# JWT 令牌

JWT（JSON Web Token）是三段式字符串，用于**无状态鉴权**：

```
header.payload.signature
```

服务器签发后不存 session，客户端每次请求带着，服务器验签名即可。

## 安装

```bash
pip install python-jose[cryptography]
```

## 签发 access_token

```python
from datetime import datetime, timedelta
from jose import jwt

SECRET_KEY = "your-secret-key-change-in-production"   # 放 config.py
ALGORITHM = "HS256"
ACCESS_TOKEN_EXPIRE_MINUTES = 30

def create_access_token(data: dict) -> str:
    payload = data.copy()
    expire = datetime.utcnow() + timedelta(minutes=ACCESS_TOKEN_EXPIRE_MINUTES)
    payload.update({"exp": expire})
    return jwt.encode(payload, SECRET_KEY, algorithm=ALGORITHM)
```

通常把用户 ID 放进去：

```python
token = create_access_token({"sub": "42"})   # sub 是 JWT 标准字段：subject
```

## 解码 token

```python
def decode_token(token: str) -> dict:
    try:
        payload = jwt.decode(token, SECRET_KEY, algorithms=[ALGORITHM])
        return payload
    except JWTError:
        raise HTTPException(status_code=401, detail="Invalid token")
```

## 完整流程

```python
# 登录成功 → 签发
user_id = 42
token = create_access_token({"sub": str(user_id)})
return {"access_token": token, "token_type": "bearer"}

# 客户端后续请求带着：
# Authorization: Bearer <token>

# 服务器解密拿到 user_id，查库确认用户存在
```

## 关于 refresh_token（简要）

access_token 有效期短（15-30 分钟），过期后客户端用 refresh_token（有效期长如 7 天）换新的 access_token。

简单项目可以只用一个 7 天有效的 access_token，不用 refresh_token 机制。小项目不需要过度设计。

---

下一节 → [oauth2_scheme.md](oauth2_scheme.md)
