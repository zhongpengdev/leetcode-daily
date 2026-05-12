# 向量检索与 pgvector

## 什么是向量（Embedding）

把一段文字、一张图、一段音频，通过模型转换成一串**浮点数数组**，这个数组就是向量。

```
"猫"  →  [0.12, -0.34, 0.78, 0.56, ...]   ← 1536 维（OpenAI embedding）
"小猫" →  [0.11, -0.33, 0.79, 0.55, ...]   ← 和"猫"很接近
"飞机" →  [-0.8, 0.42, 0.03, -0.67, ...]   ← 和"猫"差很远
```

**核心思想：语义相近的内容，向量在空间中距离近。**

## 为什么需要向量检索

传统搜索靠关键词匹配：

```
用户搜："怎么退货"
数据库里写的是："商品不满意时如何申请退款"

关键词匹配：找不到（没有"退货"这个词）
向量匹配：能找到（"退货"和"退款"语义相近，向量距离近）
```

向量检索 = 给定一个查询向量，从海量向量中**找最相似的 K 个**。

## 距离度量

三种常见距离，衡量两个向量有多"像"：

### 余弦距离（Cosine Distance）

```
cosine_distance = 1 - cos(θ)

θ = 0°（方向完全一致）→ cos(θ) = 1 → 距离 = 0（最相似）
θ = 90°（正交）       → cos(θ) = 0 → 距离 = 1
θ = 180°（完全相反）  → cos(θ) = -1 → 距离 = 2（最不相似）
```

**不关心向量长度，只关心方向。** 最常用，OpenAI embedding 默认用这个。

### L2 距离（欧几里得距离）

```
L2 = sqrt((a1-b1)² + (a2-b2)² + ...)
```

**关心绝对距离。** 两个点在空间中实际差多远。

### 内积（Inner Product）

```
ip = -(a1*b1 + a2*b2 + ...)
取负是因为 pgvector 里距离越小排越前
```

**关心方向和长度。** 适合推荐场景（用户偏好向量 × 商品向量，值越大越匹配）。

### pgvector 中的运算符

| 运算符 | 距离类型 | 用法 |
|:------|:-------|:-----|
| `<=>` | 余弦距离 | `ORDER BY embedding <=> query_vec` |
| `<->` | L2 距离 | `ORDER BY embedding <-> query_vec` |
| `<#>` | 内积 | `ORDER BY embedding <#> query_vec` |

## pgvector 存储

### 建表

```sql
CREATE EXTENSION IF NOT EXISTS vector;

CREATE TABLE items (
    id SERIAL PRIMARY KEY,
    content TEXT,
    embedding vector(1536)  -- 固定维度，OpenAI text-embedding-ada-002 是 1536 维
);
```

`vector(1536)` 就是一个长度 1536 的 float4 数组，PostgreSQL 内部用紧凑的二进制格式存储。

### 插入向量

```sql
-- 直接写
INSERT INTO items (content, embedding)
VALUES ('猫很可爱', '[0.12, -0.34, 0.78, ...]'::vector);

-- Python 中插入
import psycopg2
from openai import OpenAI

client = OpenAI()
response = client.embeddings.create(input="猫很可爱", model="text-embedding-ada-002")
vec = response.data[0].embedding  # list of 1536 floats

cur.execute(
    "INSERT INTO items (content, embedding) VALUES (%s, %s)",
    ("猫很可爱", str(vec))  # pgvector 接收字符串形式 '[0.12, -0.34, ...]'
)
```

### 查询

```sql
-- 找最相似的 5 条（余弦距离）
SELECT content, 1 - (embedding <=> %s::vector) AS similarity
FROM items
ORDER BY embedding <=> %s::vector
LIMIT 5;
```

`similarity = 1 - cosine_distance`，值越接近 1 越相似。

## 索引：为什么需要、两种索引

**没有索引时**，每次查询要算**所有行**的距离，全表扫描，数据量大了就慢。索引的作用是**不扫全表也能快速找到最近的向量**。

### HNSW（Hierarchical Navigable Small World）

