导出依赖: pip freeze > requirements.txt

安装依赖：pip install -r requirements.txt

创建虚拟环境: python -m venv venv

激活虚拟环境: .\venv\Scripts\Activate.ps1

关闭虚拟环境：deactivate

# dotenv
from dotenv import load_dotenv

load_dotenv()

# requirements.txt requirements-dev.txt

`requirements-dev.txt`用来放测试包的，只能开发时运行不能放在生产中构建，自动导入requirements.txt的包。

```
-r requirements.txt
pytest>=7.4
pytest-asyncio>=0.21
pytest-cov>=4.0
httpx>=0.24
aioresponses>=0.7
```
