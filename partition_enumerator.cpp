struct partition_enumerator : vector<int> {
  using vector<int>::vector;
  partition_enumerator(int n) : vector<int>(n, 1) {}
  bool next_partition() {
    const int n = size();
    if (n <= 1) {
      if (n == 1) assign((*this)[0], 1);
      return false;
    }
    int mn = ++(*this)[n - 2];
    int rest = back() - 1;
    pop_back();
    while (rest >= mn) {
      push_back(mn);
      rest -= mn;
    }
    back() += rest;
    return true;
  }
};