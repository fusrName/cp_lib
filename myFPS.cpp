template <class T>
struct FPS : vector<T> {
  using vector<T>::vector;

  FPS& operator+=(const FPS& f) {
    int tsz = (*this).size(), fsz = f.size();
    if (tsz < fsz) {
      (*this).insert((*this).end(), f.begin() + tsz, f.end());
      while (tsz--) (*this)[tsz] += f[tsz];
    } else {
      while (fsz--) (*this)[fsz] += f[fsz];
    }
    return *this;
  }
  FPS& operator+=(FPS&& f) {
    int tsz = (*this).size(), fsz = f.size();
    if (tsz < fsz) {
      swap(*this, f);
      while (tsz--) (*this)[tsz] += f[tsz];
    } else {
      while (fsz--) (*this)[fsz] += f[fsz];
    }
    return *this;
  }
  friend FPS operator+(const FPS& f, const FPS& g) {
    int fsz = f.size(), gsz = g.size();
    FPS h;
    if (fsz < gsz) {
      h.reserve(gsz);
      for (int i = 0; i < fsz; i++) h.emplace_back(f[i] + g[i]);
      h.insert(h.end(), g.begin() + fsz, g.end());
    } else {
      h.reserve(fsz);
      for (int i = 0; i < gsz; i++) h.emplace_back(f[i] + g[i]);
      h.insert(h.end(), f.begin() + gsz, f.end());
    }
    return h;
  }
  friend FPS operator+(FPS&& f, const FPS& g) {
    f += g;
    return move(f);
  }
  friend FPS operator+(const FPS& f, FPS&& g) {
    g += f;
    return move(g);
  }
  friend FPS operator+(FPS&& f, FPS&& g) {
    f += move(g);
    return move(f);
  }

  FPS& operator-=(const FPS& f) {
    int tsz = (*this).size(), fsz = f.size();
    if (tsz < fsz) {
      (*this).reserve(fsz);
      for (int i = tsz; i < fsz; i++) {
        (*this).emplace_back(-f[i]);
      }
      while (tsz--) (*this)[tsz] -= f[tsz];
    } else {
      while (fsz--) (*this)[fsz] -= f[fsz];
    }
    return *this;
  }
  FPS& operator-=(FPS&& f) {
    int tsz = (*this).size(), fsz = f.size();
    if (tsz < fsz) {
      swap(*this, f);
      while (fsz-- != tsz) (*this)[fsz] = -(*this)[fsz];
      while (tsz--) (*this)[tsz] = f[tsz] - (*this)[tsz];
    } else {
      while (fsz--) (*this)[fsz] -= f[fsz];
    }
    return *this;
  }
  friend FPS operator-(const FPS& f, const FPS& g) {
    int fsz = f.size(), gsz = g.size();
    FPS h;
    if (fsz < gsz) {
      h.reserve(gsz);
      int i = 0;
      for (; i < fsz; i++) h.emplace_back(f[i] - g[i]);
      for (; i < gsz; i++) h.emplace_back(-g[i]);
    } else {
      h.reserve(fsz);
      for (int i = 0; i < gsz; i++) h.emplace_back(f[i] - g[i]);
      h.insert(h.end(), f.begin() + gsz, f.end());
    }
    return h;
  }
  friend FPS operator-(FPS&& f, const FPS& g) {
    f -= g;
    return move(f);
  }
  friend FPS operator-(const FPS& f, FPS&& g) {
    int fsz = f.size(), gsz = g.size();
    if (fsz < gsz) {
      while (gsz-- != fsz) g[gsz] = -g[gsz];
      while (fsz--) g[fsz] = f[fsz] - g[fsz];
    } else {
      g.insert(g.end(), f.begin() + gsz, f.end());
      while (gsz--) g[gsz] = f[gsz] - g[gsz];
    }
    return move(g);
  }
  friend FPS operator-(FPS&& f, FPS&& g) {
    f -= move(g);
    return move(f);
  }
  FPS operator-() && {
    for (T& x: *this) x = -x;
    return move(*this);
  }
  FPS operator-() const& {
    FPS f;
    f.reserve((*this).size());
    for (const T& x: *this) f.emplace_back(-x);
    return f;
  }

  static void multiply_naive(FPS& f, const FPS& g) {
    int n = int(f.size()), m = int(g.size());
    if (!n || !m) {
      f.clear();
    } else if (n < m) {
      FPS f_;
      swap(tmp, f_);
      if (f_.capacity() < f.capacity()) swap(f_, f);
      else f_ = f;
      f.assign(n + m - 1, T());
      for (int j = m - 1; j >= 0; j--) {
        for (int i = n - 1; i >= 0; i--) {
          f[i + j] += f_[i] * g[j];
        }
      }
      swap(tmp, f_);
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
  static void multiply_naive(FPS& f, FPS&& g) {
    int n = int(f.size()), m = int(g.size());
    if (n < m) swap(f, g);
    if (int(f.capacity()) >= n + m - 1 || int(g.capacity()) < n + m - 1) {
      multiply_naive(f, g);
    } else {
      multiply_naive(g, f);
      swap(f, g);
    }
  }

  static void multiply_fft(FPS& f_, const FPS& g_) {
    int n = int(f_.size()), m = int(g_.size());
    if (!n || !m) {
      f_.clear();
      return;
    }
    int z = 1 << internal::ceil_pow2(n + m - 1);
    FPS f, g;
    swap(f, f_);
    swap(g, tmp);
    g.clear();
    g.reserve(z);
    g = g_;

    f.resize(z);
    internal::butterfly(f);
    g.resize(z);
    internal::butterfly(g);
    for (int i = 0; i < z; i++) {
      f[i] *= g[i];
    }
    internal::butterfly_inv(f);
    f.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) f[i] *= iz;

    swap(g, tmp);
    swap(f, f_);
    return;
  }
  static void multiply_fft(FPS& f_, FPS&& g_) {
    int n = int(f_.size()), m = int(g_.size());
    if (!n || !m) {
      f_.clear();
      return;
    }
    int z = 1 << internal::ceil_pow2(n + m - 1);
    if (int(g_.capacity()) < z) {
      multiply_fft(f_, g_);
      return;
    }
    if (int(f_.capacity()) < z) {
      multiply_fft(g_, f_);
      swap(f_, g_);
      return;
    }
    FPS f, g = move(g_);
    swap(f, f_);

    f.resize(z);
    internal::butterfly(f);
    g.resize(z);
    internal::butterfly(g);
    for (int i = 0; i < z; i++) {
      f[i] *= g[i];
    }
    internal::butterfly_inv(f);
    f.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) f[i] *= iz;

    swap(f, f_);
    return;
  }

  FPS& operator*=(const FPS& f) {
    int n = int((*this).size()), m = int(f.size());
    if (min(n, m) <= 60) multiply_naive(*this, f);
    else multiply_fft(*this, f);
    return *this;
  }
  FPS& operator*=(FPS&& f) {
    int n = int((*this).size()), m = int(f.size());
    if (min(n, m) <= 60) multiply_naive(*this, move(f));
    else multiply_fft(*this, move(f));
    return *this;
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
  FPS operator-=(T x) {
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
    (*this).insert((*this).begin(), x, T());
    return *this;
  }
  friend FPS operator<<(FPS f, int x) {
    f <<= x;
    return f;
  }
  FPS& operator>>=(int x) {
    assert(x >= 0);
    int tsz = (*this).size();
    if (tsz <= x) (*this).clear();
    else (*this).erase((*this).begin(), (*this).begin() + x);
    return *this;
  }
  friend FPS operator>>(FPS f, int x) {
    f >>= x;
    return f;
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
