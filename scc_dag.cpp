std::vector<std::vector<int>> scc_dag(std::vector<std::vector<int>>& scc, std::vector<std::vector<int>>& graph) {
    int n = graph.size();
    int m = scc.size();
    std::vector<int> d(n);
    for(int i = 0; i < m; ++i) for(int v: scc[i]) d[v] = i;
    std::vector<std::vector<int>> to(m);
    std::vector<int> added(m, -1);
    for(int i = 0; i < m; ++i) {
        added[i] = i;
        for(int u: scc[i]) for(int x: graph[u]) {
            int v = d[x];
            if (added[v] != i) {
                added[v] = i;
                to[i].push_back(v);
            }
        }
    }
    return to;
}