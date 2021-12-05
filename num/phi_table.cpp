vector<int> build_phi_table(const int n) {
  assert(n < (int)lpf.size());
  vector<int> res(n + 1);
  res[1] = 1;
  for(int p: primes) {
    if (p > n) break;
    res[p] = p - 1;
  }
  for(int x = 2; x <= n; x++) {
    int px = lpf[x];
    for(int q: primes) {
      int y = q * x;
      if (y > n) break;
      if (q != px) {
        res[y] = res[x] * (q - 1);
      } else {
        res[y] = res[x] * q;
        break;
      }
    }
  }
  return res;
}

auto phi = build_phi_table(1000000);