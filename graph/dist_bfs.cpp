vector<int> bfs(const vector<vector<int>>& g, int s=0) {
  const int n = g.size();
  vector<int> dist(n, -1);
  vector<int> q(n);
  q[s] = 0;
  dist[s] = 0;
  int l = 0, r = 1;
  while(l < r) {
    int u = q[l++];
    int nd = dist[u] + 1;
    for(int v: g[u]) if (dist[v] == -1) {
      dist[v] = nd;
      q[r++] = v;
    }
  }
  return dist;
}
