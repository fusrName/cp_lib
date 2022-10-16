template <class T, class C = long long>
struct run_length_encoding {
  vector<pair<T, C>> d;

  run_length_encoding() {}
  template <class Iter>
  run_length_encoding(Iter first, Iter last) {
    while (first != last) {
      auto v = *first++;
      int cnt = 1;
      while (first != last && *first == v) {
        first++;
        cnt++;
      }
      d.emplace_back(v, cnt);
    }
  }
  void push_back(T v, C count) {
    assert(count >= 0);
    if (count == 0) return;
    if (!d.empty() && d.back().first == v) {
      d.back().second += count;
    } else {
      d.emplace_back(v, count);
    }
  }
  bool operator<(const run_length_encoding& rhs) const {
    int i = 0;
    int ub = min(d.size(), rhs.d.size());
    while (i < ub && d[i] == rhs.d[i]) i++;
    if (i == ub) return d.size() < rhs.d.size();
    auto [vl, cl] = d[i];
    auto [vr, cr] = rhs.d[i];
    if (vl != vr) return vl < vr;
    if (cl < cr) {
      return i + 1 == int(d.size()) || d[i + 1].first < vr;
    } else {
      return i + 1 < int(rhs.d.size()) && vl < rhs.d[i + 1].first;
    }
  }
};
