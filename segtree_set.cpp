struct segtree_set {
  static char op(char x, char y) { return x || y; }
  static char e() { return false; }
  const int n;
  segtree<char, op, e> seg;
  segtree_set(int n) : n(n), seg(n) {}
  segtree_set(const vector<char>& v) : n(v.size()), seg(v) {}
  bool contains(int x) { return seg.get(x); }
  void insert(int x) { seg.set(x, true); }
  void erase(int x) { seg.set(x, false); }
  struct iterator {
    const segtree_set& st;
    int i;
    int operator*() const { return i; }
    bool operator!=(const iterator& rhs) const { return i != rhs.i; }
    iterator& operator++() { i = *st.upper_bound(i); return *this; }
    iterator& operator--() {
      i = st.seg.min_left(i, [](char t) { return !t; }) - 1;
      return *this;
    }
  };
  iterator lower_bound(int x) const { return {*this, seg.max_right(x, [](char t) { return !t; })}; }
  iterator upper_bound(int x) const { return lower_bound(x + 1); }
  iterator begin() { return lower_bound(0); }
  iterator end() { return {*this, n}; }
};