/*
欧拉路径 或欧拉回路
*/
#include <stdlib.h>
#define MAXV 1000

typedef struct MGPath
{
    int numberVerctices, numberEdges; // 图中顶点数 边数
    char VerticesList[MAXV]; //顶点表
    int Edge[MAXV][MAXV];
} MGPath;

int isExitEL(MGPath G)
{
    int count = 0; // 全部度为奇数的个数

    int n = G.numberVerctices;

    for (int i = 0; i < n; i++)
    {
        int degree = 0;
        for (int j = 0; j < G.numberVerctices; j++)
        {
            degree += G.Edge[i][j];
        }

        // 如果是奇数
        if (degree % 2 != 0)
        {
            count++;
        }
    }

    if (count == 0 || count == 2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
