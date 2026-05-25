// ============================================================
// WebSocket 客户端 —— TypeScript 实现
// 编译: tsc client.ts --outDir dist
// ============================================================

/** 消息协议定义 */
interface WsMessage {
  type: "chat" | "system" | "get_online" | "online_list";
  content?: string;
  from?: number;
  target?: number;
  time?: string;
  online?: number[];
  users?: number[];
}

type MessageHandler = (msg: WsMessage) => void;

class ChatClient {
  private ws: WebSocket;
  private handlers = new Map<string, Set<MessageHandler>>();

  constructor(url: string) {
    this.ws = new WebSocket(url);
    this.ws.onmessage = (event) => this.dispatch(JSON.parse(event.data));
    this.ws.onopen = () => console.log("[ws] connected");
    this.ws.onclose = () => console.log("[ws] disconnected");
  }

  /** 订阅某种消息类型，返回取消订阅函数 */
  on(type: string, handler: MessageHandler): () => void {
    if (!this.handlers.has(type)) {
      this.handlers.set(type, new Set());
    }
    this.handlers.get(type)!.add(handler);
    return () => this.handlers.get(type)?.delete(handler);
  }

  /** 发送消息 */
  send(msg: WsMessage): void {
    if (this.ws.readyState === WebSocket.OPEN) {
      this.ws.send(JSON.stringify(msg));
    }
  }

  /** 发送聊天消息 */
  chat(content: string, target?: number): void {
    this.send({ type: "chat", content, target });
  }

  /** 请求在线用户列表 */
  getOnline(): void {
    this.send({ type: "get_online" });
  }

  /** 内部：分发消息到对应的 handler */
  private dispatch(msg: WsMessage): void {
    this.handlers.get(msg.type)?.forEach((fn) => fn(msg));
    // 同时通知通配符监听者
    this.handlers.get("*")?.forEach((fn) => fn(msg));
  }
}
