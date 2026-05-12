# 为什么选 PostgreSQL

## 对比 MySQL

| 维度 | PostgreSQL | MySQL |
|:----|:----------|:------|
| SQL 标准遵循 | 严格，几乎完全遵循 | 部分宽松，有些语法"方便但不标准" |
| 数据类型 | JSON/JSONB、数组、范围类型、几何类型、自定义类型 | 基础类型为主，JSON 支持较弱 |
| 复杂查询 | 窗口函数、CTE、递归查询、物化视图支持完善 | 8.0 后才加窗口函数，递归 CTE 支持有限 |
| 并发控制 | MVCC 实现更干净，读写不互相阻塞 | 早期用表锁，InnoDB 后才用行锁 |
| 扩展性 | 支持自定义函数、操作符、索引方法、扩展（PostGIS、TimescaleDB） | 扩展生态弱 |
| 全文搜索 | 内置 tsvector/tsquery，不需要外挂 | 需要 Elasticsearch 等外部方案 |
| 一致性 | 默认严格模式，不容易写出脏数据 | 默认宽松，容易写出问题但跑得快 |

**一句话：MySQL 赢在简单好上手，PostgreSQL 赢在功能和正确性。**

## PostgreSQL 的核心优势

### 1. 类型系统强大

```sql
-- 数组
SELECT ARRAY[1,2,3] && ARRAY[2,3,4];  -- 有交集 → true

-- JSONB（二进制 JSON，可索引）
SELECT data->>'name' FROM users WHERE data @> '{"role": "admin"}';

-- 范围类型
SELECT * FROM events WHERE tsrange(start_time, end_time) && tsrange('2024-01-01', '2024-12-31');

-- 枚举、复合类型、自定义域
CREATE TYPE mood AS ENUM ('happy', 'sad', 'neutral');
```

MySQL 的 JSON 存进去就是个文本字段，不能索引查询。PostgreSQL 的 JSONB 可以建 GIN 索引，查询速度接近原生列。

### 2. 查询能力

```sql
-- 窗口函数：每个部门内排名
SELECT name, salary, RANK() OVER (PARTITION BY dept ORDER BY salary DESC)
FROM employees;

-- 递归 CTE：查组织架构树
WITH RECURSIVE org AS (
    SELECT id, name, manager_id, 1 AS depth FROM employees WHERE manager_id IS NULL
    UNION ALL
    SELECT e.id, e.name, e.manager_id, o.depth + 1
    FROM employees e JOIN org o ON e.manager_id = o.id
)
SELECT * FROM org;

-- 物化视图：预计算复杂查询的结果，定期刷新
CREATE MATERIALIZED VIEW monthly_sales AS
SELECT date_trunc('month', order_date) AS month, SUM(amount) FROM orders GROUP BY 1;
REFRESH MATERIALIZED VIEW monthly_sales;
```

### 3. 扩展生态

| 扩展 | 用途 |
|:----|:----|
| PostGIS | 地理空间数据，做附近的人/区域查询 |
| TimescaleDB | 时序数据，IoT/监控场景 |
| pg_trgm | 模糊搜索，"类似 Google 的搜索建议" |
| hstore | 键值对存储 |
| pgvector | 向量相似度搜索，AI 场景 |

装一个扩展就多一种能力，不需要换数据库。

### 4. MVCC 与并发

PostgreSQL 的 MVCC（多版本并发控制）：

- 读操作**不加锁**，不阻塞写
- 写操作**不阻塞读**
- 每行数据有版本号，读到的是事务开始时的快照
- 不需要 undo log，历史版本直接存在行里（vacuum 回收）

对比 MySQL InnoDB：也需要 MVCC，但实现方式不同（undo log），并发高时 undo log 膨胀是常见问题。

### 5. 数据完整性

```sql
-- CHECK 约束
ALTER TABLE products ADD CONSTRAINT price_positive CHECK (price > 0);

-- EXCLUDE 约束（排他约束，MySQL 没有）
-- 例：同一会议室同一时间段不能重复预约
CREATE TABLE bookings (
    room_id INT,
    tsrange tstzrange,
    EXCLUDE USING GIST (room_id WITH =, tsrange WITH &&)
);

-- 外键、唯一、非空、生成列，全部支持
```

## AI 产品为什么用 PostgreSQL

### 核心原因：pgvector 扩展

AI 产品最需要的能力是**向量相似度搜索**（语义搜索、推荐、RAG 检索）。传统做法是引入一个额外的向量数据库（Pinecone、Milvus、Qdrant）。而 pgvector 让 PostgreSQL 直接支持向量运算：

