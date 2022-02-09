vector<vector<int>> cycle_decomposition(const vector<int>& a) {
  const int n = a.size();
  vector<char> visited(n);
  vector<vector<int>> res;
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

vector<vector<int>> root_cycles(const vector<int>& a) {
  int n = a.size();
  vector<char> visited(n);
  vector<char> in_stack(n);
  vector<int> st;
  vector<vector<int>> res;
  for (int i = 0; i < n; i++) {
    if (visited[i]) continue;
    int v = i;
    do {
      visited[v] = true;
      st.emplace_back(v);
      in_stack[v] = true;
      v = a[v];
      if (in_stack[v]) {
        auto it = find(st.begin(), st.end(), v);
        res.emplace_back(it, st.end());
        break;
      }
    } while (!visited[v]);
    while (!st.empty()) {
      in_stack[st.back()] = false;
      st.pop_back();
    }
  }
  return res;
}