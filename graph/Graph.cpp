template <class S>
struct CSR {
  vector<S> v;
  vector<int> start;
  CSR() {}
  CSR(int n, const vector<pair<int, S>>& data) : v(data.size()), start(n + 1) {
    for (const auto& [i, d] : data) start[i + 1]++;
    for (int i = 0; i < n; i++) start[i + 1] += start[i];
    for (const auto& [i, d] : data) v[start[i]++] = d;
    for (int i = n; i > 0; i--) start[i] = start[i - 1];
    start[0] = 0;
  }
  auto operator[](int i) const {
    return span(v.begin() + start[i], v.begin() + start[i + 1]);
  }
};

template <class Edge>
struct Graph {
  int n;
  CSR<Edge> es;
  Graph() : n(0) {}
  Graph(int n, const vector<pair<int, Edge>>& es) : n(n), es(n, es) {}
  int size() const { return n; }
  auto operator[](int u) const {
    if constexpr (is_same_v<Edge, int>) return es[u];
    else return ranges::transform_view(es[u], [](const auto& e) { return e.to; });
  }
  template <bool directed>
  static Graph build_graph_from_stdin(int n, int m) {
    return Graph(n, [&]() {
      vector<pair<int, Edge>> es;
      es.reserve(directed ? m : 2 * m);
      for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        es.emplace_back(u, Edge{v});
        if constexpr (!directed) es.emplace_back(v, Edge{u});
      }
      return es;
    }());
  }
};

// struct E {
//   int to;
// };
