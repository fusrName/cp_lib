template<class T, int element_size>
struct array_map {
  int state[element_size];  // -1 if not in set else index in elems
  pair<int, T> elems[element_size];
  int size = 0;
  array_map() { memset(state, -1, sizeof(int) * element_size); }
  bool contains(int x) { return state[x] != -1; }
  pair<int, T>* begin() { return elems; }
  pair<int, T>* end() { return elems + size; }
  void clear() { while(size) state[elems[--size].first] = -1; }
  void erase(int x) {  // not tested
    int idx = state[x];
    if (idx != -1) {
      state[x] = -1;
      size--;
      if (idx != size) {
        int y = elems[size].first;
        state[y] = idx;
        elems[idx] = move(elems[size]);
      }
    }
  }
  T& operator[](int x) {
    if (state[x] == -1) {
      state[x] = size;
      elems[size].first = x;
      elems[size].second = T();
      size++;
    }
    return elems[state[x]].second;
  }
};