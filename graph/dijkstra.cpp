constexpr long long INF = 1002003004005006007;
template <class T>
vector<long long> dijkstra(vector<vector<pair<int, T>>>& to, int s = 0) {
  struct QueElem {
    int v;
    long long d;
    bool operator<(const QueElem& a) const { return d > a.d; }
    QueElem(int v, long long d) : v(v), d(d) {}
  };
  priority_queue<QueElem> q;
  vector<long long> dist(to.size(), INF);
  dist[s] = 0;
  q.emplace(s, 0);
  while (!q.empty()) {
    auto [u, d] = q.top(); q.pop();
    if (d > dist[u]) continue;
    for (auto [v, dv] : to[u]) {
      long long nd = d + dv;
      if (nd < dist[v]) {
        dist[v] = nd;
        q.emplace(v, nd);
      }
    }
  }
  return dist;
}
