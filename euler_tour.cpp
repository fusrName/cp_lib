std::vector<int> vertices_on_euler_tour(const std::vector<std::vector<int>>& tree, int root=0) {
    const unsigned int n = tree.size();
    assert(n > 0);
    std::vector<int> res;
    res.reserve(2 * n - 1); // 1 + 2|E|
    std::vector<std::pair<int, int>> s;
    s.reserve(n);
    s.emplace_back(root, 0);
    res.push_back(root);
    while(!s.empty()) {
        auto& [u, i] = s.back();
        if (i == tree[u].size()) {
            s.pop_back();
            if (!s.empty()) res.push_back(s.back().first);
        } else {
            int v = tree[u][i++];
            const unsigned int sz = s.size();
            if (sz >= 2 && v == s[sz - 2].first) continue;
            s.emplace_back(v, 0);
            res.push_back(v);
        }
    }
    return res;
}