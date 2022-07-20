struct HLD {
  const vector<vector<int>>& to;
  int root, n;
  vector<int> sz, parent, depth, idx, ridx, head, inv;

  HLD(const vector<vector<int>>& to, int root=0)
      : to(to), root(root), n(to.size()),
        sz(n), parent(n), depth(n), idx(n), ridx(n), head(n), inv(n) {
    init_tree_data(root, -1, 0);
    int nxt = 0;
    assign_idx(root, root, nxt);
  }
  void init_tree_data(int u, int p, int d) {
    parent[u] = p;
    depth[u] = d;
    int s = 1;
    for (int v: to[u]) if (v != p) {
      init_tree_data(v, u, d+1);
      s += sz[v];
    }
    sz[u] = s;
  }
  void assign_idx(int u, int h, int& nxt, int p=-1) {
    head[u] = h;
    idx[u] = nxt;
    inv[nxt] = u;
    nxt++;
    int heaviest = -1;
    int mxweight = 0;
    for (int v: to[u]) if (v != p) {
      if (sz[v] > mxweight) {
        heaviest = v;
        mxweight = sz[v];
      }
    }
    if (heaviest != -1) {
      assign_idx(heaviest, h, nxt, u);
      for (int v: to[u]) if (v != p && v != heaviest) {
        assign_idx(v, v, nxt, u);
      }
    }
    ridx[u] = nxt;
  }

  int lca(int u, int v) {
    while (head[u] != head[v]) {
      if (depth[head[u]] > depth[head[v]]) {
        u = parent[head[u]];
      } else {
        v = parent[head[v]];
      }
    }
    return depth[u] < depth[v] ? u : v;
  }
  // returns reference to tuple of (path fragments from x upto lca (excluding lca), those from y, lca)
  // storage of retval is reused to avoid creating short vectors on each query
  tuple<vector<pair<int, int>>, vector<pair<int, int>>, int> paths_res;
  auto& paths(int x, int y) {
    auto& [x_paths, y_paths, lca] = paths_res;
    x_paths.clear();
    y_paths.clear();
    while (head[x] != head[y]) {
      int hx = head[x], hy = head[y];
      if (depth[hx] > depth[hy]) {
        x_paths.emplace_back(x, hx); x = parent[hx];
      } else {
        y_paths.emplace_back(y, hy); y = parent[hy];
      }
    }
    if (depth[x] > depth[y]) {
      x_paths.emplace_back(x, inv[idx[y] + 1]); x = y;
    } else if (depth[x] < depth[y]) {
      y_paths.emplace_back(y, inv[idx[x] + 1]); y = x;
    }
    lca = x;
    return paths_res;
  }
  int dist(int u, int v) {
    int w = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[w];
  }
  template <class F> int max_ancestor(int v, F f) {
    if (!f(v)) return -1;
    int hv = head[v];
    int p = parent[hv];
    while (p != -1 && f(p)) {
      v = p;
      hv = head[v];
      p = parent[hv];
    }
    int il = idx[hv] - 1, ir = idx[v];
    while (ir - il > 1) {
      int ic = (il + ir) / 2;
      (f(inv[ic]) ? ir : il) = ic;
    }
    return inv[ir];
  }
  int ascend(int v, int k) {
    assert(depth[v] >= k);
    int td = depth[v] - k;
    int hv = head[v];
    while (depth[hv] > td) {
      v = parent[hv];
      hv = head[v];
    }
    int rest = depth[v] - td;
    return inv[idx[v] - rest];
  }
};
