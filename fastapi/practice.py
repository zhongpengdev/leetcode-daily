import asyncio

def normal():
    print("hello")
    
nore1 = normal
print(type(nore1)) # <class 'function'>

nore2 = normal() # 执行

async def asy_func():
    return 42
    
asyfunc1 = asy_func
print(type(asyfunc1)) # <class 'function'>

asyfunc2 = asy_func()
print(type(asyfunc2)) # <class 'coroutine'>

result = asyncio.run(asyfunc2)
print(result)