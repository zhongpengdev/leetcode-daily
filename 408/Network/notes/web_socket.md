# WebSocket 完全指南

## 一、WebSocket 是什么？

WebSocket 是一种**全双工通信协议**，建立在单个 TCP 连接之上。它解决了 HTTP 的核心痛点：**服务器无法主动向客户端推送数据**。

### HTTP vs WebSocket

| 特性 | HTTP | WebSocket |
|------|------|-----------|
| 通信模式 | 半双工（请求-响应） | 全双工（双向实时） |
| 连接生命周期 | 每次请求新建连接（HTTP/1.1 可复用但本质仍是短连接） | 一次握手，长期保持 |
| 头部开销 | 每次请求 ~200-800B headers | 数据帧仅 2-14B 开销 |
| 服务端推送 | 不原生支持（需要轮询/SSE） | 原生支持 |
| 适用场景 | REST API、页面加载 | 实时聊天、游戏、行情推送 |

### 一句话理解

> HTTP 像发短信——你发一条，等对方回一条。WebSocket 像打电话——接通后双方随时都能说话。

---

## 二、握手过程（Handshake）

WebSocket 连接从一个 HTTP 请求开始，这就是所谓的**协议升级（Protocol Upgrade）**：

```
客户端 → 服务器（HTTP 请求）:
GET /chat HTTP/1.1
Host: example.com
Upgrade: websocket          ← 告诉服务器"我想升级为 WebSocket"
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==   ← 随机 Base64 编码的 16 字节
Sec-WebSocket-Version: 13

服务器 → 客户端（HTTP 响应）:
HTTP/1.1 101 Switching Protocols   ← 101 表示协议切换成功
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=  ← 对 Key 做运算后的结果
```

握手成功后，底层 TCP 连接不关闭，后续所有通信都走 WebSocket 帧（frame），不再经过 HTTP。

---

## 三、实际应用场景

1. **即时通讯** — 微信网页版、Slack、Discord
2. **实时数据推送** — 股票行情、加密货币价格、体育比分
3. **协同编辑** — Google Docs、Notion 的多人实时编辑
4. **在线游戏** — 多人对战游戏的状态同步
5. **IoT 设备监控** — 实时仪表盘、传感器数据流
6. **通知系统** — 站内消息、订单状态变更推送

---

## 四、后端实现（Python）

### 4.1 使用 `websockets` 库（推荐）

```bash
pip install websockets
```

### 4.2 基础 WebSocket 服务器

```python
import asyncio
import websockets

# 存储所有连接的客户端
connected_clients = set()

async def handler(websocket):
    """处理单个客户端连接"""
    connected_clients.add(websocket)
    try:
        async for message in websocket:
            print(f"收到消息: {message}")
            # 广播给所有客户端
            for client in connected_clients:
                await client.send(f"广播: {message}")
    finally:
        connected_clients.remove(websocket)

async def main():
    async with websockets.serve(handler, "localhost", 8765):
        print("WebSocket 服务器运行在 ws://localhost:8765")
        await asyncio.Future()  # 永远运行

if __name__ == "__main__":
    asyncio.run(main())
```

### 4.3 使用 FastAPI 的 WebSocket（更贴近实际项目）

```bash
pip install fastapi uvicorn
```

