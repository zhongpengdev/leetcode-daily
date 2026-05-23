# Writing basic knowledge of HDU OJ

## EOF

EOF(End Of File) which defined in `<stdio.h>` and the value of it is -1 usually (a specific number).

and `!= EOF` means the input has reached the end of the file.

```c
# include<stdio.h>
// when scanf read two numbers, it will return 2 and finall -1 at the end
while(scanf("%d, %d", &a, &b) != EOF) { 
    // execute the code here until the end of file
}
```

```cpp
while (cin >> a >> b); // Internally determine EOF by C++ itself
```