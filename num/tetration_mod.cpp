pair<long long, vector<pair<long long, long long>>> split_power_mods(
    long long x, long long m, long long max_count = -1) {
  vector<pair<long long, long long>> res;
  vector<pair<long long, int>> factors;
  auto factorize = [&](long long x) {
    for (int p = 2; (long long)p * p <= x; p++) {
      if (x % p == 0) {
        int c = 0;
        while (x % p == 0) x /= p, c++;
        factors.emplace_back(p, c);
      }
    }
    if (x != 1) factors.emplace_back(x, 1);
  };
  auto powll = [](long long x, int k) {
    long long xk = k & 1 ? x : 1;
    while (k >>= 1) {
      x *= x;
      if (k & 1) xk *= x;
    }
    return xk;
  };
  factorize(m);
  while (!factors.empty() && max_count--) {
    long long m1 = 1;
    long long m2 = 1;
    for (int i = 0, sz = factors.size(); i < sz; i++) {
      auto [p, c] = factors[i];
      if (x % p == 0) {
        swap(factors[i], factors.back());
        factors.pop_back();
        i--, sz--;
        m2 *= powll(p, c);
      } else {
        m1 *= powll(p, c);
      }
    }
    res.emplace_back(m1, m2);
    for (int i = 0, sz = factors.size(); i < sz; i++) {
      auto& [p, c] = factors[i];
      if (p == 2 && c >= 3) c--;
      c--;
      factorize(p - 1);
    }
    factors.erase(
        remove_if(factors.begin(), factors.end(),
                  [](const pair<long long, int> x) { return x.second == 0; }),
        factors.end());
    sort(factors.begin(), factors.end());
    for (int i = (int)factors.size() - 1; i > 0; i--) {
      if (factors[i - 1].first == factors[i].first) {
        factors[i - 1].second = factors[i].second;
      }
    }
    factors.erase(unique(factors.begin(), factors.end()), factors.end());
  }
  long long m1 = 1;
  for (auto [p, c] : factors) m1 *= powll(p, c);
  return {m1, move(res)};
}

long long tetration_mod(long long x, long long n, long long m) {
  assert(x >= 1 && n >= 0 && m >= 1);
  // if (x == 0) return (1 - n % 2) % m;
  if (x == 1) return 1 % m;
  auto [m0, mods] = split_power_mods(x, m, n);
  n -= (int)mods.size();
  long long res = n ? 0 : 1 % m0;
  long long xtet = 1;
  bool large = false;
  while (n--) {
    long long nv = 1;
    while (!large && xtet--) {
      large = __builtin_mul_overflow(nv, x, &nv) || nv >= m;
    }
    if (large) break;
    xtet = nv;
  }
  auto pow_modll = [](long long x, long long k, long long m) {
    long long xk = (k & 1 ? x : 1) % m;
    while (k >>= 1) {
      x = __int128(x) * x % m;
      if (k & 1) xk = __int128(xk) * x % m;
    }
    return xk;
  };
  for (int i = (int)mods.size() - 1; i >= 0; i--) {
    auto [m1, m2] = mods[i];
    if (large) {
      long long r1 = pow_modll(x, res, m1);
      res = crt({r1, 0}, {m1, m2}).first;
    } else {
      res = pow_modll(x, xtet, m1 * m2);
      if (i) {
        long long nv = 1;
        while (!large && xtet--) {
          large = __builtin_mul_overflow(nv, x, &nv) || nv >= m;
        }
        xtet = nv;
      }
    }
  }
  return res;
}