```
原理：把向量构建成一张多层图，从顶层开始往下导航，像跳表一样快速逼近目标。

高层：稀疏，快速跳转
  ○───────────────○
  │               │
中层：中等密度
  ○─────○─────○───○
  │     │     │   │
底层：密集，所有向量都在
  ○─○─○─○─○─○─○─○─○
```

```sql
-- 建 HNSW 索引（余弦距离）
CREATE INDEX ON items USING hnsw (embedding vector_cosine_ops);

-- L2 距离
CREATE INDEX ON items USING hnsw (embedding vector_l2_ops);

-- 内积
CREATE INDEX ON items USING hnsw (embedding vector_ip_ops);
```

| 特性 | 说明 |
|:----|:----|
| 查询速度 | 快，O(log n) 级别 |
| 建索引速度 | 慢，数据量大时建索引很耗时 |
| 内存占用 | 大，整个图结构要驻留内存 |
| 精度 | 高，默认参数下召回率 > 95% |
| 适用场景 | 数据量中大（百万~千万级），追求查询速度 |

**调参：**

```sql
-- ef_construction：建索引时的搜索宽度，越大精度越高但越慢（默认 64）
-- m：每个节点的邻居数，越大图越密精度越高但内存越大（默认 16）
CREATE INDEX ON items USING hnsw (embedding vector_cosine_ops)
WITH (m = 16, ef_construction = 200);

-- 查询时调精度
SET hnsw.ef_search = 100;  -- 默认 40，越大越精确但越慢
```

### IVFFlat（Inverted File with Flat Compression）

```
原理：先把所有向量用 k-means 聚类成若干组（类比分桶），查询时只搜最近的几个桶。

桶1: [v1, v5, v8, ...]     ← 中心点 c1
桶2: [v2, v6, v9, ...]     ← 中心点 c2
桶3: [v3, v4, v7, ...]     ← 中心点 c3

查询向量 → 离 c2 最近 → 只搜桶2里的向量
```

```sql
-- 先插入数据，再建索引（IVFFlat 需要数据来聚类）
CREATE INDEX ON items USING ivfflat (embedding vector_cosine_ops)
WITH (lists = 100);  -- 分 100 个桶
```

| 特性 | 说明 |
|:----|:----|
| 查询速度 | 中等 |
| 建索引速度 | 快 |
| 内存占用 | 小 |
| 精度 | 较低，只搜部分桶可能漏掉近邻 |
| 适用场景 | 数据量小（< 百万），或对精度要求不高 |

**调参：**

```sql
-- lists：桶数量，建议 rows / 1000（数据量 10 万 → lists = 100）
-- probes：查询时搜几个桶，默认 1，越大越精确越慢
SET ivfflat.probes = 10;
```

### 选哪个

| 数据量 | 推荐 |
|:------|:----|
| < 10 万 | 不需要索引，直接暴力搜索就够快 |
| 10 万 ~ 100 万 | IVFFlat，建索引快 |
| > 100 万 | HNSW，查询速度快 |
| > 1 亿 | 考虑分区表 + HNSW，或专用向量库 |

## 完整流程示例

```python
import psycopg2
from openai import OpenAI

client = OpenAI()
conn = psycopg2.connect(dbname="testdb", user="postgres", password="xxx")
cur = conn.cursor()

# 1. 建表
cur.execute("""
    CREATE TABLE IF NOT EXISTS docs (
        id SERIAL PRIMARY KEY,
        content TEXT,
        embedding vector(1536)
    )
""")
conn.commit()

# 2. 插入文档（实际中批量处理）
texts = ["PostgreSQL 很强大", "向量搜索是什么", "Python 连接数据库"]
for text in texts:
    resp = client.embeddings.create(input=text, model="text-embedding-ada-002")
    vec = resp.data[0].embedding
    cur.execute("INSERT INTO docs (content, embedding) VALUES (%s, %s)", (text, str(vec)))
conn.commit()

# 3. 建索引（数据量小可以跳过这步）
cur.execute("CREATE INDEX ON docs USING hnsw (embedding vector_cosine_ops)")
conn.commit()

# 4. 搜索
query = "数据库怎么用"
resp = client.embeddings.create(input=query, model="text-embedding-ada-002")
query_vec = resp.data[0].embedding

cur.execute("""
    SELECT content, 1 - (embedding <=> %s::vector) AS similarity
    FROM docs
    ORDER BY embedding <=> %s::vector
    LIMIT 3
""", (str(query_vec), str(query_vec)))

for row in cur.fetchall():
    print(f"{row[0]}  (相似度: {row[1]:.3f})")

cur.close()
conn.close()
```

