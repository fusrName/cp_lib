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
    int pre = -1;
    while (parent_or_size[a] >= 0) {
      int na = parent_or_size[a];
      parent_or_size[a] = pre;
      pre = a;
      a = na;
    }
    S w = e();
    while (pre != -1) {
      w = op(w, weight[pre]);
      weight[pre] = w;
      int npre = parent_or_size[pre];
      parent_or_size[pre] = a;
      pre = npre;
    }
    return a;
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




// test code
#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for(int i = 0; i < int(n); i++)
constexpr int SZ = 10;
using S = array<int, 10>;
S op(S g, S h) {
  // g;h
  S res;
  rep(i, SZ) res[i] = h[g[i]];
  return res;
}
S e() {
  S res;
  rep(i, SZ) res[i] = i;
  return res;
}
S inv(S g) {
  S res;
  rep(i, SZ) res[g[i]] = i;
  return res;
}
// generates random numbers
std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
using dist_type = std::uniform_int_distribution<>;
using param_type = dist_type::param_type;
dist_type dist;

#include<atcoder/dsu>

void test() {
  dist.param(param_type(1, 100));
  int n = dist(gen);
  // actual data;
  vector<S> A(n);
  for(auto& g : A) {
    rep(i, SZ) g[i] = i;
    shuffle(g.begin(), g.end(), gen);
  }
  weighted_union_find<S, op, e, inv> d(n);
  atcoder::dsu uf(n);
  dist.param(param_type(20, 300));
  int q = dist(gen);
  int q0 = 0, q1 = 0;
  rep(_, q) {
    dist.param(param_type(0, 1));
    int type = dist(gen);
    dist.param(param_type(0, n - 1));
    int u = dist(gen), v = dist(gen);
    // calc weight u->v; gu^-1 gv
    S w;
    rep(i, SZ) w[A[u][i]] = A[v][i];
    if (type == 0) {
      q0++;
      // check diff if connected
      if (!uf.same(u, v)) continue;
      S res = d.diff(u, v);
      assert(res == w);
    } else {
      q1++;
      // do merge if not connected
      if (uf.same(u, v)) continue;
      d.merge(u, v, w);
      uf.merge(u, v);
    }
  }
  cout << q << " queries successfully processed; ";
  cout << q0 << ' ' << q1 << ' ' << d.groups().size() << endl;
}

int main(void) {
  int tt;
  cin >> tt;
  while(tt--) test();
}