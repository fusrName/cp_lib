template<class S, class E>
struct Rerooting {
    const vector<vector<E>>& g;
    const int n;
    const int root;
    vector<vector<S>> dp, sl, sr;

    Rerooting(const vector<vector<E>>& g, int root=0): g(g), n(g.size()), root(root), dp(n), sl(n), sr(n) {
        dfs1(root);
        dfs2(root);
    }

    S dfs1(int u, int p=-1) {
        const int sz = g[u].size();
        dp[u].resize(sz);
        sl[u].resize(sz + 1);
        sr[u].resize(sz + 1);

        S res;
        for(int i = 0; i < sz; i++) {
            const E& e = g[u][i];
            int v = dest(e);
            if (v == p) continue;
            dp[u][i] = dfs1(v, u).apply(e);
            res = res.merge(dp[u][i]);
        }
        return res;
    }

    void dfs2(int u, int p=-1) {
        const int sz = g[u].size();

        {
            S s;
            for(int i = 0; i < sz; i++) {
                s = s.merge(dp[u][i]);
                sl[u][i + 1] = s;
            }
        }
        {
            S s;
            for(int i = sz - 1; i >= 0; i--) {
                s = s.merge(dp[u][i]);
                sr[u][i] = s;
            }
        }

        for(int i = 0; i < sz; i++) {
            int v = dest(g[u][i]);
            if (v == p) continue;
            const int sz_v = g[v].size();
            for(int j = 0; j < sz_v; j++) {
                const E& e = g[v][j];
                int w = dest(e);
                if (w != u) continue;
                dp[v][j] = sl[u][i].merge(sr[u][i + 1]).apply(e);
                break;
            }
            dfs2(v, u);
        }
    }

    S get_acc(int v) { return sr[v][0]; }
    S get_res(int v, E e) { return sr[v][0].apply(e); }

    private:
    int dest(const E& e) {
        if constexpr (is_same<E, int>::value) return e;
        else return e.to;
    };
};

// example: below is for ABC222-F
VI d;
struct E { int to, c; };
struct S {
    ll c = 0;
    S apply(E e) const { return S{ e.c + max<ll>(c, d[e.to]) }; }
    S merge(const S& rhs) const { return S{ max(c, rhs.c) }; }
};