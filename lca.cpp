// depends on RmQ and euler_tour

template<class T, T (*e)()=std::numeric_limits<T>::max>
struct RmQ {
    unsigned int n;
    std::vector<T> a;
    std::vector<std::vector<T>> btable;
    std::vector<unsigned int> ibtable;
    std::vector<int> floor_log2;

    RmQ() {}
    explicit RmQ(const std::vector<T>& a): a(a), n(a.size()), ibtable(n), floor_log2(n + 1) {
        preproc_block_RmQ();
        preproc_in_block_RmQ();
        preproc_floor_log2();
    }

    void preproc_block_RmQ() {
        if (n == 0) return;
        const unsigned int m = (n + 31) >> 5; // block count
        const unsigned int k = ceil_pow2(m);
        btable.resize(k + 1);
        for(int i = 0; i <= k; i++) btable[i].resize(m);
        T now = a[n - 1];
        for(int i = n - 1; i >= 0; i--) {
            if (a[i] < now) now = a[i];
            if ((i & 31) == 0) {
                btable[0][i >> 5] = now;
                if (i) {
                    now = a[--i];
                }
            }
        }
        for(unsigned int i = 0; i < k; i++) {
            unsigned int p1 = 0, p2 = 1 << i;
            for(; p2 < m; p1++, p2++) {
                T x = btable[i][p1], y = btable[i][p2];
                btable[i + 1][p1] = (x < y ? x : y);
            }
            for(; p1 < m; p1++) btable[i + 1][p1] = btable[i][p1];
        }
    }

    void preproc_in_block_RmQ() {
        const unsigned int m = (n + 31) >> 5; // block count
        unsigned int st[32];
        for(unsigned int b = 0; b < m; b++) {
            unsigned int s = b << 5, t = std::min((b + 1) << 5, n);
            int top = -1;
            for(unsigned int i = s; i < t; i++) {
                while(top != -1 && !(a[st[top]] < a[i])) top--;
                unsigned int x = 1U << (i & 31);
                if (top != -1) x |= ibtable[st[top]];
                ibtable[i] = x;
                st[++top] = i;
            }
        }
    }

    void preproc_floor_log2() {
        int lg = 0;
        int now = 1;
        for(int i = 1; i <= n; i++) {
            if (!(now & i)) {
                lg++;
                now <<= 1;
            }
            floor_log2[i] = lg;
        }
    }

    T operator()(int l, int r) {
        if (l >= r) return e();
        r--; // [l, r]
        const unsigned int bl = l >> 5, ibl = l & 31, br = r >> 5, ibr = r & 31;
        if (bl == br) {
            return a[(bl << 5) | __builtin_ctz(ibtable[r] & (~0U << ibl))];
        } else if (bl + 1 == br) {
            return std::min(a[bl << 5 | __builtin_ctz(ibtable[l | 31] & (~0U << ibl))], a[(br << 5) | __builtin_ctz(ibtable[r])]);
        } else {
            // [bl + 1, br)
            int len = br - bl - 1;
            int k = floor_log2[len];
            // int k = 31 - __builtin_clz(len);
            return std::min(
                std::min(btable[k][bl + 1], btable[k][br - (1 << k)]),
                std::min(a[bl << 5 | __builtin_ctz(ibtable[l | 31] & (~0U << ibl))], a[(br << 5) | __builtin_ctz(ibtable[r])])
            );
        }
    }

    // excerpt from internal_bit in ACL
    // @param n `0 <= n`
    // @return minimum non-negative `x` s.t. `n <= 2**x`
    int ceil_pow2(int n) {
        int x = 0;
        while ((1U << x) < (unsigned int)(n)) x++;
        return x;
    }
};


std::vector<int> vertices_on_euler_tour(const std::vector<std::vector<int>>& tree, int root=0) {
    const unsigned int n = tree.size();
    assert(n > 0);
    std::vector<int> res;
    res.reserve(2 * n - 1); // 1 + 2|E|
    std::vector<std::pair<int, int>> s;
    s.reserve(n);
    s.emplace_back(root, 0);
    res.push_back(root);
    while(!s.empty()) {
        auto& [u, i] = s.back();
        if (i == tree[u].size()) {
            s.pop_back();
            if (!s.empty()) res.push_back(s.back().first);
        } else {
            int v = tree[u][i++];
            const unsigned int sz = s.size();
            if (sz >= 2 && v == s[sz - 2].first) continue;
            s.emplace_back(v, 0);
            res.push_back(v);
        }
    }
    return res;
}


struct LCA {
    const int n;
    std::vector<int> depth;
    const std::vector<int> tour;
    std::vector<int> pos;
    struct S {
        int i, d;
        bool operator<(const S x) const {return d < x.d;}
    };
    static S e() {return {0, INT_MAX};}
    RmQ<S, e> rmq;
    LCA(const std::vector<std::vector<int>>& tree, int root=0): n(tree.size()), depth(n, -1), tour(vertices_on_euler_tour(tree)), pos(n) {
        std::vector<int> s{root};
        s.reserve(n);
        depth[root] = 0;
        while(!s.empty()) {
            int u = s.back(); s.pop_back();
            for(int v: tree[u]) {
                if (depth[v] == -1) {
                    depth[v] = depth[u] + 1;
                    s.push_back(v);
                }
            }
        }
        std::vector<S> t(tour.size());
        for(int i = 0; i < tour.size(); i++) {
            int v = tour[i];
            t[i] = {v, depth[v]};
            pos[v] = i;
        }
        rmq = RmQ<S, e>(t);
    }
    int operator()(int u, int v) {
        int pu = pos[u], pv = pos[v];
        if (pu > pv) std::swap(pu, pv);
        return rmq(pu, pv + 1).i;
    }
};


// Verification

// // https://judge.yosupo.jp/problem/lca
// #define rep(i,n)for (int i = 0; i < int(n); ++i)
// using VI = std::vector<int>;
// using VVI = std::vector<VI>;
// 
// int main() {
//     using namespace std;
//     ios::sync_with_stdio(false);
//     cin.tie(0);
//     int n, q;
//     cin >> n >> q;
//     VVI to(n);
//     for(int i = 1; i < n; i++) {
//         int p;
//         cin >> p;
//         to[i].push_back(p);
//         to[p].push_back(i);
//     }
//     LCA lca(to);
//     while(q--) {
//         int u, v;
//         cin >> u >> v;
//         cout << lca(u, v) << '\n';
//     }
// }