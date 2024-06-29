struct E { int from, to, i; };
 
// returns (undirected) MST
vector<E> incremental_scc(int n, span<E> edges) {
  int m = ssize(edges);
  for (int i = 0; i < m; i++) assert(edges[i].i == i);
  vector<char> used(n);
  vector<int> vs, e_data(m), e_begin(n), e_end(n), ord(n), low(n), ids(n), visited;
  vs.reserve(n), visited.reserve(n);
  auto scc = [&](span<E> edges) {
    for (int v : vs) used[v] = false;
    vs.clear();
    for (auto [from, to, i] : edges) {
      for (int v : {from, to}) {
        if (!used[v]) {
          used[v] = true;
          vs.emplace_back(v);
        }
      }
    }
    for (int v : vs) e_begin[v] = 0;
    for (auto e : edges) e_begin[e.from]++;
    for (int acc = 0; int v : vs) e_end[v] = acc, acc += exchange(e_begin[v], acc);
    for (auto e : edges) e_data[e_end[e.from]++] = e.to;
    int now_ord = 0;
    for (int v : vs) ord[v] = -1;
    for (int v : vs) if (ord[v] == -1) {
      auto dfs = [&](auto self, int u) -> void {
        low[u] = ord[u] = now_ord++;
        visited.emplace_back(u);
        for (int i = e_begin[u]; i < e_end[u]; i++) {
          int v = e_data[i];
          if (ord[v] == -1) {
            self(self, v);
            low[u] = min(low[u], low[v]);
          } else {
            low[u] = min(low[u], ord[v]);
          }
        }
        if (low[u] == ord[u]) {
          while (true) {
            int v = visited.back(); visited.pop_back();
            ord[v] = n;
            ids[v] = u;
            if (v == u) break;
          }
        }
      };
      dfs(dfs, v);
    }
  };
  vector<E> tmp, res;
  tmp.reserve(m);
  scc(edges);
  auto it = begin(edges);
  for (auto e : edges) {
    if (ids[e.from] == ids[e.to]) *it++ = e;
  }
  dsu uf(n);
  auto dfs = [&](auto self, int l, int r, span<E> edges) {
    if (r - l <= 1) {
      for (auto [from, to, i] : edges) {
        if (!uf.same(from, to)) {
          uf.merge(from, to);
          res.emplace_back(from, to, l);
        }
      }
      return;
    }
    int c = (l + r) / 2;
    auto it_c = ranges::lower_bound(edges, c, {}, &E::i);
    scc({begin(edges), it_c});
    tmp.clear();
    auto it1 = begin(edges);
    for (auto e : span(begin(edges), it_c)) {
      if (ids[e.from] == ids[e.to]) *it1++ = e;
      else tmp.emplace_back(ids[e.from], ids[e.to], e.i);
    }
    auto it2 = end(edges);
    for (auto e : span(it_c, end(edges)) | views::reverse) {
      if (used[e.from]) e.from = ids[e.from];
      if (used[e.to]) e.to = ids[e.to];
      if (e.from != e.to) *--it2 = e;
    }
    for (auto e : tmp | views::reverse) *--it2 = e;
    self(self, l, c, span(begin(edges), it1));
    self(self, c, r, span(it2, end(edges)));
  };
  dfs(dfs, 0, m, {begin(edges), it});
  return res;
}
