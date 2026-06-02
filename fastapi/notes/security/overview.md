# FastAPI 安全 — 完整流程

```
                    注册 /api/auth/register
客户端 ──────────────────────────────────►  接收明文密码
                                               │
                                               ▼
                                          hash 密码（bcrypt）
                                               │
                                               ▼
                                          存入数据库 users 表
                                               │
                         登录 /api/auth/login   │
客户端 ──────────────────────────────────►   |
                                               ▼
                                       查用户 + verify 密码
                                               │
                                               ▼
                                       生成 JWT（access_token）
                                               │
                                               ▼
                                       返回 token 给客户端
                                               │
                   请求头带 Bearer token         │
客户端 ──────────────────────────────────►   │
                                               ▼
                                       dependencies 中解码 token
                                               │
                                               ▼
                                       查用户 → 注入路由
```

**核心：密码从不存明文，鉴权靠 JWT，每个请求解密一次 token。**

---

## 文件索引

| 文件 | 内容 |
|---|---|
| [password_hashing.md](password_hashing.md) | bcrypt 加密+验密 |
| [jwt_token.md](jwt_token.md) | JWT 签发+验证 |
| [oauth2_scheme.md](oauth2_scheme.md) | FastAPI OAuth2 集成 |
| [route_protection.md](route_protection.md) | 路由保护 + 角色权限 |

按顺序读。
