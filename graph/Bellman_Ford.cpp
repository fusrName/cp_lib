// if dist[u] = INF, u is unreachable
// if dist[u] = -INF, u is inside a reachable negative cycle, or reachable from such a cycle

template<class T>
std::vector<long long> Bellman_Ford(const std::vector<std::vector<std::pair<int,T>>>& to, int s=0) {
    constexpr long long INF = 1002003004005006007;
    const int n = to.size();
    // make a permutation
    std::vector<int> p(n), p_inv(n);
    std::iota(p.begin(), p.end(), 0);
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::shuffle(p.begin(), p.end(), rng);
    for(int i = 0; i < n; i++) p_inv[p[i]] = i;

    // divide edges into two sets
    int ecnt[2] = {};
    for(int u = 0; u < n; u++) for(auto [v, _]: to[u]) {
        ecnt[p[u] > p[v]]++;
    }
    std::vector<std::pair<int, int>> vs[2]; // (vertex, id_e_right)
    std::vector<std::pair<int, T>> es[2];
    for(int k = 0; k < 2; k++) vs[k].reserve(n), es[k].reserve(ecnt[k]);
    int idx0 = 0, idx1 = ecnt[1];
    for(int pu = 0; pu < n; pu++) {
        int u = p_inv[pu];
        int idx0_old = idx0, idx1_old = idx1;
        for(auto [v, c]: to[u]) {
            if (pu <= p[v]) es[0].emplace_back(v, c), idx0++;
            else es[1].emplace_back(v, c), idx1--;
        }
        if (idx0 != idx0_old) vs[0].emplace_back(u, idx0);
        if (idx1 != idx1_old) vs[1].emplace_back(u, idx1_old);
    }
    std::reverse(vs[1].begin(), vs[1].end());
    std::reverse(es[1].begin(), es[1].end());

    std::vector<char> updated[2]; updated[0].resize(n), updated[1].resize(n);
    std::vector<long long> dist(n, INF);
    dist[s] = 0;
    updated[0][s] = updated[1][s] = true;
    const int max_iter = (n + 1) / 2; // forwards by at least two for each iteration, except for the first one; max_len = n - 1 <= 2(max_iter - 1) + 1
    for(int it = max_iter + 1; it > 0; it--) {
        bool any_updated = false;
        for(int k = 0; k < 2; k++) {
            int idx = 0;
            for(auto [u, r]: vs[k]) {
                if (!updated[k][u]) {
                    idx = r;
                    continue;
                }
                updated[k][u] = false;
                long long d = dist[u];
                for(; idx < r; idx++) {
                    auto [v, c] = es[k][idx];
                    long long nd = d + c;
                    if (nd < dist[v]) dist[v] = nd, updated[0][v] = updated[1][v] = any_updated = true;
                }
            }
        }
        if (!any_updated) return dist;
    }
    // contains negative cycle
    std::vector<char> is_inf(n, false);
    std::vector<int> todo;
    for(auto [u, r]: vs[0]) if (updated[0][u]) {
        is_inf[u] = true;
        dist[u] = -INF;
        todo.push_back(u);
    }
    while(!todo.empty()) {
        int u = todo.back(); todo.pop_back();
        for(auto [v, c]: to[u]) if (!is_inf[v]) {
            is_inf[v] = true;
            dist[v] = -INF;
            todo.push_back(v);
        }
    }
    return dist;
}