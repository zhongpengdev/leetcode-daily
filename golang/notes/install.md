# download latest version

```bash
# 下载官方 Linux 64位架构二进制包
wget https://go.dev/dl/go1.26.3.linux-amd64.tar.gz

# 解压至系统级通用目录
sudo tar -C /usr/local -xzf go1.26.3.linux-amd64.tar.gz

```

# APTH

```bash
echo 'export GOPATH=$HOME/gopath' >> ~/.bashrc
echo 'export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin' >> ~/.bashrc
source ~/.bashrc
```

# Go Modules

```bash
# 强制开启 Go Modules 模式
go env -w GO111MODULE=on

# 配置国内高可用代理，加速依赖下载
go env -w GOPROXY=https://goproxy.cn,direct

# 验证安装结果
go version
```

# go run 

```bash
go run hello.go



# 先编译后运行
# 编译
go build hello.go

# 运行（Linux/macOS）
./hello

# 或者运行（Windows）
hello.exe
```

## 使用go modules

```bash
# 1. 创建项目目录
mkdir helloproject && cd helloproject

# 2. 初始化模块（会生成 go.mod 文件）
go mod init example/hello

# 3. 创建 hello.go 文件（内容同上）

# 4. 运行
go run .
```