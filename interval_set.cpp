#include <bits/stdc++.h>
using namespace std;

template <class T>
struct interval_set {
  // mp[R] = L for each [L, R)
  map<T, T> mp{{numeric_limits<T>::min(), numeric_limits<T>::min()},
               {numeric_limits<T>::max(), numeric_limits<T>::max()}};
  T size = 0;

  void insert(T left, T right) {
    if (left == right) return;
    assert(left < right);
    // remove all ranges [it->second, it->first) that overwrap with / neighbor on [left, right)
    // i.e., left <= it->first && it->second <= right
    auto it = mp.lower_bound(left);
    while (it->second <= right) {
      size -= it->first - it->second;
      chmin(left, it->second);
      chmax(right, it->first);
      it = mp.erase(it);
    }
    mp.emplace_hint(it, left, right);
    size += right - left;
  }
  void erase(T left, T right) {
    if (left == right) return;
    assert(left < right);
    auto it = mp.upper_bound(left);
    if (it->second < left) {
      mp.emplace_hint(left, left, it->second);
      size += it->left - it->second;
    }
    while (it->second < right) {
      if (it->first <= right) {
        size -= it->first - it->second;
        it = mp.erase(it);
      } else {
        size -= right - it->second;
        it->second = right;
        return;
      }
    }
  }
  bool contains(T x) {
    return mp.upper_bound(x)->second <= x;
  }
};