```python
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse

app = FastAPI()

# 连接管理器 —— 管理多客户端
class ConnectionManager:
    def __init__(self):
        self.active_connections: list[WebSocket] = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()  # 接受握手
        self.active_connections.append(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def send_personal(self, message: str, websocket: WebSocket):
        """私聊：只发给指定客户端"""
        await websocket.send_text(message)

    async def broadcast(self, message: str):
        """广播：发给所有客户端"""
        for connection in self.active_connections:
            await connection.send_text(message)

manager = ConnectionManager()

@app.websocket("/ws/{client_id}")
async def websocket_endpoint(websocket: WebSocket, client_id: int):
    await manager.connect(websocket)
    try:
        while True:
            data = await websocket.receive_text()  # 阻塞等待客户端消息
            await manager.broadcast(f"客户端 {client_id}: {data}")
    except WebSocketDisconnect:
        manager.disconnect(websocket)
        await manager.broadcast(f"客户端 {client_id} 已断开")

# 带认证的 WebSocket
from fastapi import Header, Query

@app.websocket("/ws/auth")
async def authenticated_ws(
    websocket: WebSocket,
    token: str = Query(...)  # 通过 query param 传 token
):
    # 验证 token（实际项目中用 JWT 等）
    if not is_valid_token(token):
        await websocket.close(code=4001, reason="认证失败")
        return

    await manager.connect(websocket)
    try:
        while True:
            data = await websocket.receive_json()  # 接收 JSON
            # 处理业务逻辑...
            await manager.send_personal({"status": "ok", "echo": data}, websocket)
    except WebSocketDisconnect:
        manager.disconnect(websocket)

def is_valid_token(token: str) -> bool:
    # 实际项目：JWT 验证、数据库查询等
    return token == "secret-token"
```

### 4.4 处理二进制数据

```python
@app.websocket("/ws/binary")
async def binary_ws(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_bytes()  # 接收二进制
        # 例如：处理图片、protobuf 等
        await websocket.send_bytes(data)  # 回传二进制
```

---

## 五、前端实现（TypeScript）

### 5.1 浏览器原生 WebSocket API

```typescript
class WebSocketClient {
  private ws: WebSocket | null = null;
  private url: string;
  private reconnectAttempts = 0;
  private maxReconnectAttempts = 5;
  private reconnectInterval = 3000;

  // 事件回调
  onMessage: (data: string) => void = () => {};
  onOpen: () => void = () => {};
  onClose: (event: CloseEvent) => void = () => {};
  onError: (event: Event) => void = () => {};

  constructor(url: string) {
    this.url = url;
  }

  connect(): void {
    this.ws = new WebSocket(this.url);

    this.ws.onopen = () => {
      console.log("WebSocket 连接已建立");
      this.reconnectAttempts = 0; // 重置重连计数
      this.onOpen();
    };

    this.ws.onmessage = (event: MessageEvent) => {
      this.onMessage(event.data);
    };

    this.ws.onclose = (event: CloseEvent) => {
      console.log("WebSocket 连接关闭", event.code, event.reason);
      this.onClose(event);
      this.reconnect(); // 自动重连
    };

    this.ws.onerror = (event: Event) => {
      console.error("WebSocket 错误", event);
      this.onError(event);
    };
  }

  send(data: string | object): void {
    if (this.ws?.readyState === WebSocket.OPEN) {
      const payload = typeof data === "string" ? data : JSON.stringify(data);
      this.ws.send(payload);
    } else {
      console.warn("WebSocket 未连接，无法发送");
    }
  }

  disconnect(): void {
    this.reconnectAttempts = this.maxReconnectAttempts; // 阻止自动重连
    this.ws?.close();
  }

  private reconnect(): void {
    if (this.reconnectAttempts < this.maxReconnectAttempts) {
      this.reconnectAttempts++;
      console.log(`第 ${this.reconnectAttempts} 次重连...`);
      setTimeout(() => this.connect(), this.reconnectInterval);
    } else {
      console.error("达到最大重连次数，停止重连");
    }
  }
}

// 使用
const client = new WebSocketClient("ws://localhost:8765");

client.onMessage = (data) => {
  console.log("收到消息:", data);
};

client.connect();
client.send("Hello WebSocket!");
client.send({ type: "chat", content: "用 JSON 发送" });
```

### 5.2 带类型的 WebSocket 封装（实际项目常用）

