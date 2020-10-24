template <class S, S (*op)(S, S), S (*e)(), int update_query_num=1000000, int height=20> struct persistent_segtree {
    struct Node {
        Node *lch, *rch;
        S d;
    };
    Node node_pool[1 + (height + 1) * update_query_num];
    int last_node = 0;
    Node *nil = &node_pool[0];
    Node *root[1 + update_query_num] = {nil};
    int last_root = 0;
    const int n;
    persistent_segtree(int n): n(n) {
        assert(n <= (1 << height));
        nil->lch = nil->rch = nil;
        nil->d = e();
    }
    int set(int p, S x, int rev=-1) {
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        Node *path[height + 1] = {now};
        bool took_left[height];
        int idx = 0;
        int l = 0, r = n;
        while(r - l != 1) {
            int m = (l + r) / 2;
            if (p < m) {
                now = now->lch;
                took_left[idx] = true;
                r = m;
            } else {
                now = now->rch;
                took_left[idx] = false;
                l = m;
            }
            path[++idx] = now;
        }
        Node *pre = &node_pool[++last_node];
        pre->d = x;
        for(--idx; idx >= 0; --idx) {
            Node *nd = &node_pool[++last_node];
            if (took_left[idx]) {
                nd->lch = pre;
                nd->rch = path[idx]->rch;
            } else {
                nd->lch = path[idx]->lch;
                nd->rch = pre;
            }
            nd->d = op(nd->lch->d, nd->rch->d);
            pre = nd;
        }
        root[++last_root] = pre;
        return last_root;
    }
    S get(int p, int rev=-1) {
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        int l = 0, r = n;
        while(r - l != 1) {
            int m = (r + l) / 2;
            if (p < m) {
                now = now->lch;
                r = m;
            } else {
                now = now->rch;
                l = m;
            }
        }
        return now->d;
    }
    S prod(int l, int r, int rev=-1) {
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        return prod_sub(l, r, 0, n, now);
    }
    S prod_sub(int l, int r, int lb, int rb, Node *x) {
        if (x == nil) return e();
        if (r <= lb || rb <= l) return e();
        if (l <= lb && rb <= r) return x->d;
        int m = (lb + rb) / 2;
        return op(
            prod_sub(l, r, lb, m, x->lch),
            prod_sub(l, r, m, rb, x->rch)
        );
    }
};