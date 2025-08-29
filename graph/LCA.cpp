// <O(NlogN, O(1)>
struct LCA {
  int n;
  vector<int> idx;
  struct S { int v, depth; };
  vector<vector<S>> spt;
  static constexpr S min(S x, S y) { return x.depth < y.depth ? x : y; }
  LCA(const auto& g, int root) : n(g.size()), idx(n, -1) {
    assert(0 <= root && root < n);
    vector<S> tour;
    auto dfs = [&](auto&& self, int u, int d) -> void {
      idx[u] = tour.size();
      tour.emplace_back(u, d);
      for (int v : g[u]) if (idx[v] == -1) {
        self(self, v, d + 1);
        tour.emplace_back(u, d);
      }
    };
    dfs(dfs, root, 0);
    int m = tour.size();
    int w = bit_width(m + 0U);
    spt.resize(w);
    spt[0] = move(tour);
    for (int k = 0; k + 1 < w; k++) {
      int k2 = 1 << k;
      int sz = m - 2 * k2 + 1;
      spt[k+1].resize(sz);
      for (int i = 0; i < sz; i++) spt[k+1][i] = min(spt[k][i], spt[k][i+k2]);
    }
  }
  int lca(int u, int v) {
    int l = idx[u], r = idx[v];
    if (l > r) swap(l, r);
    int k = bit_width(r - l + 1U) - 1;
    return min(spt[k][l], spt[k][r+1-(1<<k)]).v;
  }
};