```typescript
// types/ws.ts
interface WsMessage<T = unknown> {
  type: string;
  payload: T;
}

interface ChatPayload {
  userId: number;
  content: string;
  timestamp: number;
}

interface NotificationPayload {
  title: string;
  body: string;
}

// TypedWebSocketClient.ts
class TypedWebSocketClient {
  private ws: WebSocket;
  private handlers = new Map<string, Set<(payload: unknown) => void>>();

  constructor(url: string) {
    this.ws = new WebSocket(url);
    this.ws.onmessage = (event) => {
      const msg: WsMessage = JSON.parse(event.data);
      const handlerSet = this.handlers.get(msg.type);
      if (handlerSet) {
        handlerSet.forEach((fn) => fn(msg.payload));
      }
    };
  }

  on<T>(type: string, handler: (payload: T) => void): () => void {
    if (!this.handlers.has(type)) {
      this.handlers.set(type, new Set());
    }
    this.handlers.get(type)!.add(handler as (payload: unknown) => void);

    // 返回取消订阅函数
    return () => {
      this.handlers.get(type)?.delete(handler as (payload: unknown) => void);
    };
  }

  send<T>(type: string, payload: T): void {
    this.ws.send(JSON.stringify({ type, payload }));
  }
}

// 使用
const ws = new TypedWebSocketClient("ws://localhost:8000/ws/1");

// 按类型订阅，自动获得类型提示
const unsub = ws.on<ChatPayload>("chat", (msg) => {
  console.log(msg.userId, msg.content); // msg 有完整类型
});

ws.send<ChatPayload>("chat", {
  userId: 1,
  content: "你好",
  timestamp: Date.now(),
});
```

### 5.3 React 中的使用

```tsx
import { useEffect, useRef, useCallback, useState } from "react";

function useWebSocket(url: string) {
  const wsRef = useRef<WebSocket | null>(null);
  const [messages, setMessages] = useState<string[]>([]);
  const [isConnected, setIsConnected] = useState(false);

  useEffect(() => {
    const ws = new WebSocket(url);

    ws.onopen = () => setIsConnected(true);
    ws.onclose = () => setIsConnected(false);
    ws.onmessage = (event) => {
      setMessages((prev) => [...prev, event.data]);
    };

    wsRef.current = ws;
    return () => ws.close();
  }, [url]);

  const send = useCallback((data: string) => {
    wsRef.current?.send(data);
  }, []);

  return { messages, send, isConnected };
}

// 组件中使用
function ChatRoom() {
  const { messages, send, isConnected } = useWebSocket("ws://localhost:8000/ws/1");

  return (
    <div>
      <p>状态: {isConnected ? "已连接" : "断开"}</p>
      <ul>
        {messages.map((msg, i) => <li key={i}>{msg}</li>)}
      </ul>
      <button onClick={() => send("你好")}>发送</button>
    </div>
  );
}
```

---

## 六、心跳机制（Heartbeat / Keep-Alive）

WebSocket 连接可能"假死"——TCP 连接还在但实际已断（比如网络切换、防火墙超时）。心跳机制用来检测这种状态。

```
客户端 ──── ping ────→ 服务器
客户端 ←── pong ───── 服务器

如果客户端连续 N 次没收到 pong，就认为连接断开，触发重连。
```

### Python 端

```python
import asyncio
import websockets

async def handler(websocket):
    try:
        # 每 30 秒发送 ping
        async for message in websocket:
            # websockets 库自动处理 ping/pong
            await websocket.send(f"echo: {message}")
    except websockets.ConnectionClosed:
        print("客户端断开")

async def main():
    async with websockets.serve(
        handler,
        "localhost",
        8765,
        ping_interval=30,   # 每 30 秒发一次 ping
        ping_timeout=10,    # 10 秒内没 pong 就断开
    ):
        await asyncio.Future()

asyncio.run(main())
```

### TypeScript 端

