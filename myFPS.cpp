#include<bits/stdc++.h>
namespace {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include<atcoder/all>
#pragma GCC diagnostic pop
using namespace std;
using namespace atcoder;
#define rep(i,n)for (int i = 0; i < int(n); ++i)
#define rrep(i,n)for (int i = int(n)-1; i >= 0; --i)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
template<class T> void chmax(T& a, const T& b) { a = max(a, b); }
template<class T> void chmin(T& a, const T& b) { a = min(a, b); }
using ll = long long;
using P = pair<int,int>;
using VI = vector<int>;
using VVI = vector<VI>;
using VL = vector<ll>;
using VVL = vector<VL>;
using mint = modint998244353;

template <class T>
struct FPS : vector<T> {
  using vector<T>::vector;
  
  FPS& operator+=(const FPS& f) {
    int fsz = f.size();
    if ((int)(*this).size() < fsz) (*this).resize(fsz);
    while (fsz--) (*this)[fsz] += f[fsz];
    return *this;
  }
  FPS& operator+=(FPS&& f) {
    int fsz = f.size();
    if ((int)(*this).size() < fsz) swap(*this, f);
    while (fsz--) (*this)[fsz] += f[fsz];
    return *this;
  }
  friend FPS operator+(const FPS& f, const FPS& g) {
    int fsz = f.size(), gsz = g.size();
    if (fsz < gsz) return g + f;
    FPS h = f;
    while (gsz--) h[gsz] += g[gsz];
    return h;
  }
  friend FPS operator+(FPS&& f, const FPS& g) {
    int fsz = f.size(), gsz = g.size();
    if (fsz < gsz) f.resize(gsz);
    while (gsz--) f[gsz] += g[gsz];
    return move(f);
  }
  friend FPS operator+(const FPS& f, FPS&& g) { return move(g) + f; }
  friend FPS operator+(FPS&& f, FPS&& g) {
    int fsz = f.size(), gsz = g.size();
    if (fsz < gsz) swap(f, g);
    while (gsz--) f[gsz] += g[gsz];
    return move(f);
  }

  FPS& operator-=(const FPS& f) {
    int fsz = f.size();
    if ((int)(*this).size() < fsz) (*this).resize(fsz);
    while (fsz--) (*this)[fsz] -= f[fsz];
    return *this;
  }
  FPS& operator-=(FPS&& f) {
    int fsz = f.size(), tsz = (*this).size();
    if (tsz < fsz) {
      swap(*this, f);
      while (fsz-- != tsz) (*this)[fsz] = -(*this)[fsz];
      while (tsz--) (*this)[tsz] = f[tsz] - (*this)[tsz];
    } else {
      while (fsz--) (*this)[fsz] -= f[fsz];
    }
    return *this;
  }
  friend FPS operator-(const FPS& f, const FPS& g) {
    int fsz = f.size(), gsz = g.size();
    FPS h;
    h.reserve(max(fsz, gsz));
    h = f;
    if (fsz < gsz) h.resize(gsz);
    while (gsz--) h[gsz] -= g[gsz];
    return h;
  }
  friend FPS operator-(FPS&& f, const FPS& g) {
    int fsz = f.size(), gsz = g.size();
    if (fsz < gsz) f.resize(gsz);
    while (gsz--) f[gsz] -= g[gsz];
    return move(f);
  }
  friend FPS operator-(const FPS& f, FPS&& g) {
    int fsz = f.size(), gsz = g.size();
    if (fsz < gsz) {
      while (gsz-- != fsz) g[gsz] = -g[gsz];
      while (fsz--) g[fsz] = f[fsz] - g[fsz];
    } else {
      g.insert(g.end(), f.begin() + gsz, f.end());
      while (gsz--) g[gsz] = f[gsz] - g[gsz];
    }
    return move(g);
  }
  friend FPS operator-(FPS&& f, FPS&& g) {
    int fsz = f.size(), gsz = g.size();
    if (fsz < gsz) {
      while (gsz-- != fsz) g[gsz] = -g[gsz];
      while (fsz--) g[fsz] = f[fsz] - g[fsz];
      return move(g);
    } else {
      while (gsz--) f[gsz] -= g[gsz];
      return move(f);
    }
  }
  FPS operator-() && {
    for (T& x: *this) x = -x;
    return move(*this);
  }
  FPS operator-() const& {
    FPS f = *this;
    for (T& x: f) x = -x;
    return f;
  }
};


} int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  using F = FPS<mint>;
  F f{100, 200, 300};
  F g{5, 6, 7, 8, 9, 10, 11};
  auto h = -move(f);
  -f;
  cout << f.data() << endl;
  rep(i, (int)h.size()) cout << h[i].val() << " \n"[i + 1 == (int)h.size()];
}