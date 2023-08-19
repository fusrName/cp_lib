template<class T>
vector<T> manhattan_dist_max(const vector<pair<T, T>>& ps) {
  T v1, v2, v3, v4;
  v1 = v3 = numeric_limits<T>::max();
  v2 = v4 = numeric_limits<T>::min();
  for(auto [x, y] : ps) {
    T s = x + y;
    T t = -x + y;
    chmin(v1, s);
    chmax(v2, s);
    chmin(v3, t);
    chmax(v4, t);
  }
  vector<T> res(ps.size());
  int i = 0;
  for(auto [x, y] : ps) {
    T s = x + y;
    T t = -x + y;
    res[i++] = max({s - v1, v2 - s, t - v3, v4 - t});
  }
  return res;
}

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

template<class T>
vector<T> manhattan_dist_min(const vector<pair<T, T>>& points) {
  const int n = points.size();
  assert(n >= 2);
  struct S {
    pair<T, T> p;
    int i;
  };
  vector<S> ps(n);
  for (int i = 0; i < n; i++) ps[i] = {points[i], i};
  sort(ps.begin(), ps.end(), [](const S& p1, const S& p2) {
    return p1.p.first < p2.p.first;
  });
  vector<T> vals(n);
  for (int i = 0; i < n; i++) vals[i] = ps[i].p.second;
  sort(vals.begin(), vals.end());
  vals.erase(unique(vals.begin(), vals.end()), vals.end());
  const int sz = vals.size();

  constexpr T inf_min = numeric_limits<T>::min() / 2;
  constexpr T inf_max = numeric_limits<T>::max() / 2;
  vector<T> ans(n, inf_max);
  vector<int> y_id(n);
  auto e = []{ return inf_min; };
  auto op = [](T x, T y) { return max(x, y); };
  BIT<T, e, op, false, false> bt1(sz);
  BIT<T, e, op, true, false> bt2(sz);
  for (int i = 0; i < n; i++) {
    auto [x, y] = ps[i].p;
    y_id[i] = lower_bound(vals.begin(), vals.end(), y) - vals.begin();
    chmin(ans[ps[i].i], min(
      x + y - bt1.sum(y_id[i]),
      x - y - bt2.sum(y_id[i])
    ));
    bt1.apply(y_id[i], x + y);
    bt2.apply(y_id[i], x - y);
  }
  bt1.d.assign(sz, e());
  bt2.d.assign(sz, e());
  for (int i = n - 1; i >= 0; i--) {
    auto [x, y] = ps[i].p;
    chmin(ans[ps[i].i], min(
      -bt2.sum(y_id[i]) - (x + y),
      -bt1.sum(y_id[i]) - (x - y)
    ));
    bt2.apply(y_id[i], -(x + y));
    bt1.apply(y_id[i], -(x - y));
  }
  return ans;
}
