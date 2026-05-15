#include<stdlib.h>
#define MaxVertexNum 100

//边表节点
typedef struct ArcNode {
    int adjvex; //弧指向的节点的位置下标
    struct ArcNode* nextarc; //下一个节点指针
}ArcNode;

//顶点表节点
typedef struct VNode {
    int data; //节点值
    ArcNode *firstarc; //第一个弧节点的指针
} VNode, AdjList[MaxVertexNum];

typedef struct 
{
    AdjList vertices;
    int vexnum, arcnum; //图的顶点数和弧数
}ALGraph;


//创建邻接表
void createGraph(ALGraph *G);

//辅助函数：查找顶点对应的下标
int locateVex(ALGraph *G, int v);

//DFS
void DFS(ALGraph *G, int v, int visited[]);

//遍历
void DFSTravers(ALGraph *G);


int locateVex(ALGraph *G, int v) {
    for (int i = 0; i < G->vexnum; i++) {
        if(G->vertices[i].data == v) {
            return i;
        }
    }
    return -1;
}

void createGraph(ALGraph *G) {
    //临时存储一条边的两个节点值
    int v1, v2;

    //待插入边的数组下标
    int i, j;
    
    printf("请输入图的顶点数和边数：");
    scanf("%d %d", &G->vexnum, &G->arcnum);

    //初始化所有顶点
    for(int i = 0; i < G->vexnum; i++) {
        scanf("%d", &G->vertices[i].data);
        G->vertices[i].firstarc = NULL;
    }

    //初始化所有的边
    for(int i = 0; i < G->arcnum; i++) {
        scanf("%d %d", &v1, &v2);

        //找到v1和v2对应的下标并插入
        i = locateVex(G, v1);
        j = locateVex(G, v2);

        if(i == -1 || j == -1) {
            //不存在，回退
            i--;
            continue;
        }

        //新建弧节点
        ArcNode* p = (ArcNode*)malloc(sizeof(ArcNode));
        
        if(p==NULL) {
            //分配失败
            exit(1);
        }

        p->adjvex = j;
        //头插法
        p->nextarc = G->vertices[i].firstarc; 
        G->vertices[i].firstarc = p; //成为新的首元素节点

        //如果是无向图的话，还需要新创建j中的i节点
    }
}





