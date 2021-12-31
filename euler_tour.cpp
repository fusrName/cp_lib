vector<int> euler_tour_vertices(const vector<vector<int>>& tree, int root=0) {
  const int n = tree.size();
  assert(n > 0);
  vector<int> res;
  res.reserve(2 * n - 1); // 1 + 2|E|
  res.emplace_back(root);
  vector<pair<int, int>> st;
  st.reserve(n);
  st.emplace_back(root, 0);
  while(!st.empty()) {
    auto& [u, i] = st.back();
    if (i == (int)tree[u].size()) {
      st.pop_back();
      if (!st.empty()) res.emplace_back(st.back().first);
    } else {
      int v = tree[u][i++];
      const int sz = st.size();
      if (sz >= 2 && v == st[sz - 2].first) continue;
      st.emplace_back(v, 0);
      res.emplace_back(v);
    }
  }
  return res;
}