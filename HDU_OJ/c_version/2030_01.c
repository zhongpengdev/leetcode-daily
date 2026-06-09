#include <stdio.h>

int main() {
    int count;
    // 1. 读取测试用例组数
    if (scanf("%d", &count) == 1) {
        getchar(); // 核心：吃掉输入数字后残留在缓冲区中的换行符 '\n'
        
        while (count--) {
            int byte_count = 0;
            int c;
            
            // 2. 逐个字符读取，直到遇到行末换行符或文件结束符 EOF
            while ((c = getchar()) != '\n' && c != EOF) {
                // GBK 编码中，汉字的两个字节最高位均为 1，即无符号数值大于 127
                if (c > 0x7F) {
                    byte_count++;
                }
            }
            
            // 3. 每个汉字由 2 个字节组成，最终汉字数 = 匹配字节数 / 2
            printf("%d\n", byte_count / 2);
        }
    }
    return 0;
}