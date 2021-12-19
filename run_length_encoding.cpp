// returns a vector of (val, len)
template <class Iter>
auto run_length_encoding(Iter first, Iter last) {
  vector<pair<typename iterator_traits<Iter>::value_type, int>> res;
  if (first != last) {
    auto v = *first++;
    int cnt = 1;
    for (; first != last; first++) {
      if (*first == v) cnt++;
      else {
        res.emplace_back(v, cnt);
        v = *first;
        cnt = 1;
      }
    }
    res.emplace_back(v, cnt);
  }
  return res;
}


template <class T>
void rle_push_back(vector<pair<T, int>>& a, T v, int len=1) {
  if (!a.empty() && a.back().first == v) a.back().second += len;
  else a.emplace_back(v, len);
}

template <class T>
void rle_push_back(vector<pair<T, int>>& a, pair<T, int> vl) {
  if (!a.empty() && a.back().first == vl.first) a.back().second += vl.second;
  else a.emplace_back(vl);
}