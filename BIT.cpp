
template<class S, auto e, auto op, bool dual=false, bool cursor_indexing=true>
struct BIT {
  vector<S> d;
  int n;
  template<class... Args>
  BIT(auto&&... args) : d(forward<decltype(args)>(args)...), n(d.size()) {
    if constexpr (dual) reverse(d.begin(), d.end());
    for (int i = 1; i < n; i++) {
      int j = i + (i & -i);
      if (j < n) d[j] = op(d[j], d[i]);
    }
  }
  BIT(int n) : d(n, e()), n(n) {}
  void apply(int i, S x) {
    if constexpr (cursor_indexing) {
      assert(0 <= i && i <= n);
      if constexpr (dual) i = n - i;
    } else {
      assert(0 <= i && i < n);
      if constexpr (dual) i = n - 1 - i;
    }
    if (i == 0) {
      if (n > 0) d[i] = op(d[i], x);
    } else {
      while (i < n) {
        d[i] = op(d[i], x);
        i += i & -i;
      }
    }
  }
  S sum(int i) {
    if constexpr (cursor_indexing) {
      assert(0 <= i && i <= n);
      if constexpr (dual) i = n - i;
      if (i == 0) return e();
      i--;
    } else {
      assert(0 <= i && i < n);
      if constexpr (dual) i = n - 1 - i;
    }
    S res = d[i];
    while (i) {
      i -= i & -i;
      res = op(res, d[i]);
    }
    return res;
  }
};
