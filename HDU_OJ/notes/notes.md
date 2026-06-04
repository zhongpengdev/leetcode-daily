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