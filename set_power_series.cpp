template <int sz>
vector<array<mint, sz>> ranked_zeta(const vector<mint>& f) {
  int N = f.size();
  vector<array<mint, sz>> fr(N);
  for (int i = 0; i < N; i++) fr[i][__builtin_popcount(i)] = f[i];
  for (int k = 1; k < N; k <<= 1) {
    for (int i = k; i < N; i = (i + 1) | k) {
      for (int p = 0; p < sz; p++) {
        fr[i][p] += fr[i ^ k][p];
      }
    }
  }
  return fr;
}

template <int sz>
vector<mint> ranked_mobius(vector<array<mint, sz>> fr) {
  int N = fr.size();
  for (int k = 1; k < N; k <<= 1) {
    for (int i = k; i < N; i = (i + 1) | k) {
      for (int p = 0; p < sz; p++) {
        fr[i][p] -= fr[i ^ k][p];
      }
    }
  }
  vector<mint> f(N);
  for (int i = 0; i < N; i++) f[i] = fr[i][__builtin_popcount(i)];
  return f;
}

template <int sz>
vector<mint> subset_convolution(const vector<mint>& f, const vector<mint>& g) {
  assert(size(f) == size(g));
  const int N = size(f), n = __builtin_ctz(size(f));
  assert(N == 1 << n);
  assert(sz >= n);
  auto fr = ranked_zeta<sz + 1>(f);
  auto gr = ranked_zeta<sz + 1>(g);
  for (int i = 0; i < N; i++) {
    for (int p = sz; p >= 0; p--) {
      for (int q = sz - p; q > 0; q--) {
        fr[i][p + q] += fr[i][p] * gr[i][q];
      }
      fr[i][p] *= gr[i][0];
    }
  }
  return ranked_mobius<sz + 1>(move(fr));
}

template <int sz>
vector<mint> sps_log(const vector<mint>& f) {
  const int N = size(f), n = __builtin_ctz(size(f));
  assert(N == 1 << n);
  assert(sz >= n);
  auto fr = ranked_zeta<sz + 1>(f);
  mint inv[sz + 1];
  inv[1] = 1;
  for (int i = 2; i <= sz; i++) {
    inv[i] = -(mint::mod() / i) * inv[mint::mod() % i];
  }
  for (int i = 0; i < N; i++) {
    auto a = fr[i];
    for (int p = 2; p <= sz; p++) fr[i][p] *= mint::raw(p);
    for (int p = 1; p < sz; p++) {
      for (int q = sz - p; q > 0; q--) {
        fr[i][p + q] -= fr[i][p] * a[q];
      }
    }
    for (int p = 2; p <= sz; p++) fr[i][p] *= inv[p];
    fr[i][0] = 0;
  }
  return ranked_mobius<sz + 1>(fr);
}
