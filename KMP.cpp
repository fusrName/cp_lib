template <class T>
vector<int> KMP(const T& pattern) {
  const int n = pattern.size();
  vector<int> pi(n + 1);
  pi[0] = -1;
  int border = 0;
  for (int i = 1; i < n; i++, border++) {
    const auto c = pattern[i];
    if (pattern[border] == c) {
      pi[i] = pi[border];
    } else {
      pi[i] = border;
      while (border != -1 && pattern[border] != c) {
        border = pi[border];
      }
    }
  }
  pi[n] = border;
  return pi;
}

template <class T>
vector<int> MP(const T& pattern) {
  const int n = pattern.size();
  vector<int> pi(n + 1);
  pi[0] = -1;
  int border = 0;
  for (int i = 1; i < n; i++, border++) {
    const auto c = pattern[i];
    pi[i] = border;
    while (border != -1 && pattern[border] != c) {
      border = pi[border];
    }
  }
  pi[n] = border;
  return pi;
}
