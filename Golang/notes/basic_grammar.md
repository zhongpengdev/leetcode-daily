# 常见标识符

```txt
break        default      func         interface    select
case         defer        go           map          struct
chan         else         goto         package      switch
const        fallthrough  if           range        type
continue     for          import       return       var
```

# 运算符

```
Precedence    Operator
    5             *  /  %  <<  >>  &  &^
    4             +  -  |  ^
    3             ==  !=  <  <=  >  >=
    2             &&
    1             ||
```

```go
a ^ b   // 异或 XOR
a &^ b  // 位清零 AND NOT（把 a 中 b 为1的位清零）
^a      // 取反 NOT
```

自增运算没有返回值

```go
a++ // 正确
++a // 错误
a-- // 正确
```

# 字面量

## 整数

下划线区分。

```go
24 // 24
024 // 24
2_4 // 24
0_2_4 // 24
10_000 // 10k
100_000 // 100k
0O24 // 20
0b00 // 0
0x00 // 0
0x0_0 // 0
```

## 浮点数

```go
0.
72.40
072.40       // == 72.40
2.71828
1.e+0
6.67428e-11
1E6
.25
.12345E+5
1_5.         // == 15.0
0.15e+0_2    // == 15.0

0x1p-2       // == 0.25
0x2.p10      // == 2048.0
0x1.Fp+0     // == 1.9375
0X.8p-0      // == 0.5
0X_1FFFP-16  // == 0.1249847412109375
0x15e-2      // == 0x15e - 2 (integer subtraction)
```
## 复数

```go
0i
0123i         // == 123i
0o123i        // == 0o123 * 1i == 83i
0xabci        // == 0xabc * 1i == 2748i
0.i
2.71828i
1.e+0i
6.67428e-11i
1E6i
.25i
.12345E+5i
0x1p-2i       // == 0x1p-2 * 1i == 0.25i
```

## 字符使用单引号

支持utf-8
```go
'a'
'ä'
'你'
'\t'
'\000'
'\007'
'\377'
'\x07'
'\xff'
'\u12e4'
'\U00101234'
```

## 字符串使用双引号

```go
`abc`                // "abc"
`\n
\n`                  // "\\n\n\\n"
"\n"
"\""                 // `"`
"Hello, world!\n"
"今天天气不错"
"日本語"
"\u65e5本\U00008a9e"
"\xff\u00FF"
```

## func

1. 参数类型后置
2. 多返回值
3. 强制花括号不换行

## 变量声明

```go
var a int = 10     // 完整写法
var a = 10         // 类型推断
a := 10            // 简短写法，只能在函数内用
```

## if / for / switch

`if` 和 `for` 不需要括号，`switch` 自动 break。

```go
if x > 0 { ... }

for i := 0; i < 10; i++ { ... }
for { ... }  // 死循环

switch x {
case 1:
    // 自动 break，不会穿透
case 2:
}
// 要穿透需显式写 fallthrough
```

## 一行代码简写

```go
for i := 0; i < 10; i++ {fmt.Println(i)}
```

## go中没有三元表达式