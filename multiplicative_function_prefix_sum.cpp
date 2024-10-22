template <integral T>
T isqrt(T x) {
  assert (x >= 0);
  if (x < (1 << 24)) return sqrt((float)x);
  else if (x < (1LL << 53)) return sqrt(x);
  else return sqrtl(x);
}

template <class T>
vector<T> list_quotients(T n) {
  vector<T> q;
  int sq = isqrt(n);
  // floor(n/sq)==sq
  // iff floor((n-sq^2)/sq)==0
  // iff n-sq^2<sq
  bool dup = n < T(sq + 1) * sq;
  q.reserve(2 * sq - dup);
  for (int i = 1; i <= sq; i++) q.emplace_back(n / i);
  for (int i = sq - dup; i >= 1; i--) q.emplace_back(i);
  return q;
}

vector<int> list_primes(int n) {
  assert(n >= 0);
  if (n == 0) return {};
  vector<char> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int p = 2; p * p <= n; p++) {
    if (is_prime[p]) {
      for (int x = p * p; x <= n; x += p) {
        is_prime[x] = false;
      }
    }
  }
  vector<int> ps;
  ps.reserve(count(is_prime.begin(), is_prime.end(), true));
  for (int p = 2; p <= n; p++) {
    if (is_prime[p]) ps.emplace_back(p);
  }
  return ps;
}

struct QP {
  long long n;
  int sq;
  vector<long long> q;
  vector<int> primes;
};

template <class S, auto f_acc, auto add, auto add_inv, auto mul_map>
pair<vector<S>, QP> Lucy(long long n) {
  assert (n >= 1);
  int sq = isqrt(n);
  vector<long long> q = list_quotients(n);
  vector<int> primes = list_primes(sq);
  int qsz = q.size();
  vector<S> dp;
  dp.reserve(qsz);
  for (auto f1i = add_inv(f_acc(1)); long long x : q) dp.emplace_back(add(f_acc(x), f1i));
  const int n_sq1 = n / (sq + 1);
  for (int p : primes) {
    auto fpacc_inv = add_inv(dp[qsz - p + 1]);
    // floor(n/d/p) ... (inf, sq),[sq, p]
    // floor(n/dp)>sq iff d<=n/(sq+1)p
    // floor(n/(sq+1)p)<=floor(n/(sq+1))<=sq
    int i1 = n_sq1 / p;
    // floor(n/dp)>=p iff d<=n/p^2
    auto p2 = 1LL * p * p;
    int i2 = p2 <= sq ? qsz - p2 + 1 : n / p2;
    // for (int i = 0; i < i1; i++) assert(q[i] / p > sq);
    // for (int i = i1; i < i2; i++) assert(sq >= q[i] / p && q[i] / p >= p);
    // for (int i = i2; i < qsz; i++) assert(q[i] / p < p);
    for (int i = 0; i < i1; i++) {
      int j = int(n / (q[i] / p)) - 1;
      dp[i] = add(dp[i], add_inv(mul_map(add(dp[j], fpacc_inv), p)));
    }
    for (int i = i1; i < i2; i++) {
      int j = qsz - int(q[i] / p);
      dp[i] = add(dp[i], add_inv(mul_map(add(dp[j], fpacc_inv), p)));
    }
  }
  return pair{move(dp), QP{.n=n, .sq=sq, .q=move(q), .primes=move(primes)}};
}

template <class S, auto f, auto add, /*auto zero,*/ auto add_inv, auto mul, auto one>
S min_25_sieve(vector<S> dp, QP qp) {
  auto [n, sq, q, primes] = move(qp);
  assert (n >= 1);
  int qsz = q.size();
  // if (!qsz) return zero();
  for (int p : primes | views::reverse) {
    auto smi = add_inv(dp[qsz - p]);
    auto p2 = 1LL * p * p;
    for (int i = 0;; i++) {
      auto v = q[i];
      if (v < p2) break;
      int e = 0;
      do {
        v /= p; e++;
        int j = v <= sq ? qsz - v : n / v - 1;
        dp[i] = add(add(dp[i], mul(f(p, e), add(dp[j], smi))), f(p, e + 1));
      } while (v >= p2);
    }
  }
  return add(dp[0], one());
}







// Example:
struct S {
  ll c1, c2;
};
S f_acc(ll x) {
  return {(x + 2) / 3, (x + 1) / 3};
}
S add_s(S x, S y) { return {x.c1 + y.c1, x.c2 + y.c2}; }
S add_s_inv(S x) { return {-x.c1, -x.c2}; }
S mul_map(S x, int p) {
  if (p == 3) return {};
  if (p % 3 == 2) swap(x.c1, x.c2);
  return x;
}
struct T {
  mint c0, c12;
};
int m;
T f(int p, int e) {
  int r = p % 3;
  mint v = comb(e + m - 1, e);
  if ((r == 1 && e % 3 == 2) || (r == 2 && e % 2 == 1)) return {v, 0};
  return {0, v};
}
T add_t(T x, T y) { return {x.c0 + y.c0, x.c12 + y.c12}; }
// T zero_t() { return {}; }
T add_t_inv(T x) { return {-x.c0, -x.c12}; }
T mul(T x, T y) {
  mint v = x.c12 * y.c12;
  return {(x.c0 + x.c12) * (y.c0 + y.c12) - v, v};
}
T one() { return {0, 1}; }
