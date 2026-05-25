"""
WebSocket 示例服务端
运行: pip install fastapi uvicorn && python server.py
"""

from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.staticfiles import StaticFiles
from fastapi.responses import FileResponse
import uvicorn
import json
from datetime import datetime


app = FastAPI()


# ============================================================
# 连接管理器 —— 唯一职责：管理所有客户端连接
# ============================================================
class ConnectionManager:
    def __init__(self):
        self._connections: dict[int, WebSocket] = {}  # {user_id: websocket}

    async def connect(self, user_id: int, websocket: WebSocket):
        await websocket.accept()
        self._connections[user_id] = websocket

    def disconnect(self, user_id: int):
        self._connections.pop(user_id, None)

    async def send_to(self, user_id: int, message: dict):
        """发给指定用户"""
        ws = self._connections.get(user_id)
        if ws:
            await ws.send_json(message)

    async def broadcast(self, message: dict, exclude: int | None = None):
        """广播给所有人（可排除某用户）"""
        for uid, ws in self._connections.items():
            if uid != exclude:
                await ws.send_json(message)

    @property
    def online_users(self) -> list[int]:
        return list(self._connections.keys())


manager = ConnectionManager()


# ============================================================
# WebSocket 端点 —— 唯一职责：收发消息 + 调度
# ============================================================
@app.websocket("/ws/{user_id}")
async def websocket_endpoint(websocket: WebSocket, user_id: int):
    await manager.connect(user_id, websocket)

    # 通知所有人：某用户上线
    await manager.broadcast({
        "type": "system",
        "content": f"用户 {user_id} 已上线",
        "online": manager.online_users,
    })

    try:
        while True:
            raw = await websocket.receive_text()
            msg = json.loads(raw)
            await handle_message(user_id, msg)

    except WebSocketDisconnect:
        manager.disconnect(user_id)
        await manager.broadcast({
            "type": "system",
            "content": f"用户 {user_id} 已下线",
            "online": manager.online_users,
        })


async def handle_message(sender_id: int, msg: dict):
    """消息路由 —— 根据 type 分发"""
    msg_type = msg.get("type")

    if msg_type == "chat":
        # 私聊：发给 target_user
        target = msg.get("target")
        payload = {
            "type": "chat",
            "from": sender_id,
            "content": msg.get("content", ""),
            "time": datetime.now().strftime("%H:%M:%S"),
        }
        if target:
            await manager.send_to(target, payload)       # 发给对方
            await manager.send_to(sender_id, payload)    # 回显给自己
        else:
            await manager.broadcast(payload)             # 无 target 则广播

    elif msg_type == "get_online":
        await manager.send_to(sender_id, {
            "type": "online_list",
            "users": manager.online_users,
        })


# ============================================================
# 前端页面（直接挂载，方便测试）
# ============================================================
import os
FRONTEND_DIR = os.path.join(os.path.dirname(__file__), "..", "frontend")

@app.get("/")
async def index():
    return FileResponse(os.path.join(FRONTEND_DIR, "index.html"))

app.mount("/static", StaticFiles(directory=FRONTEND_DIR), name="static")


if __name__ == "__main__":
    uvicorn.run("server:app", host="0.0.0.0", port=8000, reload=True)
