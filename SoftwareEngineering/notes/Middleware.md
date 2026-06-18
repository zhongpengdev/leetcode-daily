# Middleware - 中间件模式

## 什么是 Middleware

中间件是在请求/响应链路上的**一个个"关卡"**，请求依次穿过每个中间件，响应原路返回。

```
请求 → [日志] → [鉴权] → [限流] → [业务处理] → [限流] → [鉴权] → [日志] → 响应
```

本质上就是 **AOP 在 HTTP 层的具体实现**。

## Go 代码示例

```go
// 中间件类型
type Middleware func(http.Handler) http.Handler

// 日志中间件
func Logging(next http.Handler) http.Handler {
    return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
        start := time.Now()
        next.ServeHTTP(w, r)
        log.Printf("%s %s %v", r.Method, r.URL.Path, time.Since(start))
    })
}

// 鉴权中间件
func Auth(next http.Handler) http.Handler {
    return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
        token := r.Header.Get("Authorization")
        if token == "" {
            http.Error(w, "unauthorized", 401)
            return
        }
        next.ServeHTTP(w, r)
    })
}

// 链式组合
func Chain(handler http.Handler, middlewares ...Middleware) http.Handler {
    for i := len(middlewares) - 1; i >= 0; i-- {
        handler = middlewares[i](handler)
    }
    return handler
}

// 使用
mux := http.NewServeMux()
mux.Handle("/api", Chain(
    http.HandlerFunc(apiHandler),
    Logging,
    Auth,
))
```

## 属于哪个层面

**表现层（Handler 层）** 的横切关注点处理。是 AOP 模式在 HTTP 场景的典型应用。

## 最佳实践

1. **中间件只做一件事**：日志、鉴权、限流、CORS 分开写
2. **顺序很重要**：通常先日志，再鉴权，再业务
3. **错误时 short-circuit**：鉴权失败直接返回，不往下走
4. **全局 vs 路由级**：有的中间件全局挂载，有的只挂特定路由

## 一句话总结

> **中间件是 HTTP 请求链路上的关卡，每个关卡只做一件事。本质是表现层的 AOP。**
