import asyncio

# async def coroutinue_func():
#     print("func")
        
# coro = coroutinue_func();

"""
<class 'coroutine'>
<coroutine object coroutinue_func at 0x799eeb734e80>
"""
# print(type(coro))
# print(coro)

"""
Future生命周期：创建 Future → 状态 pending（未完成）→ set_result() 被调用 → 状态 done（已完成）
"""
f = Future()

print(f.done())
# print(f.result()) # 报错，Result is not set

f.set_result(42)

print(f.done())
print(f.result())


class AsyncConnrction:
    async def __aenter__(self):
        print("async manneager opened")
        await asyncio.sleep(1)
        return self

    async def __aexit__(self, exc_type, exc, tb):
        print("async mannager closed")
        await asyncio.sleep(1)
        return False
    
    async def query(self, sql):
        await asyncio.sleep(1)
        return f"result of {sql}"
    