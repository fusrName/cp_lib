vector<signed char> build_mu_table(const int n) {
  assert(n < (int)lpf.size());
  vector<signed char> res(n + 1);
  res[1] = 1;
  for(int p: primes) {
    if (p > n) break;
    res[p] = -1;
  }
  for(int x = 2; x <= n; x++) {
    if (!res[x]) continue;
    int px = lpf[x];
    for(int q: primes) {
      if (q == px) break;
      int y = q * x;
      if (y > n) break;
      res[y] = -res[x];
    }
  }
  return res;
}

auto mu = build_mu_table(1000000);