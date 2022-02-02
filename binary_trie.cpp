struct binary_trie {
  const int bit_length = 20;
  using T = unsigned int;

  struct Node {
    int count = 0;
    int idx[2] = {-1, -1};
  };
  inline static struct : vector<Node> {
    vector<int> free_id;
    int create() {
      if (free_id.empty()) {
        int res = size();
        emplace_back();
        return res;
      } else {
        int res = free_id.back();
        free_id.pop_back();
        (*this)[res] = {};
        return res;
      }
    }
    void release(int i) { free_id.emplace_back(i); }
  } nodes;

  const int root = nodes.create();

  binary_trie() {}
  binary_trie(binary_trie&& rhs) { swap(nodes[root], nodes[rhs.root]); }
  binary_trie& operator=(binary_trie&& rhs) {
    swap(nodes[root], nodes[rhs.root]);
    return *this;
  }

  void add(T x) {
    int p = root;
    nodes[p].count++;
    for (int k = bit_length - 1; k >= 0; k--) {
      bool bit = x >> k & 1;
      int nxt = nodes[p].idx[bit];
      if (nxt == -1) nxt = nodes[p].idx[bit] = nodes.create();
      p = nxt;
      nodes[p].count++;
    }
  }
  T xor_min(T x) {
    assert(nodes[root].count);
    int p = root;
    T res = 0;
    for (int k = bit_length - 1; k >= 0; k--) {
      bool bit = x >> k & 1;
      if (nodes[p].idx[bit] == -1) bit = !bit;
      p = nodes[p].idx[bit];
      res = res << 1 | bit;
    }
    return x ^ res;
  }
  T xor_max(T x) {
    assert(nodes[root].count);
    int p = root;
    T res = 0;
    for (int k = bit_length - 1; k >= 0; k--) {
      bool bit = !(x >> k & 1);
      if (nodes[p].idx[bit] == -1) bit = !bit;
      p = nodes[p].idx[bit];
      res = res << 1 | bit;
    }
    return x ^ res;
  }
  bool contains(T x) {
    int p = root;
    for (int k = bit_length - 1; k >= 0; k--) {
      bool bit = x >> k & 1;
      p = nodes[p].idx[bit];
      if (p == -1) return false;
    }
    return true;
  }
  binary_trie extract_first(int size) {
    assert(0 <= size && size <= nodes[root].count);
    binary_trie res;
    if (nodes[root].count == size) {
      swap(nodes[root], nodes[res.root]);
      return res;
    } else if (size == 0) return res;
    int p_from = root;
    int p_to = res.root;
    while (true) {
      nodes[p_from].count -= size;
      nodes[p_to].count = size;
      int nxt0 = nodes[p_from].idx[0];
      if (nxt0 == -1) {
        if (nodes[p_from].idx[1] == -1) break;
        p_from = nodes[p_from].idx[1];
        p_to = nodes[p_to].idx[1] = nodes.create();
      } else {
        if (size < nodes[nxt0].count) {
          p_from = nxt0;
          p_to = nodes[p_to].idx[0] = nodes.create();
        } else {
          nodes[p_to].idx[0] = nxt0;
          nodes[p_from].idx[0] = -1;
          size -= nodes[nxt0].count;
          if (size == 0) break;
          p_from = nodes[p_from].idx[1];
          p_to = nodes[p_to].idx[1] = nodes.create();
        }
      }
    }
    return res;
  }
  binary_trie extract_last(int size) {
    assert(0 <= size && size <= nodes[root].count);
    binary_trie res = extract_first(nodes[root].count - size);
    swap(res, *this);
    return res;
  }
  void merge_in(binary_trie&& bt) {
    static vector<pair<int, int>> todo;
    todo.emplace_back(root, bt.root);
    do {
      auto [p, q] = todo.back(); todo.pop_back();
      nodes[p].count += nodes[q].count;
      for (int i = 0; i < 2; i++) {
        if (nodes[p].idx[i] == -1) {
          nodes[p].idx[i] = nodes[q].idx[i];
          nodes[q].idx[i] = -1;
        } else if (nodes[q].idx[i] != -1) {
          todo.emplace_back(nodes[p].idx[i], nodes[q].idx[i]);
        }
      }
    } while (!todo.empty());
  }
  ~binary_trie() {
    static vector<int> todo;
    todo.emplace_back(root);
    do {
      int i = todo.back(); todo.pop_back();
      for (int j : nodes[i].idx) if (j != -1) todo.emplace_back(j);
      nodes.release(i);
    } while (!todo.empty());
  }
  vector<T> get_all() {
    vector<T> res;
    res.reserve(nodes[root].count);
    auto dfs = [&](auto&& self, int p, int k, T v) -> void {
      if (k == 0) {
        for (int i = nodes[p].count; i > 0; i--) {
          res.emplace_back(v);
        }
      } else {
        for (int i : {0, 1}) {
          int nxt = nodes[p].idx[i];
          if (nxt != -1) self(self, nxt, k - 1, v << 1 | i);
        }
      }
    };
    dfs(dfs, root, bit_length, 0);
    return res;
  }
};