struct AhoCorasick {
  static const int csize = 26;
  static const int offset = 'a';
  struct Node {
    int to[csize];
    int flink;

    bool ends = false;

    Node() { memset(to, -1, sizeof(to)); }
    void mark() { ends = true; }
  };
  vector<Node> nodes;
  void add(const auto& s) {
    if (nodes.empty()) nodes.emplace_back();
    int u = 0;
    for (auto si : s) {
      int c = si - offset;
      int v = nodes[u].to[c];
      if (v == -1) {
        v = nodes[u].to[c] = nodes.size();
        nodes.emplace_back();
      }
      u = v;
    }
    nodes[u].mark();
  }
  void build() {
    nodes[0].flink = -1;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
      const int u = q.front();
      q.pop();
      for (int c = 0; c < csize; c++) {
        int v = nodes[u].to[c];
        if (v == -1) continue;
        q.emplace(v);
        int f = next_v(nodes[u].flink, c);
        nodes[v].flink = f;
        // problem-specific process
        nodes[v].ends |= nodes[f].ends;
      }
    }
  }
  int next_v(int u, int c) {
    while (u != -1) {
      int v = nodes[u].to[c];
      if (v != -1) return v;
      u = nodes[u].flink;
    }
    return 0;
  }
  int next(int u, auto si) { return next_v(u, si - offset); }
  auto run(const auto& s) {
    int u = 0;
    for (auto si : s) {
      u = next(u, si);
      if (!u) {
        return;
      }
    }
  }
};