// S : group
template <class S, S (*op)(S, S), S (*e)(), S (*inv)(S)>
struct weighted_union_find {
 public:
  weighted_union_find() : _n(0) {}
  explicit weighted_union_find(int n) : _n(n), parent_or_size(n, -1), weight(n, e()) {}

  int merge(int a, int b, S w) {
    // W(a->b) = Wa^-1 Wb = w
    assert(0 <= a && a < _n);
    assert(0 <= b && b < _n);
    int x = leader(a), y = leader(b);
    assert(x != y);
    if (-parent_or_size[x] < -parent_or_size[y]) {
      std::swap(x, y);
      std::swap(a, b);
      w = inv(w);
    }
    // Wa^-1 Wy Wb = w
    // Wy = Wa w Wb^-1
    weight[y] = op(op(weight[a], w), inv(weight[b]));
    parent_or_size[x] += parent_or_size[y];
    parent_or_size[y] = x;
    return x;
  }

  S diff(int a, int b) {
    // W(a->b) = Wa^-1 Wb
    int x = leader(a), y = leader(b);
    assert(x == y);
    return op(inv(weight[a]), weight[b]);
  }

  bool same(int a, int b) {
    assert(0 <= a && a < _n);
    assert(0 <= b && b < _n);
    return leader(a) == leader(b);
  }

  int leader(int a) {
    assert(0 <= a && a < _n);
    int x = a;
    S w = e();
    while (parent_or_size[x] >= 0) {
      w = op(w, weight[x]);
      x = parent_or_size[x];
    }
    while (a != x) {
      int na = parent_or_size[a];
      S nw = op(inv(weight[a]), w);
      parent_or_size[a] = x;
      weight[a] = w;
      a = na;
      w = nw;
    }
    return x;
  }

  int size(int a) {
    assert(0 <= a && a < _n);
    return -parent_or_size[leader(a)];
  }

  std::vector<std::vector<int>> groups() {
    std::vector<int> leader_buf(_n), group_size(_n);
    for (int i = 0; i < _n; i++) {
      leader_buf[i] = leader(i);
      group_size[leader_buf[i]]++;
    }
    std::vector<std::vector<int>> result(_n);
    for (int i = 0; i < _n; i++) {
      result[i].reserve(group_size[i]);
    }
    for (int i = 0; i < _n; i++) {
      result[leader_buf[i]].push_back(i);
    }
    result.erase(
      std::remove_if(result.begin(), result.end(),
              [&](const std::vector<int>& v) { return v.empty(); }),
      result.end());
    return result;
  }

 private:
  int _n;
  // root node: -1 * component size
  // otherwise: parent
  std::vector<int> parent_or_size;
  std::vector<S> weight;
};