vector<vector<int>> cycle_decomposition(const vector<int>& a) {
  const int n = a.size();
  vector<char> visited(n);
  vector<vector<int>> res(n);
  for(int i = 0; i < n; i++) if (!visited[i]) {
    res.emplace_back();
    auto& vs = res.back();
    int v = i;
    do {
      visited[v] = true;
      vs.push_back(v);
      v = a[v];
    } while (v != i);
  }
  return res;
}