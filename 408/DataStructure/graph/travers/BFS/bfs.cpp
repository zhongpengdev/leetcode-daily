/*
BFS
队列
visited[]
*/
#include<queue>
#include<vector>
#include<iostream>

void BFS(int startNode, std::vector<std::vector<int>>& adj){
    int n = adj.size();
    std::vector<bool> visited(n, false); //标记数组

    std::queue<int> q;

    visited[startNode] = true;
    q.push(startNode);

    while(!q.empty()) {
        int u = q.front();
        q.pop();

        std::cout << u << " ";

        //邻接表的实现
        for(int neighbor : adj[u]) {
            if(!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }

        //如果是邻接矩阵的话
        //一个一个试
        // for(int v = 0; v < n; v++) {
        //     if(adj[u][v] == 1) {
        //         int neighbor = v;
        //         if(!visited[neighbor]) { ... }
        //     }
        // }
    }

    std::cout << std::endl;
}