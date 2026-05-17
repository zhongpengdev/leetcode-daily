# 标准输入输出

Go 的 I/O 基于 `fmt`、`bufio`、`os` 三个包。

## 输出

### fmt.Print / fmt.Println / fmt.Printf

```go
fmt.Print("hello", "world")   // helloworld，不换行
fmt.Println("hello", "world")  // hello world\n，自动加空格和换行
fmt.Printf("name: %s, age: %d\n", "alice", 20)  // 格式化输出
```

### 常用格式化占位符

| 占位符 | 说明 |
|--------|------|
| `%v` | 默认格式（万能） |
| `%+v` | 打印结构体时带字段名 |
| `%#v` | Go 语法表示 |
| `%T` | 类型 |
| `%d` | 十进制整数 |
| `%f` | 浮点数 |
| `%s` | 字符串 |
| `%t` | bool |
| `%p` | 指针地址 |
| `%%` | 字面百分号 |

```go
type User struct {
    Name string
    Age  int
}

u := User{"alice", 20}
fmt.Printf("%v\n", u)   // {alice 20}
fmt.Printf("%+v\n", u)  // {Name:alice Age:20}
fmt.Printf("%#v\n", u)  // main.User{Name:"alice", Age:20}
fmt.Printf("%T\n", u)   // main.User
```

### fmt.Sprint / fmt.Sprintf（返回字符串不输出）

```go
s := fmt.Sprintf("hello %s", "world")  // s = "hello world"
```

### fmt.Fprint（写入指定 Writer）

```go
f, _ := os.Create("output.txt")
fmt.Fprintln(f, "写入文件")  // 写入文件，不是控制台
```

## 输入

### fmt.Scan / fmt.Scanln

```go
var name string
var age int

fmt.Scan(&name, &age)       // 空格分隔，阻塞等待
fmt.Scanln(&name, &age)     // 换行结束

fmt.Scanf("%s %d", &name, &age)  // 格式化读取
```

### bufio.NewReader（推荐，更灵活）

```go
reader := bufio.NewReader(os.Stdin)

// 读一行
line, _ := reader.ReadString('\n')
fmt.Println("你输入了:", line)

// 读一个字节
b, _ := reader.ReadByte()

// 读到指定分隔符
data, _ := reader.ReadString('\n')
```

### bufio.Scanner（最推荐，处理按行输入）

```go
scanner := bufio.NewScanner(os.Stdin)

for scanner.Scan() {
    line := scanner.Text()
    fmt.Println("读到:", line)
}

if err := scanner.Err(); err != nil {
    fmt.Fprintln(os.Stderr, "读取错误:", err)
}
```

## 文件 I/O

### 写文件

```go
// 方式1：os.WriteFile（一次性写入，Go 1.16+）
data := []byte("hello world")
os.WriteFile("test.txt", data, 0644)

// 方式2：os.Create + Write
f, _ := os.Create("test.txt")
defer f.Close()
f.Write([]byte("hello world"))

// 方式3：fmt.Fprint
f, _ := os.Create("test.txt")
defer f.Close()
fmt.Fprintln(f, "hello world")
```

### 读文件

```go
// 方式1：os.ReadFile（一次性读入内存，Go 1.16+）
data, _ := os.ReadFile("test.txt")
fmt.Println(string(data))

// 方式2：bufio.Scanner（逐行读取，适合大文件）
f, _ := os.Open("test.txt")
defer f.Close()

scanner := bufio.NewScanner(f)
for scanner.Scan() {
    fmt.Println(scanner.Text())
}
```

### 追加写入

```go
f, _ := os.OpenFile("test.txt", os.O_APPEND|os.O_WRONLY, 0644)
defer f.Close()
fmt.Fprintln(f, "追加的内容")
```

## 常用 os 常量

```go
os.Stdin   // 标准输入
os.Stdout  // 标准输出
os.Stderr  // 标准错误
```

## 对比总结

| 场景 | 推荐方式 |
|------|---------|
| 简单输出 | `fmt.Println` |
| 格式化输出 | `fmt.Printf` |
| 读一个值 | `fmt.Scan` |
| 读一行 | `bufio.NewScanner` |
| 读整个文件 | `os.ReadFile` |
| 逐行读文件 | `bufio.NewScanner` |
| 写整个文件 | `os.WriteFile` |
| 逐行写文件 | `fmt.Fprintln` |
