template<class E=int>
struct centroid_decomposition {
  const vector<vector<E>>& g0;
  const int n;
  vector<vector<int>> ctree;
  int root;
  // used for temp data in build()
  vector<int> subtree_size, parent, vertices;
  vector<char> valid;

  centroid_decomposition(const vector<vector<E>>& g)
      : g0(g), n(g.size()), ctree(n), root(-1),
        subtree_size(n), parent(n), valid(n, true) {
    assert(n > 0);
    vertices.reserve(n);
    build(0, -1);
  }
  void examine_tree(int u, int p) {
    subtree_size[u] = 1;
    parent[u] = p;
    vertices.emplace_back(u);
    for (const E& e : g0[u]) {
      int v = edge_dest(e);
      if (v == p || !valid[v]) continue;
      examine_tree(v, u);
      subtree_size[u] += subtree_size[v];
    }
  }
  void build(int a, int p_ctree) {
    vertices.clear();
    examine_tree(a, -1);
    // find centroid
    const int tree_size = subtree_size[a];
    const int half_tree_size = tree_size / 2;
    int centroid = -1;
    for (int u : vertices) {
      bool is_centroid = true;
      int p = parent[u];
      for (const E& e : g0[u]) {
        int v = edge_dest(e);
        if (!valid[v]) continue;
        int sz = v != p ? subtree_size[v] : tree_size - subtree_size[u];
        if (sz > half_tree_size) {
          is_centroid = false;
          break;
        }
      }
      if (is_centroid) {
        centroid = u;
        break;
      }
    }
    assert(centroid != -1);
    if (p_ctree == -1) root = centroid;
    else ctree[p_ctree].emplace_back(centroid);

    // next recursion
    valid[centroid] = false;
    for (const E& e : g0[centroid]) {
      int v = edge_dest(e);
      if (!valid[v]) continue;
      build(v, centroid);
    }
  }

 private:
  int edge_dest(const E& e) {
    if constexpr (is_same_v<E, int>) return e;
    else return e.to;;
  }
};
