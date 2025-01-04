struct relaxed_convolution {
  vector<mint> f{0}, g{0}, fg{0}, tmp;
  vector<vector<mint>> f_ftt, g_ftt;
  int cur = 0;
  void proceed() {
    cur++;
    if ((cur & (cur - 1)) == 0) {
      int n2 = 2 * cur;
      f.resize(n2), g.resize(n2), fg.resize(n2);
      tmp.resize(n2);
      vector<mint> gg(n2);
      for (int i = 0; i < cur; i++) tmp[i] = f[i], gg[i] = g[i];
      internal::butterfly(tmp), internal::butterfly(gg);
      for (int i = 0; i < n2; i++) tmp[i] *= gg[i];
      internal::butterfly_inv(tmp);
      mint iz = mint::raw(mint::mod() - (mint::mod() - 1) / n2);
      for (int i = cur; i < n2; i++) fg[i] = tmp[i] * iz;
      g_ftt.emplace_back(move(gg));
    } else {
      int n = cur & -cur, n2 = 2 * n, k = countr_zero(0U + n), l = cur - n;
      if (ssize(f_ftt) == k) {
        vector<mint> ff(f.begin(), f.begin() + n2), gg = move(g_ftt[k]);
        gg.assign(g.begin(), g.begin() + n2);
        internal::butterfly(ff), internal::butterfly(gg);
        mint iz = mint::raw(mint::mod() - (mint::mod() - 1) / n2);
        for (auto& x : ff) x *= iz;
        for (auto& x : gg) x *= iz;
        f_ftt.emplace_back(move(ff)), g_ftt[k] = move(gg);
      }
      {
        tmp.assign(f.begin() + l, f.begin() + cur), tmp.resize(n2);
        internal::butterfly(tmp);
        for (auto it = tmp.begin(); auto x : g_ftt[k]) *it++ *= x;
        internal::butterfly_inv(tmp);
        for (int i = n; i < n2; i++) fg[l + i] += tmp[i];
      }
      {
        tmp.assign(g.begin() + l, g.begin() + cur), tmp.resize(n2);
        internal::butterfly(tmp);
        for (auto it = tmp.begin(); auto x : f_ftt[k]) *it++ *= x;
        internal::butterfly_inv(tmp);
        for (int i = n; i < n2; i++) fg[l + i] += tmp[i];
      }
    }
  }
};
