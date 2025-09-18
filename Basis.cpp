template <unsigned_integral T>
struct Basis {
  vector<T> bs;
  bool add(T x) {
    for (T b : bs) chmin(x, x ^ b);
    if (!x) return false;
    int i = ranges::lower_bound(bs, x) - bs.begin();
    bs.insert(bs.begin() + i, x);
    for (auto& b : bs | views::drop(i + 1)) chmin(b, b ^ x);
    return true;
  }
  int dim() { return bs.size(); }
  T spsize() {
    assert(dim() < numeric_limits<T>::digits);
    return T(1) << dim();
  }
  T get_kth(T k) {
    assert(bit_width(k) <= dim());
    T res = 0;
    for (int i = 0, n = bs.size(); i < n; i++) {
      if (k >> i & 1) res ^= bs[i];
    }
    return res;
  }
  // returns (idx in asc order, val); val is smallest s.t. val >= x
  optional<pair<T, T>> lower_bound(T x) {
    T idx = 0, val = 0;
    for (auto b : bs) val ^= b;
    if (val < x) return nullopt;
    for (int i = bs.size(); i--;) {
      T nv = val ^ bs[i];
      if (nv >= x) val = nv;
      else idx |= T(1) << i;
    }
    return pair(idx, val);
  }
};
