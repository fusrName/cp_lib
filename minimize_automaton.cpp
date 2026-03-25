#include<bits/stdc++.h>
namespace {
using namespace std;
// https://github.com/atcoder/ac-library/
#define private public
#include <atcoder/dsu>
#undef private
using namespace atcoder;
#define rep(i,n) for(int i = 0; i < (int)(n); i++)
#define rrep(i,n) for(int i = (int)(n) - 1; i >= 0; i--)
#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
template<class T> bool chmax(T& a, const T& b) { if (a < b) { a = b; return true; } else return false; }
template<class T> bool chmin(T& a, const T& b) { if (b < a) { a = b; return true; } else return false; }
using ll = long long;
using P = pair<int,int>;
using VI = vector<int>;
using VVI = vector<VI>;
using VL = vector<ll>;
using VVL = vector<VL>;

} int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m, sc, tc;
  cin >> n >> m >> sc >> tc;
  VI starts(sc), goals(tc);
  for (int& x : starts) cin >> x;
  for (int& x : goals) cin >> x;
  vector<vector<P>> to(n), to_inv(n);
  rep(_, m) {
    int u, v, l;
    cin >> u >> v >> l;
    to[u].emplace_back(v, l);
    to_inv[v].emplace_back(u, l);
  }
  rep(i, n) ranges::sort(to[i], {}, &P::second);
  dsu uf(n);
  {
    VI v0, v1;
    vector<char> is_goal(n);
    for (int v : goals) is_goal[v] = true;
    rep(i, n) (is_goal[i] ? v1 : v0).emplace_back(i);
    rep(i, ssize(v0) - 1) uf.merge(v0[i], v0[i+1]);
    rep(i, ssize(v1) - 1) uf.merge(v1[i], v1[i+1]);
  }
  for (bool updated = true; updated;) {
    updated = false;
    for (VI& g : uf.groups()) {
      map<vector<P>, VI> mp;
      for (int u : g) {
        vector<P> lst;
        for (auto [v, l] : to[u]) lst.emplace_back(uf.leader(v), l);
        mp[lst].emplace_back(u);
      }
      if (mp.size() != 1) {
        updated = true;
        for (int v : g) uf.parent_or_size[v] = -1;
        for (const auto& [_, vs] : mp) {
          rep(i, ssize(vs) - 1) uf.merge(vs[i], vs[i+1]);
        }
      }
    }
  }

  auto compute_reachablility = [&](const auto& to, const auto& srcs) {
    vector<char> visited(n);
    VI st;
    for (int x : srcs) {
      x = uf.leader(x);
      if (!visited[x]) visited[x] = true, st.emplace_back(x);
    }
    while (st.size()) {
      int u = st.back(); st.pop_back();
      for (auto [v, _] : to[u]) {
        v = uf.leader(v);
        if (!visited[v]) visited[v] = true, st.emplace_back(v);
      }
    }
    return visited;
  };
  auto vis1 = compute_reachablility(to, starts);
  auto vis2 = compute_reachablility(to_inv, goals);
  int nxt_id = 0;
  VI id(n, -1);
  rep(i, n) if (vis1[i] && vis2[i]) id[i] = nxt_id++;
  vector<vector<P>> g_new(nxt_id);
  rep(u, n) if (int i = id[u]; i != -1) {
    for (auto [v, l] : to[u]) if (int j = id[uf.leader(v)]; j != -1) {
      g_new[i].emplace_back(j, l);
    }
  }
  VI starts_new, goals_new;
  rep(_, 2) {
    for (int v : starts) if (int i = id[uf.leader(v)]; i != -1) starts_new.emplace_back(i);
    ranges::sort(starts_new);
    starts_new.erase(unique(all(starts_new)), starts_new.end());
    swap(starts, goals);
    swap(starts_new, goals_new);
  }
  int n_new = nxt_id, m_new = 0;
  rep(i, n_new) m_new += ssize(g_new[i]);
  cout << nxt_id << ' ' << m_new << ' ' << starts_new.size() << ' ' << goals_new.size() << '\n';
  rep(i, ssize(starts_new)) cout << starts_new[i] << " \n"[i + 1 == ssize(starts_new)];
  rep(i, ssize(goals_new)) cout << goals_new[i] << " \n"[i + 1 == ssize(goals_new)];
  rep(i, n_new) for (auto [j, l] : g_new[i]) cout << i << ' ' << j << ' ' << l << '\n';
}