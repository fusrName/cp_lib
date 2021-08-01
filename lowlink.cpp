// The graph is assumed to be simple and connected
std::tuple<
    std::vector<std::vector<int>>,
    std::vector<int>,
    std::vector<int>> lowlink_connected(std::vector<std::vector<int>>& to, int root=0) {
    // returns a tuple of (spanning_tree (directed), ord, low)
    std::tuple<
        std::vector<std::vector<int>>,
        std::vector<int>,
        std::vector<int>> res;
    int n = to.size();
    auto& tree = std::get<0>(res);
    auto& ord = std::get<1>(res);
    auto& low = std::get<2>(res);
    tree.resize(n);
    ord.resize(n, -1);
    low.resize(n);
    int nxt_ord = 0;
    auto dfs = [&](auto&& self, int u, int parent=-1)->void {
        low[u] = ord[u] = nxt_ord++;
        for(int v: to[u]) {
            if (v == parent) continue;
            if (ord[v] == -1) {
                tree[u].push_back(v);
                self(self, v, u);
                if (low[v] < low[u]) low[u] = low[v];
            } else {
                if (ord[v] < low[u]) low[u] = ord[v];
            }
        }
    };
    dfs(dfs, root);
    return res;
}

// The graph is assumed to be simple
std::pair<std::vector<int>, std::vector<int>> lowlink(std::vector<std::vector<int>>& to) {
    // returns a pair of (ord, low)
    std::pair<std::vector<int>, std::vector<int>> res;
    const int n = to.size();
    auto& ord = res.first;
    auto& low = res.second;
    ord.resize(n, -1);
    low.resize(n);
    int nxt_ord = 0;
    auto dfs = [&](auto&& self, int u, int parent=-1) -> void {
        low[u] = ord[u] = nxt_ord++;
        for(int v: to[u]) {
            if (v == parent) continue;
            if (ord[v] == -1) {
                self(self, v, u);
                if (low[v] < low[u]) low[u] = low[v];
            } else {
                if (ord[v] < low[u]) low[u] = ord[v];
            }
        }
    };
    for(int i = 0; i < n; i++) if (ord[i] == -1) dfs(dfs, i);
    return res;
}