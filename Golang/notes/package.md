# Package

go的基本单位是package

```go
package main

import "example" // 导入包

import (
    "package1"
    "package2"
)
```

给包起别名
```go
package main

import (
    e "example"
    el "example1"
)
```

别名为`_`为匿名导入，匿名导入的包无法被使用，用于加载`init`函数，数据库注册

```go
package main

import (
    e "example"
    _ "mysql-driver-go"
)
```