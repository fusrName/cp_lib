struct relaxed_convolution {
  vector<mint> f{0}, g{0}, fg{0}, tmp1, tmp2;
  vector<vector<mint>> f_fft, g_fft;
  int cur = 0;
  void add(mint fi, mint gi) {
    f[cur] += fi, g[cur] += gi;
    fg[cur] += cur ? fi * g[0] + f[0] * gi : fi * gi;
  }
  mint get() { return fg[cur]; }
  void proceed(mint fi, mint gi) {
    add(fi, gi);
    proceed();
  }
  void proceed() {
    cur++;
    if ((cur & (cur - 1)) == 0) {
      int n2 = 2 * cur;
      f.resize(n2), g.resize(n2), fg.resize(n2);
      if (cur == 1) return;
      f_fft.emplace_back(), g_fft.emplace_back();
      swap(f_fft.back(), tmp1), swap(g_fft.back(), tmp2);
      tmp1.resize(n2), tmp2.resize(n2);
      for (int i = 0; i < cur; i++) tmp1[i] = f[i], tmp2[i] = g[i];
      internal::butterfly(tmp1), internal::butterfly(tmp2);
      f_fft.back().assign(tmp1.begin(), tmp1.begin() + cur);
      g_fft.back().assign(tmp2.begin(), tmp2.begin() + cur);
      mint iz = mint::raw(mint::mod() - (mint::mod() - 1) / cur);
      for (auto& x : f_fft.back()) x *= iz;
      for (auto& x : g_fft.back()) x *= iz;
      for (int i = 0; i < n2; i++) tmp1[i] *= tmp2[i];
      internal::butterfly_inv(tmp1);
      iz = mint::raw(mint::mod() - (mint::mod() - 1) / n2);
      for (int i = cur; i < n2; i++) fg[i] = tmp1[i] * iz;
    } else {
      int n = cur & -cur, n2 = 2 * n, k = countr_zero(0U + n), l = cur - n;
      {
        tmp1.assign(f.begin() + l, f.begin() + cur), tmp1.resize(n2);
        internal::butterfly(tmp1);
        for (auto it = tmp1.begin(); auto x : g_fft[k]) *it++ *= x;
        internal::butterfly_inv(tmp1);
        for (int i = n; i < n2; i++) fg[l + i] += tmp1[i];
      }
      {
        tmp1.assign(g.begin() + l, g.begin() + cur), tmp1.resize(n2);
        internal::butterfly(tmp1);
        for (auto it = tmp1.begin(); auto x : f_fft[k]) *it++ *= x;
        internal::butterfly_inv(tmp1);
        for (int i = n; i < n2; i++) fg[l + i] += tmp1[i];
      }
    }
  }
};

struct semi_relaxed_convolution {
  const vector<mint>& f;
  vector<mint> d{0}, tmp;
  vector<vector<mint>> f_fft;
  int cur = 0;
  semi_relaxed_convolution(const vector<mint>& f) : f(f) {}
  mint get() { return d[cur]; }
  void set(mint gi) { d[cur] = gi; }
  void proceed(mint gi) {
    set(gi);
    proceed();
  }
  void proceed() {
    cur++;
    int n = cur & -cur, n2 = 2 * n, k = countr_zero(0U + n), l = cur - n;
    if ((cur & (cur - 1)) == 0) {
      d.resize(n2);
      vector<mint> ff; ff.reserve(n2);
      ff.assign(f.begin(), f.begin() + min<int>(ssize(f), n2)); ff.resize(n2);
      internal::butterfly(ff);
      mint iz = mint::raw(mint::mod() - (mint::mod() - 1) / n2);
      for (auto& x : ff) x *= iz;
      f_fft.emplace_back(move(ff));
    }
    tmp.assign(d.begin() + l, d.begin() + cur), tmp.resize(n2);
    internal::butterfly(tmp);
    for (auto it = tmp.begin(); auto x : f_fft[k]) *it++ *= x;
    internal::butterfly_inv(tmp);
    for (int i = n; i < n2; i++) d[l + i] += tmp[i];
  }
};
