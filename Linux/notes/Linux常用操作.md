pwd

ls -la 长格式+隐藏文件

which 查的是可执行文件 
locate 更快，但是需要单独安装：sudo apt install plocate]
    locate Linux常用操作
find 默认支持 find [文件路径] [文件名]
    find . -name "Linxu常用操作"

创建文件 touch
创建目录 mkdir

删除文件/目录 rm -r filename

复制 
cp a.txt b.txt
cp a.txt dir/
cp -r dira dir2
cp records.md ./backup/records_rename.md 复制并重命名

移动/重命名
mv text.txt txt_newname.txt
mv text.txt ./new_directory

# 软硬连接

硬链接

`ln HDU_OJ/c_version/1013.c this_link.c`

ln [要连接的文件，必须未创建] [目标文件]

改一处全部都改。

ln -s # 软连接

ln -s [被链接的文件] [新文件，必须未创建]

lrwxrwxrwx  1 zhongpeng zhongpeng   23 Jun 22 11:23 soft_link.c -> HDU_OJ/c_version/1013.c


cat查看文件

tail 查看文件的末尾，看日志最常用

tail -f 实时监控
tail -n 指定行数 tail -n 20 等价于 tail -20

head 查看文件开头

grep 

grep [选项] [模式] [文件]
grep [选项] [模式]           # 从标准输入读取
命令 | grep [选项] [模式]    # 管道用法

-i 忽略大小写
-n 显示行号
-v 排除
-c 统计行数

-C n 查看匹配前后的n行

grep -in error app.log
grep -vn error app.log # 排除error
grep -c ERROR app.log
grep -C 10 ERROR app.log

tail -f app.log | grep -i error --color # 实时监控日志

# 进程相关

ps 进程快照

ps -ef 查看所有的进程
ps -aux

ps -ef | grep python 
ps -ef | grep nginx
