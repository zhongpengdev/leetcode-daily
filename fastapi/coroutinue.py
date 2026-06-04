from asyncio import Future

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