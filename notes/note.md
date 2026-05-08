# g++ command

1. `g++ main.cpp -o app`

# Linux command

Recursively createv file: `mkdir -p`

`mkdir -p build && g main.cpp -o build/app` && 文件就绪后执行

# Warnning

-Wall
-Wextra
-Wpedantic
-Werror

`-Wall -Wextra -Werror`

# Linux alias

`g++ -Wall -Wextra -Werror -02 main.cpp -o app`

`g main.cpp -o app`

# using .bashrc or zshrc 

using alias permanently

```bash
echo $SHELL

nano ~/.bashrc

alias g='g++ -Wall -Wextra -Werror -O2'

ctrl+O save file
ctrl+X close nano

source ~/.bashrc
```


