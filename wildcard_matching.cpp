// pattern shift: [-(m-1), (n-1)]
template<class S1, class S2, class F, class G>
vector<mint> wildcard_matching(const S1& text, const S2& pattern, F mapping, G is_wildcard) {
  const int n = size(text), m = size(pattern);
  // sum (t[i+k]-p[i])^2 * is_wildcard(p[i])
  // = sum t[i+k]^2 * is_wildcard(p[i])
  //   - 2 sum t[i+k] p[i] is_wildcard(p[i])
  //   + sum p[i]^2 * is_wildcard(p[i])
  vector<mint> t1(n), t2(n), p0(m), p1(m);
  vector<mint> res(n + m - 1);
  for (int i = 0; i < n; i++) {
    mint ti = mapping(text[i]);
    t1[i] = ti;
    t2[i] = ti * ti;
  }
  for (int i = 0; i < m; i++) {
    if (is_wildcard(pattern[i])) continue;
    mint pi = mapping(pattern[i]);
    p0[m - 1 - i] = 1;
    p1[m - 1 - i] = pi;
  }
  // sum t[i+k]^2 * is_wildcard(p[i])
  vector<mint> r = convolution(t2, p0);
  for (int i = 0; i < n + m - 1; i++) res[i] += r[i];
  // - 2 sum t[i+k] p[i] is_wildcard(p[i])
  r = convolution(t1, p1);
  for (int i = 0; i < n + m - 1; i++) res[i] -= 2 * r[i];
  // + sum p[i]^2 * is_wildcard(p[i])
  mint p2;
  for (int i = 0; i < n + m - 1; i++) {
    if (i < m && !is_wildcard(pattern[i])) {
      mint pi = mapping(pattern[i]);
      p2 += pi * pi;
    }
    res[n + m - 2 - i] += p2;
    if (i >= n - 1 && !is_wildcard(pattern[i - (n - 1)])) {
      mint pi = mapping(pattern[i - (n - 1)]);
      p2 -= pi * pi;
    }
  }
  return res;
}