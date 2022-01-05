#include <bits/stdc++.h>
#include <atcoder/convolution>
using namespace std;
using namespace atcoder;

template<class T>
struct FormalPowerSeries : vector<T> {
  using vector<T>::vector;
  using vector<T>::operator=;
  using F = FormalPowerSeries;

  F operator-() const {
    F res(*this);
    for (auto &e : res) e = -e;
    return res;
  }
  F& operator*=(const T& g) {
    for (auto &e : *this) e *= g;
    return *this;
  }
  F& operator/=(const T& g) {
    assert(g != T());
    *this *= g.inv();
    return *this;
  }
  F& operator+=(const F& g) {
    const int m = g.size();
    assert(m <= size());
    for (int i = 0; i < m; i++) (*this)[i] += g[i];
    return *this;
  }
  F& operator-=(const F& g) {
    const int m = g.size();
    assert(m <= size());
    for (int i = 0; i < m; i++) (*this)[i] -= g[i];
    return *this;
  }
  F& operator<<=(const int d) {
    const int n = size();
    for (int i = n - 1; i >= d; i--) (*this)[i] = (*this)[i - d];
    for (int i = d - 1; i >= 0; i--) (*this)[i] = T();
    return *this;
  }
  F& operator>>=(const int d) {
    const int n = size();
    for (int i = 0; i < n - d; i++) (*this)[i] = (*this)[i + d];
    for (int i = n - d; i < n; i++) (*this)[i] = T();
    return *this;
  }
  F inv(int d = -1) const {
    int n = (*this).size();
    assert(n != 0 && (*this)[0] != 0);
    if (d == -1) d = n;
    assert(d > 0);
    const int mx = internal::ceil_pow2(d);
    F res; res.reserve(mx);
    res.emplace_back((*this)[0].inv());
    F f; f.reserve(mx);
    F g; g.reserve(mx);
    while (res.size() < d) {
      const int m = res.size();
      f.assign(begin(), begin() + min(n, 2 * m));
      g = res;
      f.resize(2 * m), internal::butterfly(f);
      g.resize(2 * m), internal::butterfly(g);
      for (int i = 0; i < 2 * m; i++) f[i] *= g[i];
      internal::butterfly_inv(f);
      f.erase(f.begin(), f.begin() + m);
      f.resize(2 * m), internal::butterfly(f);
      for (int i = 0; i < 2 * m; i++) f[i] *= g[i];
      internal::butterfly_inv(f);
      T iz = T(2 * m).inv(); iz *= -iz;
      for (int i = 0; i < m; i++) res.emplace_back(f[i] * iz);
    }
    res.resize(d);
    return res;
  }

  // // fast: FMT-friendly modulus only
  // F& operator*=(const F& g) {
  //   *this = convolution(*this, g);
  //   return *this;
  // }
  // F& operator/=(const F& g) {
  //   *this = convolution(*this, g.inv(n));
  //   return *this;
  // }

  // naive
  // F& operator*=(const F& g) {
  //   const int n = (*this).size(), m = g.size();
  //   resize(n + m - 1);
  //   for (int i = n - 1; i >= 0; i--) {
  //     for (int j = m - 1; j >= 1; j--) (*this)[i + j] += (*this)[i] * g[j];
  //     (*this)[i] *= g[0];
  //   }
  //   return *this;
  // }
  // F& operator/=(const F& g) {
  //   const int n = (*this).size(), m = g.size();
  //   assert(m > 0 && g[0] != T());
  //   T ig0 = g[0].inv();
  //   rep(i, n) {
  //     rep2(j, 1, min(i+1, m)) (*this)[i] -= (*this)[i-j] * g[j];
  //     (*this)[i] *= ig0;
  //   }
  //   return *this;
  // }

  // sparse
  F& operator*=(vector<pair<int, T>> g) {
    int n = (*this).size();
    auto [d, c] = g.front();
    if (d == 0) g.erase(g.begin());
    else c = 0;
    drep(i, n) {
      (*this)[i] *= c;
      for (auto &[j, b] : g) {
        if (j > i) break;
        (*this)[i] += (*this)[i-j] * b;
      }
    }
    return *this;
  }
  F& operator/=(vector<pair<int, T>> g) {
    int n = (*this).size();
    auto [d, c] = g.front();
    assert(d == 0 && c != T(0));
    T ic = c.inv();
    g.erase(g.begin());
    rep(i, n) {
      for (auto &[j, b] : g) {
        if (j > i) break;
        (*this)[i] -= (*this)[i-j] * b;
      }
      (*this)[i] *= ic;
    }
    return *this;
  }

  // multiply and divide (1 + cz^d)
  void multiply(const int d, const T c) { 
    int n = (*this).size();
    if (c == T(1)) drep(i, n-d) (*this)[i+d] += (*this)[i];
    else if (c == T(-1)) drep(i, n-d) (*this)[i+d] -= (*this)[i];
    else drep(i, n-d) (*this)[i+d] += (*this)[i] * c;
  }
  void divide(const int d, const T c) {
    int n = (*this).size();
    if (c == T(1)) rep(i, n-d) (*this)[i+d] -= (*this)[i];
    else if (c == T(-1)) rep(i, n-d) (*this)[i+d] += (*this)[i];
    else rep(i, n-d) (*this)[i+d] -= (*this)[i] * c;
  }

  T eval(const T& a) const {
    T x(1), res(0);
    for (auto e : *this) res += e * x, x *= a;
    return res;
  }

  F operator*(const T& g) const { return F(*this) *= g; }
  F operator/(const T& g) const { return F(*this) /= g; }
  F operator+(const F& g) const { return F(*this) += g; }
  F operator-(const F& g) const { return F(*this) -= g; }
  F operator<<(const int d) const { return F(*this) <<= d; }
  F operator>>(const int d) const { return F(*this) >>= d; }
  F operator*(const F& g) const { return F(*this) *= g; }
  F operator/(const F& g) const { return F(*this) /= g; }
  F operator*(vector<pair<int, T>> g) const { return F(*this) *= g; }
  F operator/(vector<pair<int, T>> g) const { return F(*this) /= g; }
};