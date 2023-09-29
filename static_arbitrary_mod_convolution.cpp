template <class mint, internal::is_static_modint_t<mint>* = nullptr>
std::vector<mint> static_arbitrary_mod_convolution(const std::vector<mint>& a,
                                                   const std::vector<mint>& b) {
  constexpr unsigned int mod = mint::mod();
  int n = int(a.size()), m = int(b.size());
  if (!n || !m) return {};

  constexpr unsigned int MOD1 = 167772161;   // 2^25
  constexpr unsigned int MOD2 = 469762049;   // 2^26
  constexpr unsigned int MOD3 = 1107296257;  // 2^25
  static constexpr int MAX_AB_BIT = 25;
  assert(n + m - 1 <= (1 << MAX_AB_BIT));

  auto conv = [&](auto tp) {
    vector<static_modint<tp.value>> a2(n), b2(m);
    for (int i = 0; i < n; i++) a2[i] = a[i].val();
    for (int i = 0; i < m; i++) b2[i] = b[i].val();
    return convolution(a2, b2);
  };
  auto c1 = conv(integral_constant<int, MOD1>{});
  auto c2 = conv(integral_constant<int, MOD2>{});
  auto c3 = conv(integral_constant<int, MOD3>{});

  vector<static_modint<mod>> c(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    // t1 = c1
    unsigned int t1 = c1[i].val();

    // t1 + m1t2 = c2
    // t2 = (c2-t1) / m1
    constexpr unsigned long long i1_2 = internal::inv_gcd(MOD1, MOD2).second;
    unsigned int t2 = (c2[i].val() - t1 + MOD2) * i1_2 % MOD2;

    // t1 + m1t2 + m1m2t3 = c3
    // t3 = (c3-t1)/m1m2 - t2/m2
    constexpr unsigned long long i1i2_3 = internal::inv_gcd(1ULL * MOD1 * MOD2, MOD3).second;
    constexpr unsigned long long i2_3 = internal::inv_gcd(MOD2, MOD3).second;
    unsigned int t3 = ((c3[i].val() - t1 + MOD3) * i1i2_3 + (MOD3 - t2) * i2_3) % MOD3;

    constexpr unsigned long long m1_4 = MOD1 % mod;
    constexpr unsigned long long m1m2_4 = 1ULL * MOD1 * MOD2 % mod;
    c[i] = t1 + m1_4 * t2 + m1m2_4 * t3;
  }
  return c;
}
