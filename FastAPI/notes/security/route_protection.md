# 路由保护 + 角色权限

## get_current_user — 保护路由的基础

```python
from fastapi import Depends, HTTPException, status
from fastapi.security import OAuth2PasswordBearer
from jose import JWTError, jwt
from sqlalchemy.orm import Session

from app.core.config import settings
from app.core.database import get_db
from app.models.user import User

oauth2_scheme = OAuth2PasswordBearer(tokenUrl="/api/auth/login")

def get_current_user(
    token: str = Depends(oauth2_scheme),
    db: Session = Depends(get_db),
) -> User:
    """从 token 解出 user_id，查库返回用户。"""
    try:
        payload = jwt.decode(token, settings.SECRET_KEY, algorithms=["HS256"])
        user_id = int(payload.get("sub"))
    except (JWTError, ValueError, TypeError):
        raise HTTPException(status_code=401, detail="Invalid token")

    user = db.query(User).filter(User.id == user_id).first()
    if not user:
        raise HTTPException(status_code=401, detail="User not found")
    return user
```

## 保护单个路由

```python
@app.get("/users/me")
def get_me(current_user: User = Depends(get_current_user)):
    return current_user
```

就是这么简单。`Depends(get_current_user)` 放在参数里，FastAPI 自动先跑完鉴权再把用户注入进来。

## 保护整个路由组

```python
router = APIRouter(
    prefix="/admin",
    tags=["admin"],
    dependencies=[Depends(get_current_user)],   # 所有路由先鉴权
)

@router.get("/stats")
def stats():
    return {"data": "secret"}
```

## 角色权限控制（RBAC）

给 User 模型加一个字段：

```python
# models/user.py
class User(Base):
    __tablename__ = "users"
    id = Column(Integer, primary_key=True)
    email = Column(String, unique=True)
    hashed_password = Column(String)
    role = Column(String, default="user")   # "user" 或 "admin"
```

创建依赖函数：

```python
from fastapi import Depends, HTTPException

def require_admin(current_user: User = Depends(get_current_user)):
    if current_user.role != "admin":
        raise HTTPException(status_code=403, detail="Admin access required")
    return current_user
```

使用：

```python
@router.get("/admin/users")
def list_all_users(
    db: Session = Depends(get_db),
    admin: User = Depends(require_admin),    # 非 admin 直接 403
):
    return db.query(User).all()
```

## 可选鉴权

有些接口登录不登录都能访问，但登录后提供更多信息。

不传 token 也能访问，只是 `current_user` 为 `None`：

```python
from fastapi.security import OAuth2PasswordBearer

# 不自动报 401
oauth2_scheme_optional = OAuth2PasswordBearer(tokenUrl="/api/auth/login", auto_error=False)

async def get_optional_user(
    token: str = Depends(oauth2_scheme_optional),
    db: Session = Depends(get_db),
) -> Optional[User]:
    if not token:
        return None
    try:
        payload = jwt.decode(token, settings.SECRET_KEY, algorithms=["HS256"])
        user_id = int(payload.get("sub"))
        return db.query(User).filter(User.id == user_id).first()
    except JWTError:
        return None

@router.get("/items")
def list_items(
    db: Session = Depends(get_db),
    current_user: Optional[User] = Depends(get_optional_user),
):
    items = db.query(Item).all()
    if current_user:
        return {"items": items, "favorites": get_user_favorites(current_user.id)}
    return {"items": items}
```

## 安全相关代码最终放哪

```
app/
├── core/
│   └── security.py        # hash_password, verify_password, create_access_token, decode_token
├── dependencies.py         # get_current_user, require_admin, get_optional_user
├── routers/
│   └── auth.py             # 登录/注册接口（用上面的函数）
```

- `core/security.py` — 纯函数：加密、JWT 签发解码，**不依赖数据库和 FastAPI**
- `dependencies.py` — 用 security 的函数 + FastAPI 的 Depends，**拼装成可注入依赖**
- `routers/auth.py` — 写路由，调 dependencies

---

## 从零实现一次安全的完整步骤

```python
# 1. core/security.py → hash / verify / create token
# 2. models/user.py   → User 表（含 hashed_password, role）
# 3. dependencies.py  → get_db + get_current_user + require_admin
# 4. routers/auth.py  → POST /register + POST /login
# 5. routers/users.py → 用 Depends(get_current_user) 保护路由
```

完成。
