# sudo -i -u

切换用户。

```bash
sudo -i -u postgres  # 切换到postgres用户
```

- `sudo`：以管理员权限执行
- `-i`：模拟登录，加载目标用户的环境变量
- `-u`：指定切换到哪个用户

# gpasswd

重置用户密码。

```bash
sudo gpasswd postgres  # 重置postgres用户密码
```

# cd

切换目录。

```bash
cd ~      # 回到当前用户的家目录
cd ..     # 回到上一级目录
cd -      # 回到上一次所在的目录
```

`~` 是家目录的缩写，等价于 `/home/用户名`。

# alias

设置命令别名。

```bash
# 临时别名（当前终端有效）
alias ll='ls -l'

# 查看所有别名
alias

# 删除别名
unalias ll
```

## 永久别名

用户级配置文件：不需要管理员权限，仅在当前用户生效。

推荐写在 `~/.bash_aliases` 中（需手动创建）：

```bash
nano ~/.bash_aliases

# 添加别名，一行一个
alias ll='ls -la'
alias gs='git status'
```

确保 `~/.bashrc` 能自动加载：

```bash
if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi
```

使配置生效：

```bash
source ~/.bashrc
```

---

## 目录导航

# pwd

Print Working Directory

显示当前工作目录的绝对路径

```bash

pwd -L # --logical

pwd -P # --physical

```

# ls

```bash

ls -a

ls -l # 长格式

ls -h # 文件大小显示为KB/MB 配合-l一起使用

ls -t # 按照修改时间排序

ls -r # 反向排序

ls -R # 递归列出所有子目录

# --------------------

ls -lah # 最常用

ls -la #组合：长格式+显示隐藏文件

ls -lh

ls -lath
```


# cd
# find
```bash
# find [搜索路径] [条件]

find . -name "*.txt"

find . -iname "readme*" # 忽略大小写

find . -type f # 只查找file 
find . -type d # 只查找目录

find . -size +100M # 大小查找，大于100M
find . -size -1k 

find . -mtime -7 # 7天内修改过的文件
find . -mtime +30 # 30天前修改过的文件

find . -maxdepth 2 -name "*.txt" # 限制搜索深度

find . -name "*.txt" -and -size +1M # 组合搜索

#-----------------

# {} 代表找到每一个文件 
# \; 代表结束命令
find . -name "*.txt" -exec rm {} \; # 对结果执行操作
find . -name "*.txt" -exec cat {} \; # 打印内容
```
# locate
```bash
# locate 和find 都是用来查文件的，先用locate再用find

# locate 查数据库索引，速度更快

# locate 只能查名字

locate "*conf"
locate -i "readme" # 忽略大小写
locate -c "*txt" # 只显示匹配数量
locate -n 5 "*log" # 只显示前五条
```
# which
查找 命令的可执行文件路径
如果没输出说明命令不在 `$PATH`里面

```bash
which python3 # /usr/bin/python3
which ls
which gcc
which node
```


## 文件操作

# touch

创建空文件，或更新文件的修改时间。

```bash
touch file.txt           # 创建空文件（已存在则更新时间）
touch a.txt b.txt c.txt  # 同时创建多个文件
```

# mkdir

创建目录。

```bash
mkdir dir1               # 创建单个目录
mkdir -p a/b/c           # -p 递归创建多层目录（父目录不存在自动创建）
```

# rmdir

删除**空**目录。

```bash
rmdir dir1               # 删除空目录
rmdir -p a/b/c           # -p 递归删除空的父目录链
```

目录非空则删除失败，非空目录用 `rm -r`。

# rm

```bash

# 删除文件夹
rm -r dir

# 删除文件
rm -r file
```

删除文件或目录。

```bash
rm file.txt              # 删除文件
rm -r dir1               # -r 递归删除目录及其内容
rm -f file.txt           # -f 强制删除，不提示确认
rm -rf dir1              # 递归 + 强制（危险操作，慎用）
rm *.log                 # 删除所有log文件
```

`rm -rf` 不可恢复，养成 `rm -ri`（交互式确认）的习惯更安全。

# cp

复制文件或目录。

```bash
cp file.txt backup.txt       # 复制文件并重命名
cp file.txt dir1/            # 复制到目录下（保留原名）
cp -r dir1 dir2              # -r 递归复制整个目录
cp -i file.txt dir1/         # -i 覆盖前提示确认
cp file1.txt file2.txt dir1/ # 同时复制多个文件到目录
```

# mv

移动文件/目录，或重命名。

```bash
mv old.txt new.txt       # 重命名
mv file.txt dir1/        # 移动到目录下
mv dir1 dir2             # 移动/重命名目录
mv -i file.txt dir1/     # -i 覆盖前提示确认
```

# ln

创建链接。

```bash
ln file.txt hard_link        # 硬链接（指向同一inode，删原文件链接仍可用）
ln -s file.txt soft_link     # -s 软链接（相当于快捷方式，原文件删了链接失效）
```

| | 硬链接 | 软链接 |
|--|--------|--------|
| 本质 | 指向同一数据块 | 指向文件路径 |
| 原文件删除 | 链接仍可用 | 链接失效（断链） |
| 能否跨分区 | 不能 | 能 |
| 能否链接目录 | 不能 | 能 |

## 文件查看

# cat
# less
# more
# head
# tail
# wc
# grep
# sort
# uniq
# diff

## 文件编辑

# nano
# vim

## 文件权限

# chmod
# chown
# chgrp

## 用户管理

# su
# sudo
# useradd
# userdel
# usermod
# passwd
# groups
# id

## 进程管理

# ps
# top
# kill
# killall
# jobs
# bg
# fg
# nohup

## 系统信息

# uname
# hostname
# uptime
# free
# df
# du
# lscpu
# lsblk

## 网络

# ping
# ifconfig
# ip
# netstat
# ss
# curl
# wget
# ssh
# scp

## 压缩解压

# tar
# gzip
# gunzip
# zip
# unzip

## 包管理

# apt
# apt-get
# dpkg

## 磁盘管理

# fdisk
# mount
# umount

## 其他

# echo
# export
# env
# history
# clear
# man
# xargs
# tee
# pipe
# redirection
