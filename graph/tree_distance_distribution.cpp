vector<mint> get_distance_distribution(const vector<vector<int>>& g) {
  const int n = g.size();
  centroid_decomposition cd(g);
  vector<char> valid(n, true);
  vector<mint> res(n);
  vector<int> depths, heads; depths.reserve(n), heads.reserve(n);
  vector<mint> f;
  auto dfs = [&](auto&& self, int u) -> void {
    depths.clear();
    heads.clear();
    auto dfs_depth = [&](auto&& self, int u, int p, int d) -> void {
      depths.emplace_back(d);
      for (int v : g[u]) if (valid[v] && v != p) {
        self(self, v, u, d + 1);
      }
    };
    depths.emplace_back(0);
    for (int v : g[u]) if (valid[v]) {
      heads.emplace_back((int)depths.size());
      dfs_depth(dfs_depth, v, u, 1);
    }
    int dmx = *max_element(depths.begin(), depths.end());
    f.assign(dmx + 1, mint());
    for (int d : depths) f[d]++;
    vector<mint> g = convolution(f, f);
    for (int i = 0, i_last = min(2 * dmx, n - 1); i <= i_last; i++) {
      res[i] += g[i];
    }
    int hsz = heads.size();
    for (int i = 0; i < hsz; i++) {
      int l = heads[i];
      int r = i + 1 < hsz ? heads[i + 1] : (int)depths.size();
      int dmx = *max_element(depths.begin() + l, depths.begin() + r);
      f.assign(dmx + 1, mint());
      for (int i = l; i < r; i++) f[depths[i]]++;
      vector<mint> g = convolution(f, f);
      for (int i = 0, i_last = min(2 * dmx, n - 1); i <= i_last; i++) {
        res[i] -= g[i];
      }
    }
    res[0]++;
    valid[u] = false;
    for (int v : cd.ctree[u]) self(self, v);
  };
  dfs(dfs, cd.root);
  mint inv2 = mint(1) / 2;
  for(mint& x : res) x *= inv2;
  return res;
}