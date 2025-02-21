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

template<class T>
T phi(T x) {
  T res = x;
  for(T p = 2; p * p <= x; p++) {
    if (x % p == 0) {
      do { x /= p; } while (x % p == 0);
      res = res / p * (p - 1);
    }
  }
  if (x > 1) res = res / x * (x - 1);
  return res;
}

template<class T>
int d(T x) {
  T res = 1;
  for(T p = 2; p * p <= x; p++) {
    int cnt = 1;
    while (x % p == 0) x /= p, cnt++;
    res *= cnt;
  }
  if (x > 1) res *= 2;
  return res;
}

long long reduced_totient(long long a) {
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

ll calc_ord(ll x, ll m) {
  assert(m >= 1 && gcd(x, m) == 1);
  if (m <= 2) return 1;
  auto fs = factorize(reduced_totient(m));
  int n = fs.size();
  vector<ll> dp(n), pe(n);
  for (int i = 0; auto [p, e] : fs) {
    ll v = 1;
    while (e--) v *= p;
    pe[i++] = v;
  }
  dp[0] = x;
  int k = bit_ceil(0U + n);
  x %= m;
  if (x < 0) x += m;
  while (k != 1) {
    int k2 = k / 2;
    for (int l = 0; l + k2 < n; l += k) {
      int c = l + k2, r = min(n, l + k);
      ll v = 1;
      for (int i = l; i < c; i++) v *= pe[i];
      dp[c] = pow_mod(dp[l], v, m);
      v = 1;
      for (int i = c; i < r; i++) v *= pe[i];
      dp[l] = pow_mod(dp[l], v, m);
    }
    k = k2;
  }
  ll res = 1;
  for (int i = 0; i < n; i++) {
    ll xi = dp[i];
    int p = fs[i].first;
    while (xi != 1) xi = pow_mod(xi, p, m), res *= p;
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


// returns (g, c) pairs, where c = #{i in [n]| gcd(i, n) = g}
template<class T>
vector<pair<T, T>> gcd_distribution(T n) {
  auto fs = factorize(n);
  vector<pair<T, T>> g2c;
  auto dfs = [&](auto&& self, int i, T g, T now) -> void {
    if (i == (int)fs.size()) {
      g2c.emplace_back(g, now / g);
      return;
    }
    auto [p, e] = fs[i];
    T nxt = now / p * (p - 1);
    for(int k = 0; k < e; k++, g *= p) {
      self(self, i + 1, g, nxt);
    }
    self(self, i + 1, g, now);
  };
  dfs(dfs, 0, 1, n);
  return g2c;
}

auto floor_div(signed_integral auto x, signed_integral auto y) {
  return x / y - ((x ^ y) < 0 && x % y != 0);
}
template <integral T>
T floor_div(T x, unsigned_integral auto y) {
  return x >= 0 ? T(x / y) : -T(-x / y + (-x % y != 0));
}
auto ceil_div(signed_integral auto x, signed_integral auto y) {
  return x / y + ((x ^ y) >= 0 && x % y != 0);
}
template <integral T>
T ceil_div(T x, unsigned_integral auto y) {
  return x >= 0 ? T(x / y + (x % y != 0)) : -T(-x / y);
}

// eps=2^-M  (M=23,52,63)
// x<z^2
// x=z^2-c
// 2^i<z<=2^(i+1)
// e=2^i*eps=2^-(M-i)
// sqrt(z^2-c) < z-e/2
// if z^2-c<z^2-ez+e^2/4
// if ez<=1
// if i+1-(M-i)<=0
// if i<=(M-1)/2
// if x<z^2<=2^2(floor((M-1)/2)+1)=2^2ceil(M/2)
template <integral T>
T isqrt(T x) {
  assert (x >= 0);
  if (x < 1 << 24) return sqrt((float)x);
  else if (x < 1LL << 52) return sqrt(x);
  else return sqrtl(x);
}
