# 变量

```go
var intNumber int
var str string
var char byte

var (
    name string
    age int
    class int
)

var num1, num2, num3 int

var name string = "string"

// 短变量声明 只能用在初始化使用
name := "string"

name, age := "zhongpeng", 21

```

```go
// 函数内声明的变量没使用会报错，函数外可以通过编译

package main 

func main() {
    a := 1 // 不适用报错
}

//-----

a := 1

func main() {}
```


# 匿名

使用`_` 表示

# 交换

```go
num1, num2 := 25, 36
num1, num2 = num2, num1
```

# 比较

go不存在隐式类型转换，比较先转类型

```go
func main() {
  var a uint64
  var b int64
  fmt.Println(int64(a) == b)
}
```

## min max

```go
minVal := min(1, 2, -1, 1.2) // 泛型

maxVal := max(100, 22, -1, 1.12)
```

它们的参数支持所有的可比较类型，go 中的可比较类型有

布尔
数字
字符串
指针
通道 （仅支持判断是否相等）
元素是可比较类型的数组（切片不可比较）（仅支持判断是否相等）（仅支持相同长度的数组间的比较，因为数组长度也是类型的一部分，而不同类型不可比较）
字段类型都是可比较类型的结构体（仅支持判断是否相等）

# 代码块

在func中使用 `{}` 做变量的隔离

```go

func main() {
    a := 1

    {
        a := 2
        fmt.Println(a)
    }

    {
        a := 3
        fmt.Println(a) 
    }

    fmt.Println(a)
}
```