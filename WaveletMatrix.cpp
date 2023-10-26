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

  int rangefreq_below(auto xr, int l, int r) {
    assert(0 <= l && l <= r && r <= n);
    if (xr <= 0) return 0;
    if (xr >> height) return r - l;
    int res = 0;
    for (int k = height - 1; k >= 0; k--) {
      bool bit = xr >> k & 1;
      if (bit) res += (r - l) - (bvecs[k].rank1(r) - bvecs[k].rank1(l));
      l = bvecs[k].mv(bit, l);
      r = bvecs[k].mv(bit, r);
    }
    return res;
  }
  // returns the number of occurrence of values in [xl, xr) in a[l, r)
  int rangefreq(auto xl, auto xr, int l, int r) {
    assert(xl <= xr);
    return rangefreq_below(xr, l, r) - rangefreq_below(xl, l, r);
  }
};
