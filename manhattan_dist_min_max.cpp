template<class T>
vector<T> manhattan_dist_max(vector<pair<T, T>> ps) {
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
  int n = ps.size();
  vector<T> res;
  res.reserve(n);
  for(auto [x, y] : ps) {
    T s = x + y;
    T t = -x + y;
    res.emplace_back(max({s - v1, v2 - s, t - v3, v4 - t}));
  }
  return res;
}

template<class T>
vector<T> manhattan_dist_min(const vector<pair<T, T>>& points) {
  const int n = points.size();
  assert(n >= 2);
  struct S {
    T x, y;
    int i;
  };
  vector<S> ps(n);
  for (int i = 0; i < n; i++) {
    auto [x, y] = points[i];
    ps[i] = {x + y, -x + y, i};
  }
  sort(ps.begin(), ps.end(), [](const S& p1, const S& p2) {
    return p1.x < p2.x;
  });
  vector<T> vals(n);
  for (int i = 0; i < n; i++) vals[i] = ps[i].y;
  sort(vals.begin(), vals.end());
  vals.erase(unique(vals.begin(), vals.end()), vals.end());
  const int sz = vals.size();
  vector<int> ys(n);
  for (int i = 0; i < n; i++) {
    ys[i] = lower_bound(vals.begin(), vals.end(), ps[i].y) - vals.begin();
  }

  vector<T> ls(n, -1), rs(n);
  auto dist = [&](const S& p1, const S& p2) {
    return max(abs(p1.x - p2.x), abs(p1.y - p2.y));
  };
  for (int i = 1; i < n; i++) rs[0] = rs[i] = dist(ps[0], ps[i]);
  struct E {
    T x;
    bool t;
    int i;
    bool operator<(const E& rhs) const {
      return x < rhs.x || (x == rhs.x && t < rhs.t);
    }
  };
  vector<E> evs;
  fenwick_tree<int> ft(sz);
  vector<int> cnt(n), yl(n), yr(n);
  while(true) {
    evs.clear();
    for (int i = 0; i < n; i++) if (rs[i] - ls[i] > 1) {
      T c = (rs[i] + ls[i]) / 2;
      assert(c >= 0);
      yl[i] = lower_bound(vals.begin(), vals.end(), ps[i].y - c) - vals.begin();
      yr[i] = upper_bound(vals.begin(), vals.end(), ps[i].y + c) - vals.begin();
      evs.push_back(E{ps[i].x - c, false, i});
      evs.push_back(E{ps[i].x + c, true, i});
    }
    if (evs.empty()) break;
    sort(evs.begin(), evs.end());
    int ptr = 0;
    for (auto [x, t, i] : evs) {
      while(ptr < n && ((!t && ps[ptr].x < x) || (t && ps[ptr].x <= x))) {
        ft.add(ys[ptr++], 1);
      }
      int sm = ft.sum(yl[i], yr[i]);
      if (!t) {
        cnt[i] = sm;
      } else {
        assert(cnt[i] + 1 <= sm);
        (cnt[i] + 1 == sm ? ls[i] : rs[i]) = (ls[i] + rs[i]) / 2;
      }
    }
  }
  vector<T> res(n);
  for (int i = 0; i < n; i++) res[ps[i].i] = rs[i];
  return res;
}
