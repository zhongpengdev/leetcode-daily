#include <stdio.h>

int main()
{
    int count;
    scanf("%d", &count);
    
    while (count--)
    {
        int n;
        scanf("%d", &n);

        // 最后只剩下三只羊了
        int result = 3;
        
        // 开始回退 result = (result - 1) * 2
        for (int i = 0; i < n; i++)
        {
            result = (result - 1) * 2;
        }
        
        printf("%d\n", result);
    }
    
    return 0;
}