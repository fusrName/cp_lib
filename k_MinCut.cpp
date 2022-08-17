template<class Cap>
struct k_MinCut {
  int n;
  int s, t;
  vector<int> idx, sizes;
  mf_graph<Cap> g;
  vector<Cap> a;
  Cap R;

  k_MinCut(vector<int> sizes) : n(sizes.size()), idx(n), R(0) {
    int now = 0;
    for (int i = 0; i < n; i++) {
      assert(sizes[i] > 0);
      idx[i] = now;
      now += sizes[i] - 1;
    }
    a.resize(now);
    s = now++;
    t = now++;
    g = mf_graph<Cap>(now);
    this->sizes = move(sizes);
  }

  template <class F1>
  void add_cost1(int i, F1 f1) {
    const int base = idx[i], sz = sizes[i];
    R += f1(0);
    for (int d = 0; d < sz - 1; d++) {
      a[base + d] += f1(d + 1) - f1(d);
    }
  }
  template <class F2>
  void add_cost2(int i, int j, F2 f2) {
    const int base_i = idx[i], base_j = idx[j], sz_i = sizes[i], sz_j = sizes[j];
    R += f2(0, 0);
    for (int d = 0; d < sz_i - 1; d++) {
      a[base_i + d] += f2(d + 1, 0) - f2(d, 0);
    }
    for (int e = 0; e < sz_j - 1; e++) {
      a[base_j + e] += f2(0, e + 1) - f2(0, e);
    }
    for (int d = 0; d < sz_i - 1; d++) {
      for (int e = 0; e < sz_j - 1; e++) {
        Cap cost = f2(d, e) - f2(d, e + 1) - f2(d + 1, e) + f2(d + 1, e + 1);
        assert(cost <= 0);
        if (!cost) continue;
        a[base_j + e] += cost;
        g.add_edge(base_i + d, base_j + e, -cost);
      }
    }
  }

  Cap min_cut() {
    for (int i = 0; i < n; i++) {
      const int base = idx[i], sz = sizes[i];
      for (int i = 0; i < sz - 1; i++) {
        if (a[base + i] > 0) {
          g.add_edge(s, base + i, a[base + i]);
        } else if (a[base + i] < 0) {
          R += a[base + i];
          g.add_edge(base + i, t, -a[base + i]);
        }
      }
      for (int i = 0; i < sz - 2; i++) {
        g.add_edge(base + i, base + i + 1, numeric_limits<Cap>::max());
      }
    }
    return g.flow(s, t) + R;
  }
};