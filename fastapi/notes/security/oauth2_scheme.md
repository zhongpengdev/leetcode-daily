# OAuth2 集成

FastAPI 内置了 `OAuth2PasswordBearer`，帮你在 `/docs` 页面自动生成"Authorize"按钮。

## OAuth2PasswordBearer

```python
from fastapi.security import OAuth2PasswordBearer

# 告诉 FastAPI：登录接口在 /api/auth/login
oauth2_scheme = OAuth2PasswordBearer(tokenUrl="/api/auth/login")
```

这个对象本身是一个依赖，功能：
- 从请求头提取 `Authorization: Bearer <token>`
- 拿不到 token 时返回 401
- 让 `/docs` 页面显示"Authorize"输入框

## 登录接口

```python
from fastapi import APIRouter, Depends, HTTPException
from fastapi.security import OAuth2PasswordRequestForm
from sqlalchemy.orm import Session

from app.core.security import verify_password, create_access_token
from app.dependencies import get_db
from app.models.user import User

router = APIRouter(prefix="/api/auth", tags=["auth"])

@router.post("/login")
def login(form_data: OAuth2PasswordRequestForm = Depends(), db: Session = Depends(get_db)):
    # 1. 查用户
    user = db.query(User).filter(User.email == form_data.username).first()

    # 2. 验证密码
    if not user or not verify_password(form_data.password, user.hashed_password):
        raise HTTPException(status_code=401, detail="Incorrect email or password")

    # 3. 签发 token
    token = create_access_token({"sub": str(user.id)})
    return {"access_token": token, "token_type": "bearer"}
```

`OAuth2PasswordRequestForm` 自动解析 `username` 和 `password` 两个表单字段（HTML 标准登录格式）。这里的 `form_data.username` 实际传的是 email。

## 测试方法

```bash
curl -X POST http://localhost:8000/api/auth/login \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "username=a@b.com&password=123456"
```

返回：

```json
{
  "access_token": "eyJhbGciOi...",
  "token_type": "bearer"
}
```

---

下一节 → [route_protection.md](route_protection.md)
