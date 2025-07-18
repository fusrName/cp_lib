template <ranges::range R>
auto minimal_factors(R&& a) {
  using T = ranges::range_value_t<R>;
  vector<T> factors, next_factors;
  for (auto x : a) {
    for (auto f : factors) while (x % f == 0) x /= f;
    if (x == 1) continue;
    next_factors.clear();
    auto strip = [&](auto&& self, auto v, auto f) {
      if (f == 1) return v;
      while (v % f == 0) v /= f;
      auto g = gcd(v, f);
      if (g == 1) {
        next_factors.emplace_back(f);
        return v;
      }
      v /= g, f /= g;
      g = self(self, g, f);
      return self(self, v, g);
    };
    for (auto f : factors) x = strip(strip, f, x);
    if (x != 1) next_factors.emplace_back(x);
    swap(factors, next_factors);
  }
  return factors;
}

auto extract_common_factor(auto x, auto c) {
  c = gcd(c, x);
  x /= c;
  while (true) {
    auto g = gcd(c, x);
    if (g == 1) return c;
    x /= g;
    c *= g;
  }
}