```typescript
class HeartbeatClient {
  private ws: WebSocket;
  private heartbeatTimer: ReturnType<typeof setInterval> | null = null;
  private pongTimeout: ReturnType<typeof setTimeout> | null = null;
  private heartbeatInterval = 30000; // 30 秒
  private pongTimeoutMs = 10000;

  constructor(url: string) {
    this.ws = new WebSocket(url);
    this.ws.onopen = () => this.startHeartbeat();
    this.ws.onclose = () => this.stopHeartbeat();
  }

  private startHeartbeat(): void {
    this.heartbeatTimer = setInterval(() => {
      if (this.ws.readyState === WebSocket.OPEN) {
        this.ws.send(JSON.stringify({ type: "ping" }));

        // 等待 pong，超时则重连
        this.pongTimeout = setTimeout(() => {
          console.error("心跳超时，断开重连");
          this.ws.close();
        }, this.pongTimeoutMs);
      }
    }, this.heartbeatInterval);
  }

  // 收到 pong 时调用
  onPong(): void {
    if (this.pongTimeout) {
      clearTimeout(this.pongTimeout);
      this.pongTimeout = null;
    }
  }

  private stopHeartbeat(): void {
    if (this.heartbeatTimer) clearInterval(this.heartbeatTimer);
    if (this.pongTimeout) clearTimeout(this.pongTimeout);
  }
}
```

---

## 七、常见问题与最佳实践

### 7.1 认证

WebSocket 握手阶段**不支持自定义 Header**（浏览器限制），常见方案：

- **Query Parameter**: `ws://host/ws?token=xxx`
- **先 HTTP 认证拿 ticket，再用 ticket 建立连接**
- **连接建立后发送认证消息，服务端验证后再开始业务通信**

### 7.2 消息格式

推荐使用 **JSON**，结构清晰、前后端通用。对性能要求高时可用 **Protocol Buffers** 或 **MessagePack**。

### 7.3 连接数限制

浏览器对同一域名的 WebSocket 连接数限制通常是 **6 个**（Chrome）。如果需要多个频道，考虑复用一条连接，用消息类型区分。

### 7.4 负载均衡

WebSocket 是有状态的长连接，传统轮询负载均衡会出问题。解决方案：

- **Sticky Session（会话保持）**: 通过 cookie 或 IP hash 把同一客户端路由到同一服务器
- **Redis Pub/Sub**: 后端多实例之间通过 Redis 广播消息

### 7.5 断线重连策略

```
重连间隔 = min(初始间隔 * 2^重连次数 + 随机抖动, 最大间隔)

示例: 1s → 2s → 4s → 8s → 16s → 32s (封顶)
```

指数退避 + 随机抖动（jitter）避免所有客户端同时重连造成"惊群效应"。

### 7.6 安全

- **生产环境必须用 `wss://`**（WebSocket over TLS），不是 `ws://`
- 验证 `Origin` 头防止跨站 WebSocket 劫持
- 对消息做输入校验和大小限制，防止 DoS
- Token 设置过期时间，定期刷新

---

## 八、WebSocket vs SSE vs 长轮询

| 特性 | WebSocket | SSE (Server-Sent Events) | 长轮询 (Long Polling) |
|------|-----------|--------------------------|----------------------|
| 方向 | 双向 | 仅服务端→客户端 | 模拟推送 |
| 协议 | ws/wss | HTTP | HTTP |
| 浏览器支持 | 所有现代浏览器 | 所有现代浏览器 | 所有浏览器 |
| 自动重连 | 需自己实现 | 浏览器自动实现 | 需自己实现 |
| 数据格式 | 文本/二进制 | 仅文本 | 任意 |
| 适用场景 | 聊天、游戏、协同 | 新闻推送、通知 | 兼容性兜底 |

**选型建议**：
- 只需要服务端推送 → **SSE**（更简单，天然走 HTTP/2）
- 需要双向实时通信 → **WebSocket**
- 需要兼容老旧环境 → **长轮询**
