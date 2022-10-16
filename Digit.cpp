template <int ZeroLength = 1, int Base = 10, int Buf = 20>
struct Digit {
  static_assert(ZeroLength <= 1);
  static_assert(2 <= Base && Base <= 256);
  unsigned char d[Buf];
  int k;
  template <class T>
  Digit(T x) : k(0) {
    assert(x >= 0);
    if constexpr (ZeroLength == 1) {
      if (x == 0) {
        d[k++] = 0;
        return;
      }
    }
    for (; x; x /= Base) d[k++] = x % Base;
  }
  Digit(const Digit& rhs) {
    k = rhs.k;
    memcpy(d, rhs.d, k * sizeof(unsigned char));
  }
  void reverse() {
    for (int i = 0, j = k - 1; i < j; i++, j--) {
      auto x = d[i] ^ d[j];
      d[i] ^= x;
      d[j] ^= x;
    }
    if constexpr (ZeroLength == 1) {
      while (k > 1 && d[k - 1] == 0) k--;
    } else {
      while (k > 0 && d[k - 1] == 0) k--;
    }
  }
  void increment() {
    for (int i = 0; i < k; i++) {
      if (d[i] != Base - 1) {
        d[i]++;
        return;
      }
      d[i] = 0;
    }
    d[k++] = 1;
  }
  template <class T>
  explicit operator T() {
    T res = 0;
    for (int i = k - 1; i >= 0; i--) {
      res = 10 * res + d[i];
    }
    return res;
  }
  friend ostream& operator<<(ostream& os, const Digit& x) {
    if constexpr (ZeroLength == 0) {
      if (x.k == 0) {
        cout << '0';
        return os;
      }
    }
    if constexpr (Base <= 10) {
      unsigned char out[Buf + 1];
      for (int i = 0; i < x.k; i++) out[i] = '0' | x.d[x.k - 1 - i];
      out[x.k] = '\0';
      os << out;
    } else {
      for (int i = x.k - 1; i >= 0; i--) {
        os << (int)x.d[i];
        if (i) os << ' ';
      }
    }
    return os;
  }
};
