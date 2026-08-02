# ~ 和 / 的区别

`cd ~`等价于`cd` 回的是家目录
`cd /`回的是文件系统根目录

进入根目录不需要root权限但是操作子目录可能需要。

| 特性 | `cd ~` | `cd /` |
| :--- | :--- | :--- |
| 目标位置 | 当前登录用户的主目录 | 文件系统根目录 |
| 实际路径（root用户） | `/root` | `/` |
| 实际路径（普通用户alice） | `/home/alice` | `/` |
| 权限要求 | 无需特殊权限 | 进入不需要权限，但操作子目录可能需要root |
| 等价写法 | `cd` (不带参数) | 无 |
| 典型用途 | 回到个人工作区、查看个人配置 | 从根目录开始浏览/搜索系统文件 |

# 根目录下常见文件夹含义

/bin 全称：Binaries 基本命令的二进制文件 如 ls, cp, cat
/sbin 全称：System Binaries 系统管理命令
/lib /lib64 共享库文件 /bin或者/sbin中程序依赖的.so文件及内核模块
/usr Unix System Resources 用户级应用程序 

> 一般来说，/bin /lib都是指向 /usr/bin /usr/lib下面的软链接

## usr子目录

/usr/bin：用户安装的命令都在这里 python vim gcc
/usr/sbin：非系统核心的管理工具
/usr/bin：库文件

# 配置与数据

/etc：Etcetera 全局配置文件
/var：Variable：可变数据
/tmp：Tmporary：临时文件
/opt：Optional：第三方大软件

# 运行时设备

/dev：设备文件：硬盘sda 终端tty 随机数urandom
/proc：Process，虚拟文件系统，映射内核和进程信息
/sys
/run：运行时数据，PID等重启删除

启动相关的：
/boot
/efi
