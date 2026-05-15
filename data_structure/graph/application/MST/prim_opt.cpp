#include<iostream>
#include<vector>
#include<queue>

typedef std::pair<int, int> PII; //{weight, node}

int prim_opt(int n, std::vector<std::vector<PII>>& adj) {
    //小顶堆 - 快速得到树中的最小值，替代第一个for循环
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq; //根据weight排序

    std::vector<bool> visited(n+1, false);
    int total_weight = 0;
    int count = 0; //入树的节点数

    pq.push({0, 1}); //第一个入树的节点

    while(!pq.empty()) {
        PII top = pq.top();
        pq.pop();

        int weight = top.first;
        int u = top.second;

        if(visited[u]) continue;

        visited[u] = true;
        total_weight += weight;
        count++;

        //第二个for循环，更新当前节点的路径
        for(auto& edge : adj[u]) {
            int v = edge.second;
            int weight = edge.first;
            if(!visited[v]) {
                pq.push({weight, v}); //入堆
            }
        }
    }

    return (count == n) ? total_weight : -1;
}