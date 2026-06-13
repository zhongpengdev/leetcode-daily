1. `sudo` 在当前用户下执行root操作，当前用户必须在sudo组才可以成功执行，输入的是当前用户的密码
   `getent group sudo` 查看sudo组有哪些用户 
   `sudo:x:27:zhongpeng` 组名：组密码占位符：组ID(GID)：用户