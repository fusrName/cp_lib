template<class T, class S, class Compare=less<S>>
vector<T> prefix_min_sums(const vector<S>& a) {
  int n = a.size();
  auto [to, root] = cartesian_tree<S, Compare>(a);
  vector<T> d(n + 1);
  auto dfs = [&](auto&& self, int u, int l, int r) -> void {
    if (u == -1) return;
    T v = T(a[u]) * (r - u);
    d[l] += v;
    d[u + 1] -= v;
    self(self, to[u][0], l, u);
    self(self, to[u][1], u + 1, r);
  };
  dfs(dfs, root, 0, n);
  for (int i = 0; i < n; i++) d[i + 1] += d[i];
  d.resize(n);
  return d;
}
