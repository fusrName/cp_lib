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
  // f=log(g); f=log(g0)(g-g0)^0+(1/g0)(g-g0)^1+O((g-g0)^2)
  // g=g0+g0(f-log(g0))
  if (precision == 1) return {1};
  int sz_tgt = bit_ceil(0U + precision);
  vector<mint> g(sz_tgt), g_inv(sz_tgt / 2), g_ftt, gi_ftt, r, g0_inv;
  g_ftt.reserve(sz_tgt), gi_ftt.reserve(sz_tgt), r.reserve(sz_tgt), g0_inv.reserve(sz_tgt);
  g[0] = g_inv[0] = 1;
  static_assert(mint::mod() > 1 && (mint::mod() - 1) % 2 == 0);
  const mint i2 = mint::raw(mint::mod() - (mint::mod() - 1) / 2);
  mint i2k = i2;
  for (int n = 1; n < sz_tgt; n *= 2, i2k *= i2) {
    g_ftt.assign(g.begin(), g.begin() + 2 * n);
    gi_ftt.assign(g_inv.begin(), g_inv.begin() + n), gi_ftt.resize(2 * n);
    internal::butterfly(g_ftt), internal::butterfly(gi_ftt);
    {  // g0_inv
      r.resize(2 * n);
      for (int i = 0; i < 2 * n; i++) r[i] = g_ftt[i] * gi_ftt[i];
      internal::butterfly_inv(r);
      for (int i = 0; i < n; i++) r[i] = 0;
      internal::butterfly(r);
      for (int i = 0; i < 2 * n; i++) r[i] *= gi_ftt[i];
      internal::butterfly_inv(r);
      mint iz = -i2k * i2k;
      g0_inv.resize(2 * n);
      for (int i = 0; i < n; i++) g0_inv[i] = g_inv[i];
      for (int i = n; i < 2 * n; i++) g0_inv[i] = r[i] * iz;
    }
    {  // r=log(g0)
      for (int i = 0; i < n; i++) r[i] = mint::raw(i) * g[i];
      for (int i = n; i < 2 * n; i++) r[i] = 0;
      internal::butterfly(r), internal::butterfly(g0_inv);
      for (int i = 0; i < 2 * n; i++) r[i] *= g0_inv[i];
      internal::butterfly_inv(r);
      for (int i = 0; i < n; i++) r[i] = 0;
      for (int i = n; i < 2 * n; i++) r[i] *= i2k * iFact[i] * Fact[i-1];
    }
    {  // g0(f-log(g0))
      for (int i = n, i_end = min<int>(ssize(f), 2 * n); i < i_end; i++) r[i] = f[i] - r[i];
      internal::butterfly(r);
      for (int i = 0; i < 2 * n; i++) r[i] *= g_ftt[i];
      internal::butterfly_inv(r);
      for (int i = n; i < 2 * n; i++) g[i] = r[i] * i2k;
    }
    if (2 * n < sz_tgt) {  // g_inv
      g_ftt.assign(g.begin(), g.begin() + 2 * n);
      internal::butterfly(g_ftt);
      for (int i = 0; i < 2 * n; i++) r[i] = g_ftt[i] * gi_ftt[i];
      internal::butterfly_inv(r);
      for (int i = 0; i < n; i++) r[i] = 0;
      internal::butterfly(r);
      for (int i = 0; i < 2 * n; i++) r[i] *= gi_ftt[i];
      internal::butterfly_inv(r);
      mint iz = -i2k * i2k;
      for (int i = n; i < 2 * n; i++) g_inv[i] = r[i] * iz;
    }
  }
  g.resize(precision);
  return g;
}
