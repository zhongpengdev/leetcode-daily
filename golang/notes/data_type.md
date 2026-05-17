# Go 数据类型

## 基本类型

### 整数

| 类型 | 大小 | 范围 |
|------|------|------|
| int8 | 1字节 | -128 ~ 127 |
| int16 | 2字节 | -32768 ~ 32767 |
| int32 (rune) | 4字节 | -2^31 ~ 2^31-1 |
| int64 | 8字节 | -2^63 ~ 2^63-1 |
| int | 平台相关 | 32位系统=4字节，64位系统=8字节 |

无符号版本：uint8 (byte)、uint16、uint32、uint64、uint

### 浮点数

- float32：精度约6-7位
- float64：精度约15-17位（默认）

### 其他基本类型

- bool：true / false
- string：不可变，UTF-8 编码
- rune：等价 int32，表示一个 Unicode 码点
- byte：等价 uint8

## 复合类型

### 数组

固定长度，长度是类型的一部分。

```go
var a [5]int            // [0 0 0 0 0]
b := [3]int{1, 2, 3}
c := [...]int{1, 2, 3}  // 自动推断长度
```

### 切片 (slice)

动态长度，底层是数组的引用。

```go
s := []int{1, 2, 3}
s = append(s, 4)        // 追加元素

// 从数组创建切片
arr := [5]int{1, 2, 3, 4, 5}
s1 := arr[1:3]           // [2, 3]
s2 := arr[:2]            // [1, 2]
s3 := arr[2:]            // [3, 4, 5]
```

切片三要素：指针、长度、容量。`len()` 取长度，`cap()` 取容量。

### map

```go
m := map[string]int{
    "alice": 90,
    "bob":   85,
}

m["charlie"] = 78       // 插入
delete(m, "bob")        // 删除

score, ok := m["alice"] // ok 判断 key 是否存在
```

### struct

```go
type Person struct {
    Name string
    Age  int
}

p := Person{Name: "alice", Age: 20}
p.Age = 21
```

## 指针

Go 有指针但不能做指针运算。

```go
x := 10
p := &x         // 取地址
fmt.Println(*p)  // 10，解引用
*p = 20          // 修改原值
```

## 类型转换

Go 不支持隐式转换，必须显式转换。

```go
var i int = 42
var f float64 = float64(i)   // 必须显式转
var u uint = uint(f)
```

## 零值

每种类型都有默认零值：

| 类型 | 零值 |
|------|------|
| 整数 | 0 |
| 浮点 | 0.0 |
| bool | false |
| string | "" (空串) |
| 指针/slice/map | nil |

## make 和 new

- `new(T)`：分配内存，返回 `*T`，用零值填充
- `make(T, args)`：只用于 slice、map、chan，返回初始化好的 T 本身

```go
p := new(int)           // *int，值为 0
s := make([]int, 0, 10) // slice，长度0容量10
m := make(map[string]int)
```