```sql
-- 启用扩展
CREATE EXTENSION IF NOT EXISTS vector;

-- 建表：既有业务字段，又有向量字段
CREATE TABLE documents (
    id SERIAL PRIMARY KEY,
    title TEXT,
    content TEXT,
    metadata JSONB,
    embedding vector(1536)  -- OpenAI embedding 维度
);

-- 建向量索引（HNSW 或 IVFFlat）
CREATE INDEX ON documents USING hnsw (embedding vector_cosine_ops);

-- 语义搜索：找最相似的 5 条
SELECT title, content, 1 - (embedding <=> '[0.1, 0.2, ...]'::vector) AS similarity
FROM documents
ORDER BY embedding <=> '[0.1, 0.2, ...]'::vector
LIMIT 5;
```

**`<=>` 是余弦距离，距离越小越相似。**

### 用在哪些场景

| 场景 | 怎么用 | 用了 PG 的什么 |
|:----|:------|:-------------|
| **RAG（检索增强生成）** | 文档切成 chunk → 生成 embedding 存 PG → 用户提问时向量搜索找到相关 chunk → 喂给 LLM | pgvector 向量搜索 + JSONB 存 metadata |
| **语义搜索** | 用户搜"怎么退款"，不只是关键词匹配，而是理解意图找最相关的文档 | pgvector + 全文搜索 tsvector |
| **推荐系统** | 用户画像向量和商品向量做相似度匹配 | pgvector |
| **图片/音频检索** | 图片/音频转成 embedding 后存入，按相似度搜索 | pgvector |
| **AI Agent 记忆** | Agent 对话历史的 embedding 存储和检索 | pgvector + JSONB 存对话上下文 |

### 为什么不用专门的向量数据库

| 维度 | 专用向量库（Pinecone/Milvus） | PostgreSQL + pgvector |
|:----|:--------------------------|:---------------------|
| 部署 | 额外起一个服务，运维一套系统 | 复用现有 PG，零额外运维 |
| 事务 | 不支持或弱支持 | 完整 ACID |
| 混合查询 | 不行，只能搜向量 | `WHERE category = 'tech' ORDER BY embedding <=> query_vec LIMIT 5` — 先过滤再排，一条 SQL 搞定 |
| 数据一致性 | 需要自己同步 | 业务数据和向量在同一张表，天然一致 |
| 成熟度 | 较新，API 经常变 | PG 几十年验证，pgvector 社区活跃 |

**关键优势：混合查询。**

```sql
-- 专用向量库做不到这个：先用业务条件过滤，再做向量排序
SELECT title, content
FROM documents
WHERE category = 'AI'           -- 传统索引过滤
  AND created_at > '2024-01-01' -- 时间过滤
ORDER BY embedding <=> query_vector  -- 向量排序
LIMIT 10;
```

专用向量库需要先查业务条件 → 拿到 id 列表 → 再去向量库里搜 → 合并结果。PG 一条 SQL 全搞定。

### AI 产品还用 PG 的其他功能

- **JSONB** — 存 LLM 输出（结构不固定）、模型配置、prompt 模板
- **全文搜索** — 关键词搜索 + 向量搜索混合使用，召回率更高
- **CDC（变更数据捕获）** — 数据变了自动触发向量重新生成
- **扩展生态** — pg_trgm 做模糊匹配、PostGIS 做地理位置相关的 AI 推荐

### 谁在用

- **Supabase** — 云数据库平台，底层就是 PG + pgvector，主打 AI 开发者
- **LangChain / LlamaIndex** — 默认向量存储后端之一就是 PG
- **很多早期 AI 创业公司** — 不想维护两套数据库，用 PG 一把梭

## 什么时候用 MySQL

- 读多写少、简单 CRUD 的 Web 应用
- 团队已经熟悉 MySQL 生态
- 需要最简单的部署和运维（MySQL 更轻量）
- 用云服务托管（RDS 等，MySQL 的云支持更成熟）

## 什么时候用 PostgreSQL

- 需要复杂查询（报表、分析、联表多）
- 数据类型丰富（JSON、数组、地理数据、时序数据）
- 数据一致性要求高（金融、交易）
- 需要扩展能力（不想为了一个功能引入额外系统）
- 需要全文搜索但不想上 Elasticsearch

## 一句话总结

**MySQL 是"够用就行"的选择，PostgreSQL 是"做正确的事"的选择。**
