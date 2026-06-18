# C++ Complie

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

实现一个完成的bash

```bash
g() {
    # 防御性边界检查：拦截空输入与不存在的无效路径
    if [ -z "$1" ] || [ ! -f "$1" ]; then
        echo "Error: Source file '$1' not found. Usage: g <path/to/file.cpp> [app_arguments...]" >&2
        return 1
    fi

    local src_file="$1"

    # 路径解耦与同构目录生成
    local src_dir
    src_dir=$(dirname -- "$src_file")
    
    local base_name
    base_name=$(basename -- "$src_file")
    base_name="${base_name%.*}"

    local target_dir="build/$src_dir"
    mkdir -p "$target_dir"

    local output_bin="$target_dir/$base_name"

    # 1. 使用 && 确保编译器返回码为 0 时才触发执行，拒绝运行历史残留的旧二进制文件
    # 2. 使用 "${@:2}" 将除第一个源文件参数外的一切后续输入，原封不动地透传给程序作为运行时参数
    g++ -Wall -Wextra -Werror -O2 "$src_file" -o "$output_bin" && "$output_bin" "${@:2}"
}
```
