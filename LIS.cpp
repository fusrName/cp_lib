int LIS(const std::vector<int>& x) {
  int n = x.size();
  std::vector<int> d;
  d.reserve(n);
  for(int xi: x) {
    auto it = std::lower_bound(d.begin(), d.end(), xi);
    if (it == d.end()) d.emplace_back(xi);
    else *it = xi;
  }
  return d.size();
}

int LNDS(const std::vector<int>& x) {
  int n = x.size();
  std::vector<int> d;
  d.reserve(n);
  for(int xi: x) {
    auto it = std::upper_bound(d.begin(), d.end(), xi);
    if (it == d.end()) d.emplace_back(xi);
    else *it = xi;
  }
  return d.size();
}

// takes iterators [first, last)
template<class T>
int LIS(T first, T last) {
  using V = typename std::iterator_traits<T>::value_type;
  using C = typename std::iterator_traits<T>::iterator_category;
  std::vector<V> d;
  if constexpr (std::is_same_v<C, std::random_access_iterator_tag>) {
    d.reserve(last - first);
  }
  for(; first != last; first++) {
    V xi = *first;
    auto it = std::lower_bound(d.begin(), d.end(), xi);
    if (it == d.end()) d.emplace_back(xi);
    else *it = xi;
  }
  return d.size();
}


// length of LIS for each last element
template<class T>
std::vector<int> LIS(T first, T last) {
  using V = typename std::iterator_traits<T>::value_type;
  using C = typename std::iterator_traits<T>::iterator_category;
  std::vector<V> d;
  std::vector<int> res;
  if constexpr (std::is_same_v<C, std::random_access_iterator_tag>) {
    d.reserve(last - first);
    res.reserve(last - first);
  }
  for(; first != last; first++) {
    V xi = *first;
    auto it = std::lower_bound(d.begin(), d.end(), xi);
    if (it == d.end()) d.emplace_back(xi);
    else *it = xi;
    res.emplace_back(it - d.begin() + 1);
  }
  return res;
}