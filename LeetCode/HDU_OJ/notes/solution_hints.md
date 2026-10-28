# OJ

1. 2017
   如何做字符串输入控制
   ```c
    char str[1000];
    scanf("%s", str); // 主要是如何读取字符串。
   ```

   做字符串比较，拿到数字
   ```c
   if (str[i] >= '0' && str[i] <= '9')
   ```

2. 2018 
   找数学规律 f(n-1) 是去年的牛，今年还在，f(n-3)是三年前牛的数量，生了同样数量的牛。

3. 2019
    不需要malloc
    ```c
    #include<stdlib.h>

    int* array = (int*) malloc(n * sizeof(int));
    ```
4. 2021 基础贪心，整除余额

5. 2026
   字符串的输入控制
   ```c
   while (fgets(str, sizeof(str), stdin)) {
      str[strcspn(str, "\n")] = '\0';
   }
   ```

6. HDU OJ 403 安全审查机制
   C 提交触发 403 是因为**函数名黑名单匹配**，不是语义分析。

   黑名单（会 403）：
   - `getchar()`, `gets()`, `fgets()`, `fgetc()` 等带文件句柄的函数
   - `strcspn()` 及 `<string.h>` 下的大部分符号
   - `#include <string.h>` 头文件本身

   白名单（安全通过）：
   - `scanf` 全系列：`%d` `%c` `%*c` `%[^\n]`
   - `printf`
   - `#include <stdio.h>`

   应对原则：只用 `stdio.h` + `scanf` + `printf`，不用 `string.h`。

   常见场景替换：
   | 需求         | 被拦写法                         | 安全写法                                         |
   | ------------ | -------------------------------- | ------------------------------------------------ |
   | 吃掉换行符   | `getchar()`                      | `scanf("%*c")`                                   |
   | 读带空格整行 | `fgets(str, 100, stdin)`         | 逐字符 `scanf("%c", &ch)` 读到 `\n`              |
   | 去换行符     | `str[strcspn(str, "\n")] = '\0'` | 读的时候遇到 `\n` 就 `break`，直接 `str[i]='\0'` |

7. 2028 求最小公倍数/求数组的最小公倍数
8. 2031 十进制转任意进制的方法
9. 1019求数组的最小公倍数

```c
    // 任意两个数都要找到最小公倍数
int arrays_lcm(int array[], int count){
    int result = array[0];

    for (int i = 1; i < count; i++)
    {
        result = lcm(result, array[i]);
    }

    return result;
}
```
