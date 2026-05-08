# C++ 编译笔记

## g++ 基本用法

```bash
g++ main.cpp -o app
```

## 编译并运行

递归创建目录并编译：

```bash
mkdir -p build && g++ main.cpp -o build/app && ./build/app
```

## 常用编译选项

| 选项 | 说明 |
|------|------|
| `-Wall` | 开启大部分警告 |
| `-Wextra` | 额外警告 |
| `-Wpedantic` | 严格遵循标准 |
| `-Werror` | 将警告视为错误 |

推荐组合：`-Wall -Wextra -Werror`

```bash
g++ -Wall -Wextra -Werror -O2 main.cpp -o app
```

## 设置 alias

查看当前 shell：

```bash
echo $SHELL
```

编辑配置文件（bash 用 `~/.bashrc`，zsh 用 `~/.zshrc`）：

```bash
nano ~/.bashrc
```

添加一行：

```bash
alias g='g++ -Wall -Wextra -Werror -O2'
```

保存（`Ctrl+O`）并退出（`Ctrl+X`），然后生效：

```bash
source ~/.bashrc
```

之后直接用 `g main.cpp -o app` 即可编译。
