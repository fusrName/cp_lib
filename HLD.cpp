struct HLD {
    std::vector<std::vector<int>> &to;
    int root, n;
    std::vector<int> sz, parent, depth, idx, ridx, head;
    HLD(std::vector<std::vector<int>>& to, int root=0):
        to(to), root(root), n(to.size()),
        sz(n), parent(n), depth(n), idx(n), ridx(n), head(n)
    {
        init_tree_data(root);
        int x = 0;
        assign_idx(root, root, x);
    }
    void init_tree_data(int u, int p=-1, int d=0) {
        parent[u] = p;
        depth[u] = d;
        int s = 1;
        for(int v: to[u]) {
            if (v == p) continue;
            init_tree_data(v, u, d+1);
            s += sz[v];
        }
        sz[u] = s;
    }
    void assign_idx(int u, int h, int &nxt, int p=-1) {
        head[u] = h;
        idx[u] = nxt++;
        if (sz[u] == 1) {
            ridx[u] = nxt;
            return;
        }
        int mxsize = 0;
        int mi;
        for(int v: to[u]) {
            if (v == p) continue;
            if (sz[v] > mxsize) {
                mxsize = sz[v];
                mi = v;
            }
        }
        assign_idx(mi, h, nxt, u);
        for(int v: to[u]) {
            if (v == p || v == mi) continue;
            assign_idx(v, v, nxt, u);
        }
        ridx[u] = nxt;
    }
};

// path query snippet
/*

    while(d.head[x] != d.head[y]) {
        int xh = d.head[x], yh = d.head[y];
        if (d.depth[xh] > d.depth[yh]) {
            int xi = d.idx[x], xhi = d.idx[xh];
            //ans += s2.sum(xhi, xi + 1);
            x = d.parent[xh];
        } else {
            int yi = d.idx[y], yhi = d.idx[yh];
            //ans += s1.sum(yhi, yi + 1);
            y = d.parent[yh];
        }
    }
    int xi = d.idx[x], yi = d.idx[y];
    if (d.depth[x] > d.depth[y]) {
        // ans += s2.sum(yi + 1, xi + 1);
    } else {
        // ans += s1.sum(xi + 1, yi + 1);
    }

*/