## 从一本书到向量数据库：完整流程

拿到一本书（PDF/TXT/DOCX），怎么变成可搜索的向量库？

```
原始文档 → 读取 → 切块 → 生成 embedding → 存入 PG → 建索引 → 可搜索
```

### 第一步：读取文档

```python
# PDF
from PyPDF2 import PdfReader

reader = PdfReader("book.pdf")
text = ""
for page in reader.pages:
    text += page.extract_text() + "\n"

# TXT 更简单
with open("book.txt", "r") as f:
    text = f.read()

# DOCX
from docx import Document
doc = Document("book.docx")
text = "\n".join([p.text for p in doc.paragraphs])
```

### 第二步：切块（Chunking）

**为什么不能整本书丢进去？**

1. Embedding 模型有 token 上限（OpenAI ada-002 是 8192 token，约 6000 个汉字）
2. 一个向量代表一段话的意思，整本书的向量太笼统，搜索精度极差
3. 返回给 LLM 时，只需要相关的几段，不需要整本书

**切块策略：**

```
按固定长度切（最简单，推荐起步）：

原文：[第1章 intro...800字...][第2章 setup...1200字...][第3章 usage...600字...]

chunk_size=500, overlap=50（每个块 500 字，块之间重叠 50 字）：

chunk1: "...第1章 intro...前500字..."
chunk2: "...450字到950字..."     ← 和 chunk1 重叠 50 字
chunk3: "...900字到1400字..."    ← 和 chunk2 重叠 50 字
```

**重叠的意义：** 防止关键信息被切到两个块的边界，导致哪个块都不完整。

```python
def chunk_text(text, chunk_size=500, overlap=50):
    """按字符数切块，带重叠"""
    chunks = []
    start = 0
    while start < len(text):
        end = start + chunk_size
        chunks.append(text[start:end])
        start = end - overlap  # 回退 overlap 字符，实现重叠
    return chunks

chunks = chunk_text(text, chunk_size=500, overlap=50)
print(f"切成了 {len(chunks)} 块")
```

**更智能的切法（按段落/章节）：**

```python
def chunk_by_paragraph(text, max_size=1000):
    """按段落切，但不超过 max_size"""
    paragraphs = text.split("\n\n")  # 双换行分段
    chunks = []
    current = ""

    for para in paragraphs:
        if len(current) + len(para) < max_size:
            current += para + "\n\n"
        else:
            if current:
                chunks.append(current.strip())
            current = para + "\n\n"

    if current:
        chunks.append(current.strip())
    return chunks
```

**按段落切的好处：** 语义更完整，不会把一句话从中间切断。

### 第三步：生成 Embedding

```python
from openai import OpenAI
client = OpenAI()

def get_embeddings(texts, batch_size=100):
    """批量生成 embedding，OpenAI 支持一次传多条"""
    all_embeddings = []
    for i in range(0, len(texts), batch_size):
        batch = texts[i:i + batch_size]
        resp = client.embeddings.create(input=batch, model="text-embedding-ada-002")
        all_embeddings.extend([d.embedding for d in resp.data])
    return all_embeddings

embeddings = get_embeddings(chunks)
print(f"生成了 {len(embeddings)} 个向量，每个 {len(embeddings[0])} 维")
```

**批量调 API 比一条条调快得多，也省钱。**

### 第四步：存入 PostgreSQL

