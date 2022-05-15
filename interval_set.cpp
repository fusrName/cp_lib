#include <bits/stdc++.h>
using namespace std;

template <class T>
struct interval_set {
  map<T, T> mp{{numeric_limits<T>::min(), numeric_limits<T>::min()},
               {numeric_limits<T>::max(), numeric_limits<T>::max()}};
  T size = 0;

  void insert(T l, T r) {
    if (l == r) return;
    assert(l < r);
    auto it = mp.lower_bound(l);
    if (it->first != l) --it;
    T cur_right;  // [it->first, cur_right) will be expanded to right until reaching r
    if (it->second < l) {
      it = mp.emplace(l, 0).second;
      cur_right = l;
    } else {
      if (r <= it->second) return;
      cur_right = it->second;
    }
    // it->second gets updated before return
    ++it;
    while (true) {
      if (r < it->first) {
        size += r - cur_right;
        it->second = r;
        return;
      }
      size += it->first - cur_right;
      cur_right = it->second;
      it = mp.erase(it);
      if (r <= cur_right) {
        it->second = cur_right;
        return;
      }
    }
  }
  void erase(T l, T r) {
    if (l == r) return;
    assert(l < r);
    auto it = mp.lower_bound(l);
    if (it->first != l) {
      auto pit = prev(it);
      if (l < pit->second) {
        size -= pit->second - l;
        pit->second = l;
      }
    }
    while (true) {
      if (r <= it->first) return;
      if (r < it->second) {
        size -= r - it->first;
        mp.emplace_hint(mp.erase(it), r, it->second);
        return;
      }
      size -= it->second - it->first;
      it = mp.erase(it);
    }
  }
  bool contains(T x) {
    auto it = lower_bound(x);
    if (it->first == x) return true;
    --it;
    return x < it->second;
  }
};
