struct doubling {
  const int n;
  int k;
  vector<vector<int>> to;
  doubling(int n) : n(n), k(-1), to(1, vector<int>(n + 1, n)) {}
  void set_dest(int u, int v) {
    // u -> v
    to[0][u] = v;
  }
  // void build() { build(n); }  // removed; may be error-prone
  template<class T>
  void build(T d) {
    // move by d will be accepted later
    // k := floor_lg(d)
    k = 1;
    while ((T(1) << (k + 1)) <= d) k++;
    // prepare data area
    to.reserve(k);
    for (int i = to.size(); i < k; i++) {
      to.emplace_back(n + 1);
    }
    // construct doubling table
    for (int i = 1; i < k; i++) {
      for (int u = 0; u <= n; u++) {
        to[i][u] = to[i-1][to[i-1][u]];
      }
    }
  }
  template<class T>
  int move(int v, T d) {
    assert(k > 0);
    // returns -1 if move from v reaches endpoint before d step
    assert(d >= 0);
    for (int l = 0; l < k && d > 0; l++, d >>= 1) {
      if (d & 1) v = to[l][v];
    }
    assert(d == 0);
    if (v == n) v = -1;
    return v;
  }
  template <class F>
  int find_first_true(int v, F f) {
    assert(k > 0);
    // returns -1 if not found
    if (f(v)) return v;
    // f(v) keeps false below
    int res = -1;
    for (int i = k - 1; i >= 0; i--) {
      int w = to[i][v];
      if (w == n) continue;
      if (f(w)) res = w;
      else v = w;
    }
    return res;
  }
  template <class F>
  int find_last_true(int v, F f) {
    assert(k > 0);
    // returns -1 if f is already false at v
    if (!f(v)) return -1;
    // f(v) keeps true below
    for (int i = k - 1; i >= 0; i--) {
      int w = to[i][v];
      if (w == n) continue;
      if (f(w)) v = w;
    }
    return v;
  }
};