template <class T>
struct FPS : vector<T> {
  using vector<T>::vector;

  FPS& operator+=(const FPS& f) {
    const int tsz = this->size(), fsz = f.size();
    const int csz = min(tsz, fsz);
    for (int i = 0; i < csz; i++) (*this)[i] += f[i];
    if (csz < fsz) {
      this->insert(this->end(), f.begin() + csz, f.end());
    }
    return *this;
  }
  FPS& operator+=(FPS&& f) {
    if (this->size() < f.size()) swap(*this, f);
    *this += f;
    return *this;
  }
  friend FPS operator+(const FPS& f, const FPS& g) {
    FPS h;
    h.reserve(max(f.size(), g.size()));
    h = f;
    h += g;
    return h;
  }
  friend FPS operator+(FPS&& f, const FPS& g) { return move(f += g); }
  friend FPS operator+(const FPS& f, FPS&& g) { return move(g += f); }
  friend FPS operator+(FPS&& f, FPS&& g) { return move(f += move(g)); }

  FPS& operator-=(const FPS& f) {
    const int tsz = this->size(), fsz = f.size();
    const int csz = min(tsz, fsz);
    for (int i = 0; i < csz; i++) (*this)[i] -= f[i];
    if (csz < fsz) {
      this->resize(fsz);
      for (int i = csz; i < fsz; i++) (*this)[i] = -f[i];
    }
    return *this;
  }
  FPS& operator-=(FPS&& f) {
    const int tsz = this->size(), fsz = f.size();
    if (tsz > fsz) {
      *this -= f;
    } else {
      for (int i = 0; i < tsz; i++) f[i] = (*this)[i] - f[i];
      for (int i = tsz; i < fsz; i++) f[i] = -f[i];
      swap(*this, f);
    }
    return *this;
  }
  friend FPS operator-(const FPS& f, const FPS& g) {
    FPS h;
    h.reserve(max(f.size(), g.size()));
    h = f;
    h -= g;
    return h;
  }
  friend FPS operator-(FPS&& f, const FPS& g) { return move(f -= g); }
  friend FPS operator-(const FPS& f, FPS&& g) {
    const int fsz = f.size(), gsz = g.size();
    const int csz = min(fsz, gsz);
    for (int i = 0; i < csz; i++) g[i] = f[i] - g[i];
    if (fsz < gsz) {
      for (int i = csz; i < gsz; i++) g[i] = -g[i];
    } else {
      g.insert(g.end(), f.begin() + csz, f.end());
    }
    return move(g);
  }
  friend FPS operator-(FPS&& f, FPS&& g) { return move(f -= move(g)); }
  FPS operator+() && { return move(*this); }
  FPS operator+() const& { return *this; }
  FPS operator-() && {
    for (T& x: *this) x = -x;
    return move(*this);
  }
  FPS operator-() const& {
    FPS f;
    f.reserve(this->size());
    for (const T& x: *this) f.emplace_back(-x);
    return f;
  }

