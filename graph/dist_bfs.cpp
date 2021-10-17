vector<int> bfs(const vector<vector<int>>& g, int s=0) {
    const int n = g.size();
    vector<int> dist(n, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while(!q.empty()) {
        int u = q.front(); q.pop();
        int nd = dist[u] + 1;
        for(int v: g[u]) if (dist[v] == -1) {
            dist[v] = nd;
            q.push(v);
        }
    }
    return dist;
}