```python
import psycopg2

conn = psycopg2.connect(dbname="testdb", user="postgres", password="xxx")
cur = conn.cursor()

# 建表：内容 + 向量 + 元数据
cur.execute("""
    CREATE TABLE IF NOT EXISTS book_chunks (
        id SERIAL PRIMARY KEY,
        book_name TEXT NOT NULL,
        chunk_index INT NOT NULL,
        content TEXT NOT NULL,
        metadata JSONB,
        embedding vector(1536)
    )
""")
conn.commit()

# 批量插入
for i, (chunk, vec) in enumerate(zip(chunks, embeddings)):
    cur.execute(
        """INSERT INTO book_chunks (book_name, chunk_index, content, metadata, embedding)
           VALUES (%s, %s, %s, %s, %s)""",
        ("数据结构与算法", i, chunk, '{"source": "book.pdf", "page": ' + str(i // 3) + '}', str(vec))
    )
conn.commit()
```

**metadata 存什么：** 来源文件名、页码、章节标题、创建时间。搜索结果返回时带上这些信息，用户才知道出处。

### 第五步：建索引

```sql
-- 数据全部插完后再建索引
CREATE INDEX ON book_chunks USING hnsw (embedding vector_cosine_ops);

-- 如果还按书名过滤，加个组合索引
CREATE INDEX ON book_chunks (book_name);
```

### 第六步：搜索

```python
query = "什么是堆排序"
resp = client.embeddings.create(input=query, model="text-embedding-ada-002")
query_vec = resp.data[0].embedding

cur.execute("""
    SELECT book_name, chunk_index, content,
           1 - (embedding <=> %s::vector) AS similarity
    FROM book_chunks
    WHERE book_name = %s                    -- 只搜某本书
    ORDER BY embedding <=> %s::vector
    LIMIT 5
""", (str(query_vec), "数据结构与算法", str(query_vec)))

for row in cur.fetchall():
    print(f"[{row[0]} 第{row[1]}块] 相似度: {row[3]:.3f}")
    print(row[2][:100])  # 打印前 100 字
    print()
```

### 完整流程图

```
┌─────────────┐
│  book.pdf   │
└──────┬──────┘
       │ 1. PyPDF2 读取
       ▼
┌─────────────────────┐
│  整本书的纯文本       │
└──────┬──────────────┘
       │ 2. 按段落/固定长度切块
       ▼
┌─────┬─────┬─────┬─────┐
│chunk│chunk│chunk│chunk│  ...
│  1  │  2  │  3  │  4  │
└──┬──┴──┬──┴──┬──┴──┬──┘
   │     │     │     │  3. OpenAI API 批量生成 embedding
   ▼     ▼     ▼     ▼
 [vec1] [vec2] [vec3] [vec4]
   │     │     │     │  4. INSERT INTO book_chunks
   ▼     ▼     ▼     ▼
┌──────────────────────────────┐
│  PostgreSQL + pgvector        │
│  book_chunks 表               │
│  ┌────┬───────┬────────────┐ │
│  │ id │content│ embedding  │ │
│  ├────┼───────┼────────────┤ │
│  │  1 │ chunk1│ [0.12,...] │ │
│  │  2 │ chunk2│ [0.34,...] │ │
│  │... │ ...   │ ...        │ │
│  └────┴───────┴────────────┘ │
│  + HNSW 索引                 │
└──────────────────────────────┘
       │
       │  用户提问 → embedding → 向量搜索
       ▼
  返回最相关的 5 个 chunk → 喂给 LLM → 生成回答
```

### 实际工程中还会做的事

| 问题 | 解法 |
|:----|:-----|
| PDF 有表格/图片 | 用 OCR（Tesseract）或专门的 PDF 解析库（unstructured） |
| 中文效果差 | 用中文 embedding 模型（bge-large-zh、text2vec-chinese） |
| 切块太碎，上下文丢失 | 用 parent-child chunk：小块搜，返回时带上父块（更大的上下文） |
| 数据更新了 | 根据 source + chunk_index 找到旧记录，删掉重新插入 |
| 多本书 | book_name 字段过滤，或按书建分区表 |
| 要返回给 LLM | 搜索到的 chunk + 用户问题 一起拼成 prompt |
