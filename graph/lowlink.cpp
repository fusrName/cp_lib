auto lowlink(const auto& g) {
  int n = g.size();
  vector<int> ord(n, -1), low(n, -1);
  int ord_nxt = 0;
  auto dfs = [&](auto&& self, int u, int par) -> void {
    low[u] = ord[u] = ord_nxt++;
    for (int v : g[u]) {
      if (v == par) {
        par = -1;
      } else if (ord[v] == -1) {
        self(self, v, u);
        chmin(low[u], low[v]);
      } else {
        chmin(low[u], ord[v]);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (ord[i] == -1) dfs(dfs, i, -1);
  }
  return pair(move(ord), move(low));
}

auto two_edge_connected_components(const auto& g) {
  int n = g.size();
  vector<int> ord(n, -1), low(n, -1), v2gid(n, -1), st_vs, st_cs;
  vector<pair<int, int>> es;
  int ord_nxt = 0, gid_nxt = 0;
  auto dfs = [&](auto&& self, int u, int par) -> void {
    low[u] = ord[u] = ord_nxt++;
    int szv = st_vs.size(), szc = st_cs.size();
    st_vs.emplace_back(u);
    for (int v : g[u]) {
      if (v == par) {
        par = -1;
      } else if (ord[v] == -1) {
        self(self, v, u);
        if (low[v] == ord[v]) st_cs.emplace_back(v2gid[v]);
        else chmin(low[u], low[v]);
      } else {
        chmin(low[u], ord[v]);
      }
    }
    if (low[u] == ord[u]) {
      while (ssize(st_vs) > szv) {
        v2gid[st_vs.back()] = gid_nxt;
        st_vs.pop_back();
      }
      while (ssize(st_cs) > szc) {
        int c = st_cs.back();
        st_cs.pop_back();
        es.emplace_back(c, gid_nxt);
        es.emplace_back(gid_nxt, c);
      }
      gid_nxt++;
    }
  };
  for (int i = 0; i < n; i++) {
    if (ord[i] == -1) dfs(dfs, i, -1);
  }
  return tuple(move(ord), move(low), Graph<int>(gid_nxt, es), move(v2gid));
}

struct E {
  int to, id;
};

// self-loops are ignored
auto block_cut_tree(const auto& g) {
  int n = g.size();
  int imx = -1;
  for (int u = 0; u < n; u++) for (const auto& e : g.es[u]) chmax(imx, e.id);
  vector<int> ord(n, -1), low(n, -1), st_eid, st_vs, eid2gid(imx + 1, -1);
  vector<pair<int, int>> es;
  int ord_nxt = 0, gid_nxt = n;
  auto dfs = [&](auto&& self, int u, int id_par) -> void {
    low[u] = ord[u] = ord_nxt++;
    for (const auto& e : g.es[u]) if (e.id != id_par) {
      int v = e.to;
      if (ord[v] == -1) {
        int sze = st_eid.size(), szv = st_vs.size();
        st_eid.emplace_back(e.id);
        st_vs.emplace_back(v);
        self(self, v, e.id);
        if (low[v] < ord[u]) {
          chmin(low[u], low[v]);
        } else {
          while (ssize(st_eid) > sze) {
            eid2gid[st_eid.back()] = gid_nxt;
            st_eid.pop_back();
          }
          st_vs.emplace_back(u);  // add root
          while (ssize(st_vs) > szv) {
            int v = st_vs.back();
            st_vs.pop_back();
            es.emplace_back(v, gid_nxt);
            es.emplace_back(gid_nxt, v);
          }
          gid_nxt++;
        }
      } else if (ord[v] < ord[u]) {
        chmin(low[u], ord[v]);
        st_eid.emplace_back(e.id);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (ord[i] == -1) dfs(dfs, i, -1);
  }
  return tuple(move(ord), move(low), Graph<int>(gid_nxt, es), move(eid2gid));
}
