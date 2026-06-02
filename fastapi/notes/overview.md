# FastAPI 概览

```
FastAPI = Starlette（网络） + Pydantic（数据校验）
```

核心优势：自动文档 + 类型校验 + 异步 + 依赖注入

---

## 完整学习路径

```
 ① 00_start.md          从零启动项目   → 安装、第一个文件、运行
 ② dependencies.md      依赖清单       → 每个包装什么、导入什么、干啥用
 ③ 01_routing.md        路由工程化     → 路径/查询参数、APIRouter
 ④ 02_database.md       数据库 + 依赖  → SQLAlchemy、Depends
 ⑤ 04_project_structure.md 项目结构   → DB/Redis/鉴权/业务层布局
 ⑥ database/overview.md 数据库连接     → MySQL / Redis 连接串与 CRUD
 ⑦ security/overview.md 安全鉴权       → bcrypt + JWT + OAuth2
 ⑧ 03_extras.md         补充功能       → CORS、错误处理、文件上传
```

直接从 `00_start.md` 开始，跟着做一遍就能跑起来一个项目。
