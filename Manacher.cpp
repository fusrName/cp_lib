template <class T>
vector<int> Manacher(const T& s) {
  const int n = s.size();
  vector<int> res(n);
  for (int i = 0, r = 1; i < n;) {
    int l = 2 * i - r;
    while (0 <= l && r < n && s[l] == s[r]) l--, r++;
    res[i] = r - i;
    int j = i - 1;
    i++;
    while (i < r && i + res[j] < r) res[i++] = res[j--];
  }
  return res;
}
