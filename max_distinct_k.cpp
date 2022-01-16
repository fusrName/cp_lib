template <class T>
long long max_distinct_k(vector<T> a, int k) {
  const int n = a.size();
  if (n < k) return 0;
  sort(a.begin(), a.end());
  const int base = n - k;
  long long rest = accumulate(a.begin(), a.begin() + base, 0LL);
  for (int i = base + 1; i < n; i++) {
    long long mx = rest / (i - base);
    if (mx < a[i] - a[i - 1]) {
      return a[i - 1] + mx;
    }
    rest -= (a[i] - a[i - 1]) * (i - base);
  }
  return a[n - 1] + rest / k;
}