  static void multiply_naive(FPS& f, const FPS& g) {
    int n = int(f.size()), m = int(g.size());
    if (n < m) {
      tmp1 = f;
      f.assign(n + m - 1, T());
      for (int j = m - 1; j >= 0; j--) {
        for (int i = n - 1; i >= 0; i--) {
          f[i + j] += tmp1[i] * g[j];
        }
      }
    } else {
      f.resize(n + m - 1);
      for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j > 0; j--) {
          f[i + j] += f[i] * g[j];
        }
        f[i] *= g[0];
      }
    }
  }

  static void multiply_fft(FPS& f, const FPS& g) {
    int z = 1 << internal::ceil_pow2(n + m - 1);
    tmp1.reserve(z), tmp1 = g, tmp1.resize(z), internal::butterfly(tmp1);
    f.resize(z), internal::butterfly(f);
    for (int i = 0; i < z; i++) f[i] *= g[i];
    internal::butterfly_inv(f);
    f.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) f[i] *= iz;
  }

  FPS& operator*=(const FPS& f) {
    int n = int((*this).size()), m = int(f.size());
    if (!n || !m) {
      this->clear();
      return *this;
    };
    if (min(n, m) <= 60) multiply_naive(*this, f);
    else multiply_fft(*this, f);
    return *this;
  }
  FPS& operator*=(FPS&& f) {
    int n = int((*this).size()), m = int(f.size());
    if (this->size() < f.size()) swap(*this, f);
    return *this *= f;
  }
  friend FPS operator*(FPS&& f, FPS&& g) { return move(f *= move(g)); }
  friend FPS operator*(FPS&& f, const FPS& g) { return move(f *= g); }
  friend FPS operator*(const FPS& f, FPS&& g) { return move(g *= f); }
  friend FPS operator*(const FPS& f, const FPS& g) {
    int n = int(f.size()), m = int(g.size());
    if (!n || !m) return {};
    int z = 1 << internal::ceil_pow2(n + m - 1);
    FPS h;
    h.reserve(z);
    h = f;
    h *= g;
    return h;
  }
  FPS& operator+=(T x) {
    if ((*this).empty()) (*this).emplace_back(x);
    else (*this)[0] += x;
    return *this;
  }
  friend FPS operator+(FPS f, T x) {
    f += x;
    return f;
  }
  friend FPS operator+(T x, FPS f) {
    f += x;
    return f;
  }
  FPS& operator-=(T x) {
    if ((*this).empty()) (*this).emplace_back(-x);
    else (*this)[0] -= x;
    return *this;
  }
  friend FPS operator-(FPS f, T x) {
    f -= x;
    return f;
  }
  friend FPS operator-(T x, FPS f) { return -move(f) + x; }
  FPS& operator*=(T x) {
    for (T& i: *this) i *= x;
    return *this;
  }
  friend FPS operator*(FPS f, T x) {
    f *= x;
    return f;
  }
  friend FPS operator*(T x, FPS f) {
    f *= x;
    return f;
  }
  FPS operator/=(T x) {
    if constexpr (internal::is_modint<T>::value) {
      T ix = x.inv();
      for (T& i: *this) i *= ix;
    } else {
      for (T& i: *this) i /= x;
    }
    return *this;
  }
  friend FPS operator/(FPS f, T x) {
    f /= x;
    return f;
  }
  FPS& operator<<=(int x) {
    assert(x >= 0);
    this->insert((*this).begin(), x, T());
    return *this;
  }
  friend FPS operator<<(const FPS& f, int x) {
    assert(x >= 0);
    const int fsz = f.size();
    FPS res;
    res.reserve(fsz + x);
    res.resize(x);
    res.insert(res.end(), f.begin(), f.end());
    return res;
  }
  friend FPS operator<<(FPS&& f, int x) {
    f <<= x;
    return move(f);
  }
  FPS& operator>>=(int x) {
    assert(x >= 0);
    const int tsz = this->size();
    if (x >= tsz) this->clear();
    else this->erase(this->begin(), this->begin() + x);
    return *this;
  }
  friend FPS operator>>(const FPS& f, int x) {
    assert(x >= 0);
    const int fsz = f.size();
    if (x >= fsz) return {};
    else return FPS(f.begin() + x, f.end());
  }
  friend FPS operator>>(FPS&& f, int x) {
    f >>= x;
    return move(f);
  }

  static FPS inv_proc(const FPS& f, FPS g, int len) {
    if (len == 0) return g;
    FPS fi, gi;
    swap(fi, tmp2);
    swap(gi, tmp3);
    const int z = 1 << internal::ceil_pow2(len);
    const int n = f.size();
    g.clear();
    g.reserve(z);
    fi.reserve(z);
    gi.reserve(z);

    assert(len >= 1 && n >= 1 && f[0] != T());
    g.emplace_back(f[0].inv());
    const T inv2 = T(2).inv();
    T i2k = 1;
    int m = 1;
    while (m != z) {
      const int m2 = 2 * m;
      fi.assign(f.begin(), f.begin() + min(n, m2));
      fi.resize(m2);
      internal::butterfly(fi);
      gi = g;
      gi.resize(m2);
      internal::butterfly(gi);
      for (int i = 0; i < m2; i++) fi[i] *= gi[i];
      internal::butterfly_inv(fi);
      fi.erase(fi.begin(), fi.begin() + m);
      fi.resize(m2);
      internal::butterfly(fi);
      for (int i = 0; i < m2; i++) fi[i] *= gi[i];
      internal::butterfly_inv(fi);
      i2k *= inv2;
      T c = -i2k * i2k;
      for (int i = 0; i < m; i++) {
        g.emplace_back(c * fi[i]);
      }
      m = m2;
    }
    g.resize(len);

    swap(fi, tmp2);
    swap(gi, tmp3);
    return g;
  }
  FPS inv(int len) const& {
    FPS g;
    return inv_proc(*this, FPS(), len);
  }
  FPS inv(int len) && {
    tmp = *this;
    return inv_proc(tmp, move(*this), len);
  }

  static FPS Taylor_Shift_proc(FPS f, T c) {
    int n = f.size();
    FPS g;
    swap(g, tmp2);
    g.clear();
    g.resize(n);
    for (int i = 0; i < n; i++) f[i] *= Fact[i];
    T ck = 1;
    for (int i = 0, j = n - 1; i < n; i++, j--, ck *= c) g[j] = ck * iFact[i];
    f *= g;
    for (int i = 0, j = n - 1; i < n; i++, j++) f[i] = iFact[i] * f[j];
    f.resize(n);
    swap(g, tmp2);
    return f;
  }
  FPS Taylor_Shift(int c) const& { return Taylor_Shift_proc(*this, c); }
  FPS Taylor_Shift(int c) && {
    int n = (*this).size();
    if (n <= 1) return move(*this);
    FPS f;
    int z = 1 << internal::ceil_pow2(2 * n - 1);
    f.reserve(z);
    f = *this;
    return Taylor_Shift_proc(move(f), c);
  }

  template <char sep = ' ', char end = '\n'>
  void print() const {
    int sz = (*this).size();
    if (sz) {
      cout << (*this)[0].val();
      for (int i = 1; i < sz; i++) cout << sep << (*this)[i].val();
    }
    cout << end;
  }

 private:
  inline static FPS tmp, tmp2, tmp3;
};
