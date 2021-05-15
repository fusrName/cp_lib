std::vector<int> topological_sort(const std::vector<std::vector<int>>& graph) {
    // if the graph contains cycles or self-loops, returns empty vector
    const int n = graph.size();
    std::vector<int> indeg(n);
    for(int u = 0; u < n; u++) for(auto v: graph[u]) {
        indeg[v]++;
    }
    std::vector<int> todo; todo.reserve(n);
    for(int u = 0; u < n; u++) if (indeg[u] == 0) todo.push_back(u);
    std::vector<int> ret; ret.reserve(n);
    while(!todo.empty()) {
        int u = todo.back(); todo.pop_back();
        ret.push_back(u);
        for(auto v: graph[u]) {
            indeg[v]--;
            if (indeg[v] == 0) todo.push_back(v);
        }
    }
    if (int(ret.size()) != n) return {};
    return ret;
}