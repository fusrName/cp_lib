struct range_edge {  // based on DST-like indexing
  const int L, R, offset;
  const vector<unsigned char> msb;

  range_edge(int L, int R, int offset): L(L), R(R), offset(offset), msb(build_msb_table(R - L)) {
    assert(L <= R);
  }
  range_edge(int R): range_edge(0, R, R) {}

  vector<unsigned char> build_msb_table(int n) {
    if (n <= 1) return {};
    unsigned char k_max = 32 - __builtin_clz(n - 1);
    vector<unsigned char> res(1 << k_max);
    unsigned char* p = res.data();
    for (unsigned char k = 0; k < k_max; k++) {
      memset(p + (1U << k), k, 1U << k);
    }
    return res;
  }
  int size() {
    int n = R - L;
    if (n <= 1) return 0;
    int k = 31 - __builtin_clz(n - 1);
    return k * n;
  }
  vector<pair<int, int>> edges() {
    const int n = R - L;
    if (n <= 1) return {};
    vector<pair<int, int>> res;
    int k_max = 32 - __builtin_clz(n - 1);
    for (int k = 1, os = offset; k < k_max; k++, os += n) {
      for (int i = 0; i < n; i++) res.emplace_back(os + i, L + i);
      for (int c = 1 << k; c < n; c += 1 << (k + 1)) {
        int l = c - (1 << k);
        int r = min(n, c + (1 << k));
        for (int i = c - 2; i >= l; i--) res.emplace_back(os + i, os + i + 1);
        for (int i = c + 1; i < r; i++) res.emplace_back(os + i, os + i - 1);
      }
    }
    return res;
  }
  template <class F>
  void edges_apply(F f) {
    const int n = R - L;
    if (n <= 1) return;
    int k_max = 32 - __builtin_clz(n - 1);
    for (int k = 1, os = offset; k < k_max; k++, os += n) {
      for (int i = 0; i < n; i++) f(os + i, L + i);
      for (int c = 1 << k; c < n; c += 1 << (k + 1)) {
        int l = c - (1 << k);
        int r = min(n, c + (1 << k));
        for (int i = c - 2; i >= l; i--) f(os + i, os + i + 1);
        for (int i = c + 1; i < r; i++) f(os + i, os + i - 1);
      }
    }
  }
  array<int, 2> query_closed(int l, int r) {
    assert(L <= l && l <= r && r < R);
    if (l == r) return {l, -1};
    else if (l + 1 == r) return {l, r};
    l -= L, r -= L;
    int k = msb[l ^ r];
    int os = offset + (R - L) * (k - 1);
    return {os + l, os + r};
  }
  array<int, 2> query(int l, int r) {
    return l == r ? array{-1, -1} : query_closed(l, r - 1);
  }
};



struct range_edge {  // based on segtree-like indexing
  int size, log, n;
  range_edge(int n) : n(n) {
    size = std::bit_ceil((unsigned int)(n));
    log = std::countr_zero((unsigned int)size);
  }
  template <class F>
  void edges_apply(F f) {
    for (int i = size - 1; i >= 1; i--) {
      f(i, 2 * i);
      f(i, 2 * i + 1);
    }
  }
  template <class F>
  void query_apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= n);
    l += size;
    r += size;
    while (l < r) {
      if (l & 1) f(l++);
      if (r & 1) f(--r);
      l >>= 1;
      r >>= 1;
    }
  }
  int get(int p) const {
      assert(0 <= p && p < n);
      return p + size;
  }
};
