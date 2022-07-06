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

template <class T>
vector<int> Manacher_cursor(const T& s) {
  const int n = s.size();
  vector<int> res(n + 1);
  for (int i = 1, r = 1; i < n;) {
    int l = 2 * i - r;
    while (l && r < n && s[l - 1] == s[r]) l--, r++;
    int radius = r - i;
    res[i] = radius;
    if (radius == 0) {
      i = r = i + 1;
    } else {
      int j = i - 1;
      i++;
      while (i + res[j] < r) res[i++] = res[j--];
    }
  }
  return res;
}
