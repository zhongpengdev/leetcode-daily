# 密码加密 — bcrypt

密码绝对不能存明文。用 bcrypt 做不可逆哈希。

## 安装

```bash
pip install passlib[bcrypt]
```

## hash 密码

```python
from passlib.context import CryptContext

# 全局单例，指定加密算法
pwd_context = CryptContext(schemes=["bcrypt"])

def hash_password(password: str) -> str:
    return pwd_context.hash(password)
```

每次 hash 结果不一样（自动加盐），但验密时都能通过。

```python
hash_password("123456")
# 每次不同：$2b$12$LJ3m8x...  /  $2b$12$kR9t2a...
```

## 验证密码

```python
def verify_password(plain_password: str, hashed_password: str) -> bool:
    return pwd_context.verify(plain_password, hashed_password)
```

## 完整用法

```python
# 注册时
hashed = hash_password("123456")
db_user = User(email="a@b.com", hashed_password=hashed)
db.add(db_user)

# 登录时
user = db.query(User).filter(User.email == email).first()
if not user or not verify_password(password, user.hashed_password):
    raise HTTPException(status_code=401)
# 验证通过 → 发 JWT
```

## 为什么用 bcrypt 不用 MD5/SHA256

MD5 和 SHA256 是**哈希函数**，设计目标是"快"。bcrypt 是**密码哈希函数**，设计目标是"慢"——暴力破解成本高十倍百倍。

---

下一节 → [jwt_token.md](jwt_token.md)
