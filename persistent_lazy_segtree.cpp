#include<bits/stdc++.h>
template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)(),
          int update_query_num=1000000,
          int height=20>
struct persistent_lazy_segtree {
    struct Node {
        Node *lch, *rch;
        S d;
        F lz;
    };
    Node node_pool[1 + (height + 1) * update_query_num + height * update_query_num];
    int last_node = 0;
    Node *nil = &node_pool[0];
    Node *root[1 + update_query_num];
    int last_root = 0;
    const int n;
    persistent_lazy_segtree(int n): n(n) {
        assert(1 <= n && n <= (1 << height));
        nil->lch = nil->rch = nil;
        nil->d = e();
        root[0] = nil;
    }
    int set(int p, S x, int rev=-1) {
        Node *new_root = &node_pool[++last_node];
        root[++last_root] = new_root;
        Node root_orig = rev == -1 ? root[last_root] : root[rev];
        new_root->lch = root_orig.lch;
        new_root->rch = root_orig.rch;
        F f_acc = root_orig.lz;
        new_root->lz = id();

        Node *now = new_root;
        Node *path[height + 1];
        path[0] = now;
        int idx = 0;
        int l = 0, r = n;
        while(r - l != 1) {
            int m = (l + r) >> 1;
            if (p < m) {
                r = m;
                Node *rnode = &node_pool[++last_node], *r_orig = now->rch;
                now->rch = rnode;
                rnode->lch = r_orig->lch;
                rnode->rch = r_orig->rch;
                rnode->d = mapping(f_acc, r_orig->d);
                rnode->lz = composition(f_acc, r_orig->lz);
                Node *lnode = &node_pool[++last_node], *l_orig = now->lch;
                now->lch = lnode;
                lnode->lch = l_orig->lch;
                lnode->rch = l_orig->rch;
                // d will be updated later
                f_acc = composition(f_acc, l_orig->lz);
                lnode->lz = id();
                now = lnode;
                path[++idx] = now;
            } else {
                l = m;
                Node *lnode = &node_pool[++last_node], *l_orig = now->lch;
                now->lch = lnode;
                lnode->lch = l_orig->lch;
                lnode->rch = l_orig->rch;
                lnode->d = mapping(f_acc, l_orig->d);
                lnode->lz = composition(f_acc, l_orig->lz);
                Node *rnode = &node_pool[++last_node], *r_orig = now->rch;
                now->rch = rnode;
                rnode->lch = r_orig->lch;
                rnode->rch = r_orig->rch;
                // d will be updated later
                f_acc = composition(f_acc, r_orig->lz);
                rnode->lz = id();
                now = rnode;
                path[++idx] = now;
            }
        }
        now->d = x;
        for(--idx; idx >= 0; --idx) {
            Node *nd = path[idx];
            nd->d = op(nd->lch->d, nd->rch->d);
        }
        return last_root;
    }
    S get(int p, int rev=-1) {
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        F f_acc = id();
        int l = 0, r = n;
        while(r - l != 1) {
            f_acc = composition(f_acc, now->lz);
            int m = (r + l) >> 1;
            if (p < m) {
                r = m;
                now = now->lch;
            } else {
                l = m;
                now = now->rch;
            }
        }
        return mapping(f_acc, now->d);
    }
    S prod(int l, int r, int rev=-1) {
        if (l >= r) return e();
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        int a = 0, b = n;
        F f_acc = id();
        while(b - a > 1) {
            f_acc = composition(f_acc, now->lz);
            int m = (l + r) >> 1;
            if (r <= m) {
                b = m;
                now = now->lch;
            } else if (m <= l) {
                a = m;
                now = now->rch;
            } else break;
        }
        if (b - a == 1) return mapping(f_acc, now->d);
        f_acc = composition(f_acc, now->lz);
        S sml = e(), smr = e();
        int c = (a + b) >> 1;
        {
            Node *nd = now->lch;
            F f = f_acc;
            int d = c; // [a, d)
            while(a != l) {
                f = composition(f, nd->lz);
                int m = (a + d) >> 1;
                if (l < m) {
                    d = m;
                    sml = op(mapping(f, nd->rch->d), sml);
                    nd = nd->lch;
                } else {
                    a = m;
                    nd = nd->rch;
                }
            }
            sml = op(mapping(f, nd->d), sml);
        }
        {
            Node *nd = now->rch;
            F f = f_acc;
            int d = c; // [d, b)
            while(b != r) {
                f = composition(f, nd->lz);
                int m = (d + b) >> 1;
                if (m <= r) {
                    d = m;
                    smr = op(smr, mapping(f, nd->lch->d));
                    nd = nd->rch;
                } else {
                    b = m;
                    nd = nd->lch;
                }
            }
            smr = op(smr, mapping(f, nd->d));
        }
        return op(sml, smr);
    }
    S all_prod(int rev=-1) {
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        return now->d;
    }
/*
    std::vector<S> reconstruct_array(int rev=-1) {
        Node *rt = (rev == -1 ? root[last_root] : root[rev]);
        std::vector<S> ret;
        ret.reserve(n);
        int l = 0, r = n;
        struct node_range {
            Node *x;
            int l, r;
        } nr[height];
        nr[0] = {rt, 0, n};
        for(int idx = 0; idx >= 0;) {
            Node *now = nr[idx].x;
            int l = nr[idx].l, r = nr[idx].r;
            --idx;
            if (r - l == 1) {
                ret.push_back(now->d);
            } else {
                int m = (r + l) / 2;
                nr[++idx] = {now->rch, m, r};
                nr[++idx] = {now->lch, l, m};
            }
        }
        return ret;
    }
*/
};