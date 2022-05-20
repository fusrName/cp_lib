struct binary_trie {
  const int bit_length = 20;
  using T = unsigned int;

  struct Node {
    int count = 0;
    int ch[2] = {-1, -1};
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

  int root;

  binary_trie(): root(-1) {}
  // TODO: move constructor unnecessary?
  binary_trie(binary_trie&& rhs): root(rhs.root) { rhs.root = -1; }
  binary_trie& operator=(binary_trie&& rhs) {
    swap(root, rhs.root);
    return *this;
  }

  void add(T x) {
    if (root == -1) root = nodes.create();
    int p = root;
    nodes[p].count++;
    for (int k = bit_length - 1; k >= 0; k--) {
      int& nxt = nodes[p].ch[x >> k & 1];
      if (nxt == -1) nxt = nodes.create();
      p = nxt;
      nodes[p].count++;
    }
  }
  bool erase(T x) {
    if (root == -1) return false;
    int idx[bit_length + 1];
    int p = root;
    idx[bit_length] = p;
    for (int k = bit_length - 1; k >= 0; k--) {
      p = nodes[p].ch[x >> k & 1];
      if (p == -1) return false;
      idx[k] = p;
    }
    int k = 0;
    while (k <= bit_lengh && node[idx[k]].count == 1) {
      nodes.release(idx[k]);
      k++;
    }
    if (k == bit_length + 1) root = -1;
    else {
      if (k) nodes[idx[k]].ch[x >> (k - 1) & 1] = -1;
      for (; k <= bit_length; k++) nodes[idx[k]].count--;
    }
  }
  T xor_min(T x) {
    assert(root != -1);
    int p = root;
    T res = 0;
    for (int k = bit_length - 1; k >= 0; k--) {
      const auto& ch = nodes[p].ch;
      bool bit = x >> k & 1;
      if (ch[bit] == -1) bit = !bit;
      p = ch[bit];
      res = res << 1 | bit;
    }
    return x ^ res;
  }
  T xor_max(T x) {
    assert(root != -1);
    int p = root;
    T res = 0;
    for (int k = bit_length - 1; k >= 0; k--) {
      const auto& ch = nodes[p].ch;
      bool bit = (x >> k & 1) ^ 1;
      if (ch[bit] == -1) bit = !bit;
      p = ch[bit];
      res = res << 1 | bit;
    }
    return x ^ res;
  }
  int count(T x) {
    if (root == -1) return 0;
    int p = root;
    for (int k = bit_length - 1; k >= 0; k--) {
      bool bit = x >> k & 1;
      p = nodes[p].ch[bit];
      if (p == -1) return 0;
    }
    return nodes[p].count;
  }
  int size() { return root == -1 ? 0 : nodes[root].count; }
  binary_trie extract_first(int num) {
    assert(0 <= num && num <= size());
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
      int nxt0 = nodes[p_from].ch[0];
      if (nxt0 == -1) {
        if (nodes[p_from].ch[1] == -1) break;
        p_from = nodes[p_from].ch[1];
        p_to = nodes[p_to].ch[1] = nodes.create();
      } else {
        if (size < nodes[nxt0].count) {
          p_from = nxt0;
          p_to = nodes[p_to].ch[0] = nodes.create();
        } else {
          nodes[p_to].ch[0] = nxt0;
          nodes[p_from].ch[0] = -1;
          size -= nodes[nxt0].count;
          if (size == 0) break;
          p_from = nodes[p_from].ch[1];
          p_to = nodes[p_to].ch[1] = nodes.create();
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
        if (nodes[p].ch[i] == -1) {
          nodes[p].ch[i] = nodes[q].ch[i];
          nodes[q].ch[i] = -1;
        } else if (nodes[q].ch[i] != -1) {
          todo.emplace_back(nodes[p].ch[i], nodes[q].ch[i]);
        }
      }
    } while (!todo.empty());
  }
  ~binary_trie() {
    if (root == -1) return;
    static vector<int> todo;
    todo.emplace_back(root);
    do {
      int i = todo.back(); todo.pop_back();
      for (int j : nodes[i].ch) if (j != -1) todo.emplace_back(j);
      nodes.release(i);
    } while (!todo.empty());
  }
  vector<T> get_all() {
    int sz = size();
    if (!sz) return {};
    vector<T> res;
    res.reserve(sz);
    auto dfs = [&](auto&& self, int p, int k, T v) -> void {
      if (k == 0) {
        for (int i = nodes[p].count; i > 0; i--) {
          res.emplace_back(v);
        }
      } else {
        for (int i : {0, 1}) {
          int nxt = nodes[p].ch[i];
          if (nxt != -1) self(self, nxt, k - 1, v << 1 | i);
        }
      }
    };
    dfs(dfs, root, bit_length, 0);
    return res;
  }
};