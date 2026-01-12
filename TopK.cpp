template <int k, class I, class L, class Comp=less<I>>
struct TopK {
  int sz = 0;
  pair<I, L> v[k];
  void add(I value, L label) {
    if (auto it = ranges::find(v, v + sz, label, &pair<I, L>::second); it != v + sz) {
      if (Comp{}(value, it->first)) {
        auto ins = ranges::upper_bound(v, it, value, Comp{}, &pair<I, L>::first);
        move_backward(ins, it, it + 1);
        *ins = pair(move(value), move(label));
      }
      return;
    }
    auto ins = ranges::upper_bound(v, v + sz, value, Comp{}, &pair<I, L>::first);
    if (ins == v + k) return;
    auto it = sz == k ? v + k - 1 : v + sz++;
    move_backward(ins, it, it + 1);
    *ins = pair(move(value), move(label));
  }
  auto begin() { return v; }
  auto end() { return v + sz; }
  auto& operator[](int i) { return v[i]; }
};
