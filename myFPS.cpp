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
    int tsz = (*this).size(), fsz = f.size();
    if (tsz < fsz) {
      (*this).insert((*this).end(), f.begin() + tsz, f.end());
      while (tsz--) (*this)[tsz] += f[tsz];
    } else {
      while (fsz--) (*this)[fsz] += f[fsz];
    }
    return *this;
  }
  FPS& operator+=(FPS&& f) {
    int tsz = (*this).size(), fsz = f.size();
    if (tsz < fsz) {
      swap(*this, f);
      while (tsz--) (*this)[tsz] += f[tsz];
    } else {
      while (fsz--) (*this)[fsz] += f[fsz];
    }
    return *this;
  }
  friend FPS operator+(const FPS& f, const FPS& g) {
    int fsz = f.size(), gsz = g.size();
    FPS h;
    if (fsz < gsz) {
      h.reserve(gsz);
      for (int i = 0; i < fsz; i++) h.emplace_back(f[i] + g[i]);
      h.insert(h.end(), g.begin() + fsz, g.end());
    } else {
      h.reserve(fsz);
      for (int i = 0; i < gsz; i++) h.emplace_back(f[i] + g[i]);
      h.insert(h.end(), f.begin() + gsz, f.end());
    }
    return h;
  }
  friend FPS operator+(FPS&& f, const FPS& g) {
    f += g;
    return move(f);
  }
  friend FPS operator+(const FPS& f, FPS&& g) {
    g += f;
    return move(g);
  }
  friend FPS operator+(FPS&& f, FPS&& g) {
    f += move(g);
    return move(f);
  }

  FPS& operator-=(const FPS& f) {
    int tsz = (*this).size(), fsz = f.size();
    if (tsz < fsz) {
      (*this).reserve(fsz);
      for (int i = tsz; i < fsz; i++) {
        (*this).emplace_back(-f[i]);
      }
      while (tsz--) (*this)[tsz] -= f[tsz];
    } else {
      while (fsz--) (*this)[fsz] -= f[fsz];
    }
    return *this;
  }
  FPS& operator-=(FPS&& f) {
    int tsz = (*this).size(), fsz = f.size();
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
    if (fsz < gsz) {
      h.reserve(gsz);
      int i = 0;
      for (; i < fsz; i++) h.emplace_back(f[i] - g[i]);
      for (; i < gsz; i++) h.emplace_back(-g[i]);
    } else {
      h.reserve(fsz);
      for (int i = 0; i < gsz; i++) h.emplace_back(f[i] - g[i]);
      h.insert(h.end(), f.begin() + gsz, f.end());
    }
    return h;
  }
  friend FPS operator-(FPS&& f, const FPS& g) {
    f -= g;
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
    f -= move(g);
    return move(f);
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

  static void multiply_naive(FPS& f, const FPS& g) {
    int n = int(f.size()), m = int(g.size());
    if (!n || !m) {
      f.clear();
    } else {
      f.resize(n + m - 1);
      for (int i = n - 1; i >= 0; i--) {
        T fi = f[i];
        for (int j = m - 1; j > 0; j--) {
          f[i + j] += fi * g[j];
        }
        f[i] = fi * g[0];
      }
    }
  }

  static void multiply_fft(FPS& f_, const FPS& g_) {
    int n = int(f_.size()), m = int(g_.size());
    if (!n || !m) {
      f_.clear();
      return;
    }
    int z = 1 << internal::ceil_pow2(n + m - 1);
    FPS f, g;
    swap(f, f_);
    swap(g, tmp);
    g.clear();
    g.reserve(z);
    g = g_;

    f.resize(z);
    internal::butterfly(f);
    g.resize(z);
    internal::butterfly(g);
    for (int i = 0; i < z; i++) {
      f[i] *= g[i];
    }
    internal::butterfly_inv(f);
    f.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) f[i] *= iz;

    swap(g, tmp);
    swap(f, f_);
    return;
  }

  static void multiply_fft(FPS& f_, FPS&& g_) {
    int n = int(f_.size()), m = int(g_.size());
    if (!n || !m) {
      f_.clear();
      if (g_.capacity() > tmp.capacity()) swap(g_, tmp);
      return;
    }
    int z = 1 << internal::ceil_pow2(n + m - 1);
    if (int(g_.capacity()) < z) {
      return multiply_fft(f_, g_);
    }
    FPS f, g = move(g_);
    swap(f, f_);

    f.resize(z);
    internal::butterfly(f);
    g.resize(z);
    internal::butterfly(g);
    for (int i = 0; i < z; i++) {
      f[i] *= g[i];
    }
    internal::butterfly_inv(f);
    f.resize(n + m - 1);
    mint iz = mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) f[i] *= iz;

    swap(f, f_);
    if (g.capacity() > tmp.capacity()) swap(g, tmp);
    return;
  }

  FPS& operator*=(const FPS& f) {
    int n = int((*this).size()), m = int(f.size());
    if (min(n, m) <= 60) multiply_naive(*this, f);
    else multiply_fft(*this, f);
    return *this;
  }
  FPS& operator*=(FPS&& f) {
    if ((*this).capacity() < f.capacity()) swap(*this, f);
    int n = int((*this).size()), m = int(f.size());
    if (min(n, m) <= 60) multiply_naive(*this, f);
    else multiply_fft(*this, move(f));
    return *this;
  }
  friend FPS operator*(FPS&& f, FPS&& g) { return move(f *= move(g)); }
  friend FPS operator*(FPS&& f, const FPS& g) { return move(f *= g); }
  friend FPS operator*(const FPS& f, FPS&& g) { return move(g *= f); }
  friend FPS operator*(const FPS& f, const FPS& g) {
    int n = int(f.size()), m = int(g.size());
    if (!n || !m) return {};
    int z = 1 << internal::ceil_pow2(n + m - 1);
    FPS h;
    h.reserve(z);
    h = f;
    h *= g;
    return h;
  }
  FPS& operator+=(T x) {
    if ((*this).empty()) (*this).emplace_back(x);
    else (*this)[0] += x;
    return *this;
  }
  friend FPS operator+(FPS f, T x) {
    f += x;
    return move(f);
  }
  friend FPS operator+(T x, FPS f) {
    f += x;
    return move(f);
  }
  FPS operator-=(T x) {
    if ((*this).empty()) (*this).emplace_back(-x);
    else (*this)[0] -= x;
    return *this;
  }
  friend FPS operator-(FPS f, T x) {
    f -= x;
    return move(f);
  }
  friend FPS operator-(T x, FPS f) { return -move(f) + x; }
  FPS& operator*=(T x) {
    for (T& i: *this) i *= x;
    return *this;
  }
  friend FPS operator*(FPS f, T x) {
    f *= x;
    return move(f);
  }
  friend FPS operator*(T x, FPS f) {
    f *= x;
    return move(f);
  }
  FPS operator/=(T x) {
    if constexpr (internal::is_modint<T>::value) {
      T ix = x.inv();
      for (T& i: *this) i *= ix;
    } else {
      for (T& i: *this) i /= x;
    }
    return *this;
  }
  friend FPS operator/(FPS f, T x) {
    f /= x;
    return move(f);
  }
  FPS& operator<<=(int x) {
    assert(x >= 0);
    (*this).insert((*this).begin(), x, T());
  }
  friend FPS operator<<(FPS f, int x) {
    f <<= x;
    return move(f);
  }
  FPS& operator>>=(int x) {
    assert(x >= 0);
    int tsz = (*this).size();
    if (tsz <= x) (*this).clear();
    else (*this).erase((*this).begin(), (*this).begin() + x);
  }
  friend FPS operator>>(FPS f, int x) {
    f >>= x;
    return move(f);
  }

 private:
  inline static FPS tmp;
};


} int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  using F = FPS<mint>;
  F f{100, 200, 300};
  F g{5, 6, 7, 8, 9, 10, 11};
  f += g;
  auto h = -move(f);
  -f;
  h *= {3, 4, 5};
  h = -move(h);
  cout << f.data() << endl;
  h = {1, 2};
  f = {5, 6, 7};
  h = f * h;
  rep(i, (int)h.size()) cout << h[i].val() << " \n"[i + 1 == (int)h.size()];
}