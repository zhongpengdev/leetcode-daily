导出依赖: pip freeze > requirements.txt

安装依赖：pip install -r requirements.txt

创建虚拟环境: python -m venv venv

激活虚拟环境: .\venv\Scripts\Activate.ps1

关闭虚拟环境：deactivate

# dotenv
from dotenv import load_dotenv

load_dotenv()
