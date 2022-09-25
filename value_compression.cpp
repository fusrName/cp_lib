template <class S>
struct value_compression : vector<S> {
  bool built = false;
  using VS = vector<S>;
  using VS::VS;
  value_compression(vector<S> v) : vector<S>(move(v)) {}
  void build() {
    sort(this->begin(), this->end());
    this->erase(unique(this->begin(), this->end()), this->end());
    built = true;
  }
  template <class T>
  void convert(T first, T last) {
    assert(built);
    for (; first != last; ++first) *first = (*this)(*first);
  }
  int operator()(S x) {
    assert(built);
    return lower_bound(this->begin(), this->end(), x) - this->begin();
  }
  void clear() {
    this->clear();
    built = false;
  }
};
