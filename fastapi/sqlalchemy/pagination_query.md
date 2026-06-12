# 分页查询

## 偏移量分页查询

```sql
-- 第1页：每页20条
SELECT * FROM videos 
ORDER BY create_time DESC 
LIMIT 20 OFFSET 0;   -- OFFSET 0 可省略

-- 第2页
SELECT * FROM videos 
ORDER BY create_time DESC 
LIMIT 20 OFFSET 20;  -- 跳过前20条
```

标准分页返回:

```json
{
  "code": 200,
  "msg": "success",
  "data": {
    "list": [
      {"id": 1, "name": "Item 1"},
      {"id": 2, "name": "Item 2"}
    ],
    "pagination": {
      "total": 100,
      "page": 2,
      "size": 10,
      "total_pages": 10,
      "has_next": true,
      "has_prev": true
    }
  }
}
```

```python
def get_sessions_paginated(user_id: str, page: int = 1, page_size: int = 10):
    db = SessionLocal()
    try:
        start = (page - 1) * page_size # 当前分页头
        
        # Count total
        total_count = db.query(SessionModel).filter(SessionModel.user_id == str(user_id)).count()

        sessions = (
            db.query(SessionModel)
            .filter(SessionModel.user_id == str(user_id))
            .order_by(SessionModel.created_at.desc())
            .offset(start)
            .limit(page_size)
            .all()
        )
        
        items = [{
            "id": s.id,
            "user_id": s.user_id,
            "title": s.title,
            "created_at": s.created_at.isoformat() if s.created_at else None
        } for s in sessions]
        
        return {
            "items": items,
            "total": total_count
        }
    finally:
        db.close()
```

# 原理

`LIMIT`顺序扫描了所有的行实现计数，只是没有返回。

分页深度越大，读取的行数就越多，读取行数/扫描行数比例就越大，浪费多

**深分页查询慢**

解决方案

1. 游标查询
2. 延迟关联：子查询只走覆盖索引（id），避免回表。
   
```sql
   -- 慢：深分页
SELECT * FROM users ORDER BY id LIMIT 100000, 10;

-- 快：先取主键，再关联
SELECT * FROM users u
INNER JOIN (
    SELECT id FROM users ORDER BY id LIMIT 100000, 10
) t ON u.id = t.id;
```

时间复杂度：$$O(offset + limit)$$

1. 按 ORDER BY 排序（如果有索引则走索引顺序）
2. 逐行扫描，计数
3. 跳过前 100000 行（不返回）
4. 继续扫描，取 10 行返回
5. 停止

## 为什么OFFSET不能直接跳到对应offset位置

1. B+树记录原理
2. 行可能被删除/插入：第5000行的物理位置可能变化

## 游标分页查询

适合无限滚动形式（抖音）

```sql
SELECT * FROM items ORDER BY id LIMIT 10;
```

前端直接传进来最后一条记录的id作为游标，假设是100：

```sql
SELECT * FROM items WHERE id > 100 ORDER BY id LIMIT 10;
```



