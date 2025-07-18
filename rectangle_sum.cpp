template <class T>
struct V {
  int i, j;
  T v;
};
struct Q { int il, ir, jl, jr; };
template <class T, class ResT=T>
auto rectangle_sum(vector<V<T>> points, vector<Q> queries) {
  ranges::sort(points, {}, &V<T>::i);
  vector<int> vc;
  vc.reserve(points.size());
  for (const auto& p : points) vc.emplace_back(p.j);
  ranges::sort(vc);
  vc.erase(unique(vc.begin(), vc.end()), vc.end());
  struct Ask {
    int i, iq;
    bool plus;
  };
  vector<Ask> asks(2 * queries.size());
  for (int iq = 0; iq < ssize(queries); iq++) {
    asks[2 * iq] = {queries[iq].il, iq, false};
    asks[2 * iq + 1] = {queries[iq].ir, iq, true};
  }
  ranges::sort(asks, {}, &Ask::i);
  const int sz = vc.size();
  vector<ResT> ft(sz);
  auto add = [&](int p, T x) {
    assert(0 <= p && p < sz);
    for (p++; p <= sz; p += p & -p) ft[p-1] += x;
  };
  auto sum = [&](int r) {
    assert(0 <= r && r <= sz);
    ResT s{};
    for (; r > 0; r -= r & -r) s += ft[r-1];
    return s;
  };
  int ptr = 0;
  vector<ResT> ans(queries.size());
  for (auto [i, iq, plus] : asks) {
    for (; ptr < ssize(points) && points[ptr].i < i; ptr++) {
      add(ranges::lower_bound(vc, points[ptr].j) - vc.begin(), points[ptr].v);
    }
    int l = ranges::lower_bound(vc, queries[iq].jl) - vc.begin();
    int r = ranges::lower_bound(vc, queries[iq].jr) - vc.begin();
    if (plus) ans[iq] += sum(r) - sum(l);
    else ans[iq] -= sum(r) - sum(l);
  }
  return ans;
}
