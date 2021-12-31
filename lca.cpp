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

template <class S, S (*op)(S, S), S (*e)()> struct SparseTable {
  int n;
  vector<int> floor_lg;
  vector<vector<S>> d;
  SparseTable() {}
  explicit SparseTable(vector<S>&& a): n(a.size()), floor_lg(n + 1) {
    if (n == 0) return;
    int frog = 0;
    for(int i = 1; i <= n; frog++) {
      int j = min(n + 1, 2 * i);
      for(; i < j; i++) floor_lg[i] = frog;
    }
    d.resize(frog);
    d[0] = move(a);
    for(int p = 0, w = 1; p < frog - 1; p++, w *= 2) {
      int last = n - 2 * w;
      d[p + 1].resize(last + 1);
      for(int i = 0; i <= last; i++) d[p + 1][i] = op(d[p][i], d[p][i + w]);
    }
  }
  explicit SparseTable(const vector<S>& a): SparseTable(vector<S>(a)) {}

  S query(int l, int r) {
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return e();
    int w = r - l;
    int p = floor_lg[w];
    return op(d[p][l], d[p][r - (1 << p)]);
  }
};

struct IdxDepth { int i, d; };
IdxDepth op_idx_depth(IdxDepth x, IdxDepth y) { return x.d < y.d ? x : y; }
IdxDepth e_idx_depth() { return {-1, numeric_limits<int>::max()}; }

struct LCA {
  using RmQ = SparseTable<IdxDepth, op_idx_depth, e_idx_depth>;
  const int n;
  vector<int> depth;
  const vector<int> tour;
  vector<int> pos;
  RmQ rmq;
  LCA(const vector<vector<int>>& tree, int root=0)
      : n(tree.size()), depth(n, -1), tour(euler_tour_vertices(tree, root)), pos(n) {
    vector<int> st{root};
    st.reserve(n);
    depth[root] = 0;
    while(!st.empty()) {
      int u = st.back(); st.pop_back();
      for(int v: tree[u]) {
        if (depth[v] == -1) {
          depth[v] = depth[u] + 1;
          st.push_back(v);
        }
      }
    }
    vector<IdxDepth> a(tour.size());
    for(int i = 0; i < (int)tour.size(); i++) {
      int v = tour[i];
      a[i] = {v, depth[v]};
      pos[v] = i;
    }
    rmq = RmQ(move(a));
  }
  int operator()(int u, int v) {
    int pu = pos[u], pv = pos[v];
    if (pu > pv) swap(pu, pv);
    return rmq.query(pu, pv + 1).i;
  }
  int get_dist(int u, int v) {
    int c = (*this)(u, v);
    return depth[u] + depth[v] - 2 * depth[c];
  }
};


// Verification

// // https://judge.yosupo.jp/problem/lca
// #define rep(i,n)for (int i = 0; i < int(n); ++i)
// using VI = vector<int>;
// using VVI = vector<VI>;
// 
// int main() {
//   using namespace std;
//   ios::sync_with_stdio(false);
//   cin.tie(0);
//   int n, q;
//   cin >> n >> q;
//   VVI to(n);
//   for(int i = 1; i < n; i++) {
//     int p;
//     cin >> p;
//     to[i].push_back(p);
//     to[p].push_back(i);
//   }
//   LCA lca(to);
//   while(q--) {
//     int u, v;
//     cin >> u >> v;
//     cout << lca(u, v) << '\n';
//   }
// }