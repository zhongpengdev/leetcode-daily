#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define MaxVertexNum 100
#define INF 1e9 //无穷大

//节点值
typedef struct ArcNode {
    int adjvex;
    struct ArcNode* nextarc;
} ArcNode;

//头节点值
typedef struct VNode {
    int data;
    ArcNode* firstarc;
}VNode, AdjList[MaxVertexNum];

//图的定义
typedef struct {
    AdjList vertices;
    int vexnum, arcnum; //头结点，边节点
}ALGraph;







//队列实现
int queue[MaxVertexNum];
int front = 0, rear = 0;

void EnQueue(int v) {   queue[rear++] = v;}
int DeQueue() { return queue[front++];}
bool IsEmpty() {    return front == rear; }






//查找u节点到其他节点之间的最短距离
void BFS_MIN_Distance(ALGraph G, int u) {
    int d[MaxVertexNum]; //distance距离数组
    bool visited[MaxVertexNum];

    for(int i = 0; i < G.vexnum; i++) {
        d[i] = INF; //初始距离为∞
        visited[i] = false;
    }

    //初始化
    d[u] = 0;
    visited[u] = true;
    EnQueue(u);

    while (!IsEmpty()) 
    {
        int currentNode = DeQueue();

        ArcNode *p = G.vertices[currentNode].firstarc;
        while (p != NULL)
        {
            int w = p->adjvex;

            if(!visited[w]) {
                d[w] = d[currentNode] + 1;
                visited[w] = true;
                EnQueue(w);
            }

            p = p->nextarc;
        }   
    }   
}


