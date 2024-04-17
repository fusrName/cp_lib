template <class S>
struct simple_deque {
  vector<S> d;
  int from = 0, n = 0;
  void ensure_size() {
    if (ssize(d) == n) {
      d.resize(n == 0 ? 1 : 2 * n);
      for (int i = n - 1; i >= from; i--) d[n + i] = move(d[i]);
      from += n;
    }
  }
  int conv_idx(int i) {
    i += from;
    if (i >= ssize(d)) i -= ssize(d);
    return i;
  }
  void emplace_back(auto&&... args) {
    ensure_size();
    d[conv_idx(n++)] = S(forward<decltype(args)>(args)...);
  }
  void emplace_front(auto&&... args) {
    ensure_size();
    if (from == 0) from = ssize(d);
    from--, n++;
    d[from] = S(forward<decltype(args)>(args)...);
  }
  void pop_back() {
    assert(n > 0);
    n--;
  }
  void pop_front() {
    assert(n > 0);
    n--;
    if (++from == ssize(d)) from = 0;
  }
  S& back() {
    assert(n > 0);
    return d[conv_idx(n - 1)];
  }
  S& front() {
    assert(n > 0);
    return d[from];
  }
  S& operator[](int i) {
    assert(0 <= i && i < n);
    return d[conv_idx(i)];
  }
  int size() { return n; }
  void clear() { n = 0; }
};
