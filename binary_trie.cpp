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

  binary_trie() : root(-1) {}
  binary_trie(binary_trie&& rhs) : root(rhs.root) { rhs.root = -1; }
  binary_trie& operator=(binary_trie&& rhs) {
    swap(root, rhs.root);
    return *this;
  }

  void add(T x) {
    if (root == -1) root = nodes.create();
    int p = root;
    nodes[p].count++;
    for (int k = bit_length - 1; k >= 0; k--) {
      // reference cannot be used since creating a new node may invalidate it
      bool bit = x >> k & 1;
      int nxt = nodes[p].ch[bit];
      if (nxt == -1) nxt = nodes[p].ch[bit] = nodes.create();
      p = nxt;
      nodes[p].count++;
    }
  }
  int erase(T x, int num = -1) {
    assert(num >= -1);
    if (root == -1) return 0;
    int idx[bit_length + 1];
    int p = root;
    idx[bit_length] = p;
    for (int k = bit_length - 1; k >= 0; k--) {
      p = nodes[p].ch[x >> k & 1];
      if (p == -1) return 0;
      idx[k] = p;
    }
    if (0 <= num && num < nodes[p].count) {
      nodes[p].count -= num;
      return num;
    }
    num = nodes[p].count;
    int k = 0;
    while (k <= bit_length && nodes[idx[k]].count == num) {
      nodes.release(idx[k]);
      k++;
    }
    if (k == bit_length + 1) root = -1;
    else {
      nodes[idx[k]].ch[x >> (k - 1) & 1] = -1;
      for (; k <= bit_length; k++) nodes[idx[k]].count -= num;
    }
    return num;
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
  bool empty() { return root == -1; }
  int size() { return root == -1 ? 0 : nodes[root].count; }
  binary_trie extract_first(int num) {
    int sz = size();
    assert(0 <= num && num <= sz);
    binary_trie res;
    if (num == 0) return res;
    else if (num == sz) {
      swap(root, res.root);
      return res;
    }
    res.root = nodes.create();
    int p_from = root;
    int p_to = res.root;
    nodes[p_from].count -= num;
    nodes[p_to].count = num;
    for (int k = bit_length - 1; k >= 0; k--) {
      // loop invariant: 0 < num && num < nodes[p_from].count + num
      int c0 = nodes[p_from].ch[0];
      if (c0 != -1) {
        int csz = nodes[c0].count;
        if (csz > num) {
          p_from = c0;
          p_to = nodes[p_to].ch[0] = nodes.create();
          nodes[p_from].count -= num;
          nodes[p_to].count = num;
          continue;
        } else {
          nodes[p_from].ch[0] = -1;
          nodes[p_to].ch[0] = c0;
          num -= csz;
          if (num == 0) break;
        }
      }
      p_from = nodes[p_from].ch[1];
      p_to = nodes[p_to].ch[1] = nodes.create();
      nodes[p_from].count -= num;
      nodes[p_to].count = num;
    }
    return res;
  }
  binary_trie extract_last(int num) {
    int sz = size();
    binary_trie res = extract_first(sz - num);
    swap(res, *this);
    return res;
  }
  void merge_in(binary_trie&& bt) {
    static vector<pair<int, int>> todo;
    binary_trie bt_local = move(bt);
    if (bt_local.empty()) return;
    if (empty()) {
      *this = move(bt_local);
      return;
    }
    todo.emplace_back(root, bt_local.root);
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