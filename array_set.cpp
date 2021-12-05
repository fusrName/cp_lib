template<int element_size>
struct array_set {
  int state[element_size];  // -1 if not in set else index in elems
  int elems[element_size];
  int size = 0;
  array_set() { memset(state, -1, sizeof(int) * element_size); }
  void insert(int x) {
    if (state[x] == -1) {
      state[x] = size;
      elems[size] = x;
      size++;
    }
  }
  bool contains(int x) { return state[x] != -1; }
  int* begin() { return elems; }
  int* end() { return elems + size; }
  void clear() { while(size) state[elems[--size]] = -1; }
  void erase(int x) {  // not tested
    int idx = state[x];
    if (idx != -1) {
      state[x] = -1;
      size--;
      if (idx != size) {
        int y = elems[size];
        state[y] = idx;
        elems[idx] = y;
      }
    }
  }
};