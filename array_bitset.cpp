template <unsigned int max_size>
struct array_bitset {
  using ull = uint64_t;
  static constexpr int capacity = (max_size + 63) / 64 + 1;
  ull d[capacity] = {};

  template <unsigned int x>
  void operator=(const array_bitset<x>& rhs) {
    size = rhs.size;
    data_size = rhs.data_size;
    memcpy(d, rhs.d, sizeof(ull) * data_size);
  }
  template <unsigned int x>
  void operator&=(const array_bitset<x>& rhs) {
    assert(size == rhs.size);
    for (int i = 0; i < data_size; i++) d[i] &= rhs.d[i];
  }
  template <unsigned int x>
  void operator|=(const array_bitset<x>& rhs) {
    assert(size == rhs.size);
    for (int i = 0; i < data_size; i++) d[i] |= rhs.d[i];
  }
  void operator<<=(int pos) {
    if (pos >= size) return reset();
    if (pos == 0) return;
    const int a = pos / 64, b = pos % 64;
    if (b == 0) {
      for (int i = data_size - 1 - a; i >= 0; i--) {
        d[i + a] = d[i];
      }
      memset(&d[0], 0, sizeof(ull) * a);
    } else {
      int ib = 64 - b;
      if (a == 0) {
        for (int i = data_size - 1; i >= 0; i--) {
          d[i + 1] |= d[i] >> ib;
          d[i] <<= b;
        }
      } else {
        for (int i = data_size - 1 - a; i >= 0; i--) {
          d[i + a + 1] |= d[i] >> ib;
          d[i + a] = d[i] << b;
        }
        memset(&d[0], 0, sizeof(ull) * a);
      }
    }
    int r = size % 64;
    if (r) d[data_size - 1] &= ~(~ull(0) << r);
  }
  void operator>>=(int pos) {
    if (pos >= size) return reset();
    if (pos == 0) return;
    const int a = pos / 64, b = pos % 64;
    if (b == 0) {
      for (int i = a; i < data_size; i++) {
        d[i - a] = d[i];
      }
      memset(&d[data_size - a], 0, sizeof(ull) * a);
    } else {
      int ib = 64 - b;
      if (a == 0) {
        d[0] >>= b;
        for (int i = 1; i < data_size; i++) {
          d[i - 1] |= d[i] << ib;
          d[i] >>= b;
        }
      } else {
        d[0] = d[a] >> b;
        for (int i = a + 1; i < data_size; i++) {
          d[i - a - 1] |= d[i] << ib;
          d[i - a] = d[i] >> b;
        }
        memset(&d[data_size - a], 0, sizeof(ull) * a);
      }
    }
  }
  bool operator[](int p) {
    const int a = p / 64, b = p % 64;
    return d[a] >> b & 1;
  }

  int find_first() {
    for (int i = 0; i < data_size; i++) if (d[i]) {
      return 64 * i + __builtin_ctzll(d[i]);
    }
    return -1;
  }
  int find_next(int p) {
    p++;
    if (p >= size) return -1;
    const int a = p / 64, b = p % 64;
    ull x = d[a] & (~ull(0) << b);
    if (x) return 64 * a + __builtin_ctzll(x);
    for (int i = a + 1; i < data_size; i++) if (d[i]) {
      return 64 * i + __builtin_ctzll(d[i]);
    }
    return -1;
  }

  void set(int p) {
    const int a = p / 64, b = p % 64;
    d[a] |= ull(1) << b;
  }
  void reset() { memset(&d[0], 0, sizeof(ull) * data_size); }

  void set_size(int sz) {
    size = sz;
    data_size = (sz + 63) / 64;
  }

 private:
  int size = max_size;
  int data_size = capacity - 1;
};