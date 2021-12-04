constexpr int INF = 1001001001;
template <class T, int k = 2>
vector<int> kBFS(vector<vector<pair<int, T>>> g, int s = 0) {
  const int n = g.size();
  vector<int> dist(n, INF);
  deque<vector<int>> q(k);
  dist[s] = 0;
  q[0].emplace_back(s);
  for (int dist_cur = 0;;) {
    vector<int>& vs = q[0];
    while (!vs.empty()) {
      int u = vs.back();
      vs.pop_back();
      if (dist[u] != dist_cur) continue;
      for (auto [v, c] : g[u]) {
        int nd = dist_cur + c;
        if (nd < dist[v]) {
          dist[v] = nd;
          q[c].emplace_back(v);
        }
      }
    }
    for(int i = 1;; i++) {
      dist_cur++;
      vector<int> tmp = move(q.front());
      q.pop_front();
      q.emplace_back(move(tmp));
      if (!q[0].empty()) break;
      if (i == k - 1) return dist;
    }
  }
}