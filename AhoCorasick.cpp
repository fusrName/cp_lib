struct AhoCorasick {
  static const int csize = 26;
  static const int offset = 'a';
  struct Node {
    int to[csize]{};
    int flink;
    int olink;
    int ends = 0;
    void mark() { ends++; }
  };
  vector<Node> nodes;
  // node 0 : dummy
  // node 1 : root
  AhoCorasick() : nodes(2) {}
  void add(const auto& s) {
    int u = 1;
    for (auto si : s) {
      int c = si - offset;
      if (!nodes[u].to[c]) {
        nodes.emplace_back();
        nodes[u].to[c] = int(nodes.size()) - 1;
      }
      u = nodes[u].to[c];
    }
    nodes[u].mark();
  }
  void build() {
    for (int c = 0; c < csize; c++) nodes[0].to[c] = 1;
    nodes[1].flink = 0;
    nodes[1].olink = 0;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
      const int u = q.front();
      q.pop();
      for (int c = 0; c < csize; c++) {
        int v = nodes[u].to[c];
        if (!v) continue;
        q.emplace(v);
        // u->v
        // compute failure link of v
        int w = nodes[u].flink;
        while(!nodes[w].to[c]) w = nodes[w].flink;
        nodes[v].flink = nodes[w].to[c];
        // compute output link of v
        int f = nodes[v].flink;
        nodes[v].olink = nodes[v].ends ? f : nodes[f].olink;
      }
    }
  }
  auto run(const auto& s) {
    int u = 1;
    for (auto si : s) {
      int c = si - offset;
      int v = nodes[u].to[c];
      while(!v) {
        u = nodes[u].flink;
        v = nodes[u].to[c];
      }
      if (!u) {
        return;
      }
      u = v;
    }
  }
};