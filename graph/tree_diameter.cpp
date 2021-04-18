std::vector<int> tree_diameter(std::vector<std::vector<int>>& to) {
    const int n = to.size();
    std::vector<int> p(n);
    auto dfs = [&](auto&& self, int u, int par=-1, int d=0) -> int {
        int ret = d;
        p[u] = -1;
        for(int v: to[u]) if (v != par) {
            int d_v = self(self, v, u, d + 1);
            if (d_v > ret) ret = d_v, p[u] = v;
        }
        return ret;
    };
    dfs(dfs, 0);
    int v = 0;
    while(p[v] != -1) v = p[v];
    dfs(dfs, v);
    std::vector<int> res;
    while(v != -1) res.push_back(v), v = p[v];
    return res;
}