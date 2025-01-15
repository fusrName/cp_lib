vector<mint> fps_inv(vector<mint> f, int precision=-1) {
  if (precision == -1) precision = f.size();
  assert(precision >= 1 && f.size() >= 1 && f[0] != 0);
  auto f0 = f[0];
  if (f0 != 1) {
    f0 = f0.inv();
    for (auto& x : f) x *= f0;
  }
  int sz_tgt = bit_ceil(0U + precision);
  vector<mint> g(sz_tgt), g_ftt, fg;
  g_ftt.reserve(sz_tgt), fg.reserve(sz_tgt);
  g[0] = 1;
  static_assert(mint::mod() > 1 && (mint::mod() - 1) % 4 == 0);
  const mint i4 = mint::raw(mint::mod() - (mint::mod() - 1) / 4);
  mint iz = -1;
  for (int n = 1; n < sz_tgt; n *= 2) {
    fg.assign(f.begin(), f.begin() + min<int>(2 * n, f.size())), fg.resize(2 * n);
    g_ftt.assign(g.begin(), g.begin() + 2 * n);
    internal::butterfly(fg), internal::butterfly(g_ftt);
    for (int i = 0; i < 2 * n; i++) fg[i] *= g_ftt[i];
    internal::butterfly_inv(fg);
    for (int i = 0; i < n; i++) fg[i] = 0;
    internal::butterfly(fg);
    for (int i = 0; i < 2 * n; i++) fg[i] *= g_ftt[i];
    internal::butterfly_inv(fg);
    iz *= i4;
    for (int i = n; i < 2 * n; i++) g[i] = fg[i] * iz;
  }
  g.resize(precision);
  if (f0 != 1) for (auto& x : g) x *= f0;
  return g;
}

vector<mint> fps_log(vector<mint> f, int precision=-1) {
  if (precision == -1) precision = f.size();
  assert(precision >= 1 && f.size() >= 1 && f[0] == 1);
  auto f_inv = fps_inv(f, precision);
  for (int i = 0; i < ssize(f); i++) f[i] *= mint::raw(i);
  auto g = convolution(f, f_inv);
  g.resize(precision);
  for (int i = 1; i < precision; i++) g[i] *= iFact[i] * Fact[i-1];
  return g;
}

vector<mint> fps_exp(vector<mint> f, int precision=-1) {
  if (precision == -1) precision = f.size();
  assert(precision >= 1 && f.size() >= 1 && f[0] == 0);
  if (precision == 1 || f.size() == 1) {
    vector<mint> res(precision);
    res[0] = 1;
    return res;
  }
  if (precision == 2) return {1, f[1]};
  int sz_tgt = bit_ceil(0U + precision), k_tgt = countr_zero(0U + sz_tgt);
  vector<mint> g(sz_tgt), g_inv(sz_tgt / 2), g_dft, g0_dft, g_inv_dft, tmp;
  {
    int cap = sz_tgt / 2;
    g_dft.reserve(cap), g0_dft.reserve(cap), g_inv_dft.reserve(cap), tmp.reserve(cap);
  }
  g[0] = 1, g[1] = f[1];
  g_inv[0] = 1;
  g_inv_dft = {1, 1};
  if (k_tgt % 2 == 0) g0_dft = {1, 1};
  const mint inv4 = mint(mint::mod() - ((mint::mod() - 1) >> 2));
  const mint inv2 = mint(mint::mod() - ((mint::mod() - 1) >> 1));
  mint inv_4k = -inv4, inv_m = inv2, inv_2m = inv4;
  for (int m = 2, k = 1; m < sz_tgt; m *= 2, k++, inv_m = inv_2m, inv_2m *= inv2, inv_4k *= inv4) {
    // extend from g_inv[0,m/2) to g_inv[0,m)
    // tmp = error of g*g_inv from 1
    g_dft.assign(g.begin(), g.begin() + m);
    if ((k_tgt - k) % 2 == 0) g_dft.resize(2 * m);
    internal::butterfly(g_dft);
    tmp.resize(m);
    for (int i = 0; i < m; i++) tmp[i] = g_dft[i] * g_inv_dft[i];
    internal::butterfly_inv(tmp);
    for (int i = 0; i < m / 2; i++) tmp[i] = 0;
    // tmp = correction term of g_inv
    internal::butterfly(tmp);
    for (int i = 0; i < m; i++) tmp[i] *= g_inv_dft[i];
    internal::butterfly_inv(tmp);
    for (int i = m / 2; i < m; i++) g_inv[i] = tmp[i] * inv_4k;
    // log(g) = \int (f'[0,m) + (g'-f'[0,m)g)/g) in [m,2m)
    // tmp = g'-f'[0,m)g in [m,2m)
    {
      int i_mid = min<int>(m, ssize(f));
      for (int i = 0; i < i_mid; i++) tmp[i] = f[i] * mint::raw(i);
      for (int i = i_mid; i < m; i++) tmp[i] = 0;
    }
    internal::butterfly(tmp);
    for (int i = 0; i < m; i++) tmp[i] *= g_dft[i];
    internal::butterfly_inv(tmp);
    for (int i = 0; i < m; i++) tmp[i] = g[i] * mint::raw(i) - tmp[i] * inv_m;
    // tmp = g_inv*(g'-f'[0,m)g) in [m,2m)
    if (2 * m < sz_tgt) {
      tmp.resize(2 * m);
      internal::butterfly(tmp);
      g_inv_dft.assign(g_inv.begin(), g_inv.begin() + m);
      g_inv_dft.resize(2 * m);
      internal::butterfly(g_inv_dft);
      for (int i = 0; i < 2 * m; i++) tmp[i] *= g_inv_dft[i];
      internal::butterfly_inv(tmp);
      for (int i = 0; i < m; i++) tmp[i] *= inv_2m;
      tmp.resize(m);
    } else {
      for (int i = 0; i < m / 2; i++) g_inv[i] = tmp[i], tmp[i] = 0;
      internal::butterfly(tmp);
      for (int i = 0; i < m; i++) g[m+i] = g_inv_dft[i] * tmp[i];
      for (int i = 0; i < m / 2; i++) tmp[i] = g_inv[i], g_inv[i] = 0;
      for (int i = m / 2; i < m; i++) tmp[i] = 0;
      internal::butterfly(g_inv);
      internal::butterfly(tmp);
      for (int i = 0; i < m; i++) {
        g_inv[i] = g_inv[i] * tmp[i] + g[m+i];
        tmp[i] *= g_inv_dft[i];
      }
      internal::butterfly_inv(g_inv);
      internal::butterfly_inv(tmp);
      for (int i = 0; i < m / 2; i++) tmp[i] *= inv_m;
      for (int i = m / 2; i < m; i++) tmp[i] = (tmp[i] + g_inv[i]) * inv_m;
    }
    // tmp = g(f - log(g)) in [m,2m)
    int i_mid = clamp<int>(ssize(f), m, 2 * m);
    for (int i = m; i < i_mid; i++) tmp[i - m] = f[i] - tmp[i - m] * iFact[i] * Fact[i-1];
    for (int i = i_mid; i < 2 * m; i++) tmp[i - m] = -tmp[i - m] * iFact[i] * Fact[i-1];
    if ((k_tgt - k) % 2 == 0) {
      tmp.resize(2 * m);
      internal::butterfly(tmp);
      for (int i = 0; i < 2 * m; i++) tmp[i] *= g_dft[i];
      internal::butterfly_inv(tmp);
      for (int i = 0; i < m; i++) g[i + m] = tmp[i] * inv_2m;
      swap(g_dft, g0_dft);
    } else {
      for (int i = 0; i < m; i++) g_dft[i] -= g0_dft[i];
      for (int i = 0; i < m / 2; i++) g[m+i] = tmp[i], tmp[i] = 0;
      internal::butterfly(tmp);
      for (int i = 0; i < m / 2; i++) tmp[i] = exchange(g[m+i], tmp[i] * g0_dft[i]);
      for (int i = m / 2; i < m; i++) g[m+i] = tmp[i] * g0_dft[i], tmp[i] = 0;
      internal::butterfly(tmp);
      for (int i = 0; i < m; i++) g_dft[i] = g_dft[i] * tmp[i] + g[m+i], g0_dft[i] *= tmp[i];
      internal::butterfly_inv(g0_dft);
      internal::butterfly_inv(g_dft);
      for (int i = 0; i < m / 2; i++) g[m+i] = g0_dft[i] * inv_m;
      for (int i = m / 2; i < m; i++) g[m+i] = (g0_dft[i] + g_dft[i]) * inv_m;
    }
  }
  g.resize(precision);
  return g;
}

