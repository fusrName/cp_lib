constexpr int INF = 1001001001;
template <int k = 2, class T>  // cost in [0, k)
vector<int> kBFS(const vector<vector<pair<int, T>>>& g, int s = 0) {
  static_assert(k >= 1);
  const int n = g.size();
  vector<int> dist(n, INF);
  vector<int> q[2 * k];
  dist[s] = 0;
  q[0].emplace_back(s);
  for (int dist_cur = 0, ptr = 0;;) {
    const auto qb = q + ptr;
    while (!qb[0].empty()) {
      int u = qb[0].back();
      qb[0].pop_back();
      if (dist[u] != dist_cur) continue;
      for (auto [v, c] : g[u]) {
        int nd = dist_cur + c;
        if (nd < dist[v]) {
          dist[v] = nd;
          qb[c].emplace_back(v);
        }
      }
    }
    do {
      dist_cur++;
      ptr++;
      if (ptr == 2 * k) return dist;
    } while (q[ptr].empty());
    if (ptr > k) {
      for (int i = ptr; i < 2 * k; i++) {
        swap(q[i], q[i - ptr]);
      }
      ptr = 0;
    }
  }
}