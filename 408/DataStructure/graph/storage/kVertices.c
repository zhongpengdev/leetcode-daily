/*
邻接矩阵中出度大于入度的顶点的个数
P214页 8题
*/

#define MAXV 100

typedef struct MGraph
{
    int numberVerctices, numberEdges; // 图中顶点数 边数
    char VerticesList[MAXV];          // 顶点表
    int Edge[MAXV][MAXV];
} MGraph;

int printVerctices(MGraph G)
{
    int n = G.numberVerctices;

    int count = 0;

    for (int i = 0; i < n; i++)
    {
        // 当前节点的出度
        int out_degree = 0;
        //出度
        int inner_degree = 0;
        for (int j = 0; j < n; j++)
        {
            out_degree += G.Edge[i][j];
            inner_degree += G.Edge[j][i];
        }

        if (out_degree > inner_degree)
        {
            count++;
        }
    }

    return count;
}