vector<mint> fps_inv_naive(vector<mint> f, int precision=-1) {
  if (precision == -1) precision = f.size();
  assert(precision >= 1 && f.size() >= 1 && f[0] != 0);
  auto f0 = f[0];
  if (f0 != 1) {
    f0 = f0.inv();
    for (auto& x : f) x *= f0;
  }
  vector<mint> g(precision);
  g[0] = 1;
  for (int i = 0; i < precision; i++) {
    for (int j = 1, j_end = min<int>(ssize(f), precision - i); j < j_end; j++) {
      g[i+j] -= g[i] * f[j];
    }
  }
  if (f0 != 1) for (auto& x : g) x *= f0;
  return g;
}

vector<mint> fps_log_naive(const vector<mint>& f, int precision=-1) {
  if (precision == -1) precision = f.size();
  assert(precision >= 1 && f.size() >= 1 && f[0] == 1);
  vector<mint> g(precision);
  for (int i = 1; i < precision; i++) {
    if (i < ssize(f)) g[i] += mint::raw(i) * f[i];
    for (int j = 1, j_end = min<int>(ssize(f), precision - i); j < j_end; j++) {
      g[i+j] -= g[i] * f[j];
    }
    g[i] *= iFact[i] * Fact[i-1];
  }
  return g;
}

vector<mint> fps_exp_naive(vector<mint> f, int precision=-1) {
  if (precision == -1) precision = f.size();
  assert(precision >= 1 && f.size() >= 1 && f[0] == 0);
  // exp(f)=1+\int exp(f)f'dx
  for (int i = 1, i_end = min<int>(ssize(f), precision); i < i_end; i++) {
    f[i] *= mint::raw(i);
  }
  vector<mint> g(precision);
  g[0] = 1;
  for (int i = 0; i < precision; i++) {
    if (i) g[i] *= iFact[i] * Fact[i-1];
    for (int j = 1, j_end = min<int>(ssize(f), precision - i); j < j_end; j++) {
      g[i+j] += g[i] * f[j];
    }
  }
  return g;
}

vector<mint> fps_exp_by_semi_relaxed_convolution(vector<mint> f, int precision=-1) {
  if (precision == -1) precision = f.size();
  assert(precision >= 1 && f.size() >= 1 && f[0] == 0);
  // exp(f)=1+\int exp(f)f'dx
  for (int i = 1, i_end = min<int>(ssize(f), precision); i < i_end; i++) {
    f[i] *= mint::raw(i);
  }
  semi_relaxed_convolution src(f);
  src.set(1);
  for (int i = 1; i < precision; i++) {
    src.proceed();
    src.set(src.get() * iFact[i] * Fact[i-1]);
  }
  src.d.resize(precision);
  return move(src.d);
}
