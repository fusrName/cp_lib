template<class Cap>
struct k_MinCut {
  mf_graph<Cap> g;
  Cap res;

  template<class F1, class F2>
  k_MinCut(vector<int> sizes, F1 f1, F2 f2) {
    const int n = sizes.size();
    vector<int> idx_head(n);
    int sz = 0;
    for (int i = 0; i < n; i++) {
      assert(sizes[i] > 0);
      idx_head[i] = sz;
      sz += sizes[i] - 1;
    }
    int s = sz++;
    int t = sz++;
    g = mf_graph<Cap>(sz);

    Cap R = 0;
    vector<Cap> a(sz - 2);
    for (int i = 0; i < n; i++) {
      R += f1(i, 0);
      for (int d = 0; d < sizes[i] - 1; d++) {
        a[idx_head[i] + d] = f1(i, d + 1) - f1(i, d);
      }
      for (int d = 0; d < sizes[i] - 2; d++) {
        int xid = idx_head[i] + d;
        g.add_edge(xid, xid + 1, numeric_limits<Cap>::max());
      }
    }
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        R += f2(i, 0, j, 0);
        for (int d = 0; d < sizes[i] - 1; d++) {
          a[idx_head[i] + d] += f2(i, d + 1, j, 0) - f2(i, d, j, 0);
        }
        for (int e = 0; e < sizes[j] - 1; e++) {
          a[idx_head[j] + e] += f2(i, 0, j, e + 1) - f2(i, 0, j, e);
        }
        for (int d = 0; d < sizes[i] - 1; d++) {
          for (int e = 0; e < sizes[j] - 1; e++) {
            Cap c = f2(i, d, j, e) - f2(i, d, j, e + 1) - f2(i, d + 1, j, e) + f2(i, d + 1, j, e + 1);
            assert(c <= 0);
            int xid = idx_head[i] + d, xje = idx_head[j] + e;
            a[xje] += c;
            g.add_edge(xid, xje, -c);
          }
        }
      }
    }
    for (int i = 0; i < n; i++) {
      for (int d = 0; d < sizes[i] - 1; d++) {
        int x = idx_head[i] + d;
        if (a[x] > 0) {
          g.add_edge(s, x, a[x]);
        } else if (a[x] < 0) {
          R += a[x];
          g.add_edge(x, t, -a[x]);
        }
      }
    }
    res = R + g.flow(s, t);
  }
};