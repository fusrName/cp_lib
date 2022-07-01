template<class T>
vector<T> divisors(T x) {
  vector<T> res1, res2;
  T d = 1;
  for(; d * d < x; d++) {
    if (x % d == 0) {
      res1.emplace_back(d);
      res2.emplace_back(x / d);
    }
  }
  if (d * d == x) res1.emplace_back(d);
  res1.insert(res1.end(), res2.rbegin(), res2.rend());
  return res1;
}

template<class T>
vector<pair<T, int>> factorize(T x) {
  vector<pair<T, int>> res;
  for(T p = 2; p * p <= x; p++) {
    int cnt = 0;
    while(x % p == 0) x /= p, cnt++;
    if (cnt) res.emplace_back(p, cnt);
  }
  if (x > 1) res.emplace_back(x, 1);
  return res;
}

long long reduced_totient(long long a) {
  auto lcm = [](long long x, long long y) { return x / gcd(x, y) * y; };
  long long res = 1;
  for(auto [p, cnt]: factorize(a)) {
    if (p == 2) {
      res = cnt <= 2 ? 1LL << (cnt - 1) : 1LL << (cnt - 2);
      continue;
    }
    long long phi = p - 1;
    for(int i = cnt - 1; i > 0; i--) phi *= p;
    res = lcm(res, phi);
  }
  return res;
}

int order_of(long long a, long long m) {
  assert(m > 0 && a >= 0);
  if (m == 1) return 1;
  if (gcd(a, m) != 1) return -1;
  long long ord = reduced_totient(m);
  for(long long d: divisors(ord)) {
    if (pow_mod(a, d, m) == 1) return d;
  }
  assert(false);
}

template<class T>
T isqrt(T x) {
  assert(x >= 0);
  if (x <= 1) return x;
  T r = x >> 1;
  while(true) {
    T nr = (r + x / r) >> 1;
    if (r <= nr) break;
    r = nr;
  }
  return r;
}
