struct enum_k_bits {
  int n2, k, s;
  enum_k_bits(int n, int k) : n2(1 << n), k(k), s((1 << k) - 1) {}
  enum_k_bits begin() { return *this; }
  int end() const { return n2; }
  int& operator*() { return s; }
  enum_k_bits& operator++() {
    if (k == 0) {
      s = n2;
    } else {
      int x = s & -s;
      int y = s + x;
      s = (((s & ~y) / x) >> 1) | y;
    }
    return *this;
  }
  bool operator!=(int ub) const { return s < ub; }
};
