#pragma GCC target("popcnt")

struct WaveletMatrix {
  int height = 0;
  int n = 0;
  using storage_type = unsigned int;
  static constexpr int B = numeric_limits<storage_type>::digits,
                       LowBits = __lg(B);
  static_assert(B == 1 << LowBits);
  struct BVecElem {
    storage_type v;
    int acc;
  };
  struct BitVector;
  vector<BVecElem> data;
  vector<BitVector> bvecs;
  struct BitVector {
    span<BVecElem> d;
    int zeros;
    BitVector(span<BVecElem> d, int k, const auto& a, auto& na) : d(d) {
      int n = size(a);
      int ptr0 = 0, ptr1 = n;
      for (int i = 0; i < n; i += B) {
        int len = min(B, n - i);
        storage_type x = 0;
        for (int i_low = 0; i_low < len; i_low++) {
          bool bit = a[i | i_low] >> k & 1;
          if (!bit) {
            na[ptr0++] = a[i | i_low];
          } else {
            x |= storage_type(bit) << i_low;
            na[--ptr1] = a[i | i_low];
          }
        }
        d[i >> LowBits].v = x;
      }
      assert(ptr0 == ptr1);
      zeros = ptr0;
      reverse(na.begin() + ptr0, na.begin() + n);
      for (int i = 0, sz = size(d); i + 1 < sz; i++) {
        d[i + 1].acc = d[i].acc + popcount(d[i].v);
      }
    }
    int rank1(int i) {
      auto di = d[i >> LowBits];
      int low = i & (B - 1);
      auto mask = (storage_type(1) << low) - 1;
      return di.acc + popcount(di.v & mask);
    }
    int rank(bool b, int i) { return b == 0 ? i - rank1(i) : rank1(i); }
    int mv(bool b, int i) { return b == 0 ? i - rank1(i) : zeros + rank1(i); }
  };

  WaveletMatrix() = default;
  template <class T>
  WaveletMatrix(vector<T> a) {
    auto mx = empty(a) ? 0 : *max_element(begin(a), end(a));
    height = mx == 0 ? 0 : __lg(mx) + 1;
    n = size(a);
    int block_count = (n >> LowBits) + 1;
    data.resize(height * block_count);
    bvecs.reserve(height);
    vector<T> na(n);
    for (int k = height - 1; k >= 0; k--) {
      bvecs.emplace_back(span(begin(data) + k * block_count, block_count), k, a,
                         na);
      swap(a, na);
    }
    reverse(begin(bvecs), end(bvecs));
  };

  // returns the number of occurrence of x in a[l, r)
  int rank(auto x, int l, int r) {
    assert(0 <= l && l <= r && r <= n);
    if (x >> height) return 0;
    for (int k = height - 1; k >= 0; k--) {
      bool bit = x >> k & 1;
      l = bvecs[k].mv(bit, l);
      r = bvecs[k].mv(bit, r);
    }
    return r - l;
  }
  int rangefreq_below(auto vr, int l, int r) {
    assert(0 <= l && l <= r && r <= n);
    if (vr <= 0) return 0;
    if (vr >> height) return r - l;
    int res = 0;
    for (int k = height - 1; k >= 0; k--) {
      bool bit = vr >> k & 1;
      if (bit) {
        int ones_r = bvecs[k].rank1(r), ones_l = bvecs[k].rank1(l);
        res += (r - l) - (ones_r - ones_l);
        l = bvecs[k].zeros + ones_l;
        r = bvecs[k].zeros + ones_r;
      } else {
        l = bvecs[k].mv(bit, l);
        r = bvecs[k].mv(bit, r);
      }
    }
    return res;
  }
  // returns the number of occurrence of values in [vl, vr) in a[l, r)
  int rangefreq(auto vl, auto vr, int l, int r) {
    assert(vl <= vr);
    return rangefreq_below(vr, l, r) - rangefreq_below(vl, l, r);
  }
  // returns the maximum vr s.t. # of points in [l, r) * [vl, vr) <= cnt_ub
  template <class T>
  T max_upper(T vl, int l, int r, int cnt_ub, T res_for_inf) {
    assert(0 <= l && l <= r && r <= n);
    assert(cnt_ub >= 0);
    assert(numeric_limits<T>::digits > height);
    cnt_ub += rangefreq_below(vl, l, r);
    if (cnt_ub >= r - l) return res_for_inf;
    T res = 0;
    for (int k = height - 1; k >= 0; k--) {
      int ones_l = bvecs[k].rank1(l), ones_r = bvecs[k].rank1(r);
      int zeros = (r - l) - (ones_r - ones_l);
      if (zeros <= cnt_ub) {
        cnt_ub -= zeros;
        res += T(1) << k;
        l = bvecs[k].zeros + ones_l;
        r = bvecs[k].zeros + ones_r;
      } else {
        l = l - ones_l;
        r = r - ones_r;
      }
    }
    return res;
  }
  template <class T>
  T min_lower(T vr, int l, int r, int cnt_ub) {
    assert(0 <= l && l <= r && r <= n);
    assert(cnt_ub >= 0);
    assert(numeric_limits<T>::digits > height);
    cnt_ub += (r - l) - rangefreq_below(vr, l, r);
    if (cnt_ub >= r - l) return 0;
    T res = T(1) << height;
    for (int k = height - 1; k >= 0; k--) {
      int ones_l = bvecs[k].rank1(l), ones_r = bvecs[k].rank1(r);
      int ones = ones_r - ones_l;
      if (ones <= cnt_ub) {
        cnt_ub -= ones;
        res -= T(1) << k;
        l = l - ones_l;
        r = r - ones_r;
      } else {
        l = bvecs[k].zeros + ones_l;
        r = bvecs[k].zeros + ones_r;
      }
    }
    return res;
  }
};
