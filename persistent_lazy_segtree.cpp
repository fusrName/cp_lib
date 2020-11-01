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
    Node node_pool[1 + update_query_num * (1 + 2 * (height + height - 1))];
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
        const Node *root_orig = rev == -1 ? root[last_root] : root[rev];
        Node *new_root = &node_pool[++last_node];
        root[++last_root] = new_root;

        F f_acc = id();
        Node *now = new_root;
        Node *now_orig = root_orig;
        Node *path[height + 1];
        path[0] = now;
        int idx = 0;
        int l = 0, r = n;
        while(r - l != 1) {
            f_acc = composition(f_acc, now_orig->lz);
            now->lz = id();
            int m = (l + r) >> 1;
            if (p < m) {
                r = m;
                Node *rnode = &node_pool[++last_node], *r_orig = now_orig->rch;
                rnode->lch = r_orig->lch;
                rnode->rch = r_orig->rch;
                rnode->d = mapping(f_acc, r_orig->d);
                rnode->lz = composition(f_acc, r_orig->lz);
                now->rch = rnode;
                Node *lnode = &node_pool[++last_node], *l_orig = now_orig->lch;
                now->lch = lnode;
                now = lnode;
                now_orig = l_orig;
                path[++idx] = now;
            } else {
                l = m;
                Node *lnode = &node_pool[++last_node], *l_orig = now_orig->lch;
                lnode->lch = l_orig->lch;
                lnode->rch = l_orig->rch;
                lnode->d = mapping(f_acc, l_orig->d);
                lnode->lz = composition(f_acc, l_orig->lz);
                now->lch = lnode;
                Node *rnode = &node_pool[++last_node], *r_orig = now_orig->rch;
                now->rch = rnode;
                now = rnode;
                now_orig = r_orig;
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
    S prod(const int l, const int r, int rev=-1) {
        if (l >= r) return e();
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        int a = 0, b = n;
        F f_acc = id();
        while(true) {
            int m = (a + b) >> 1;
            if (r <= m) {
                b = m;
                f_acc = composition(f_acc, now->lz);
                now = now->lch;
            } else if (m <= l) {
                a = m;
                f_acc = composition(f_acc, now->lz);
                now = now->rch;
            } else break;
        }
        if (a == l && b == r) return mapping(f_acc, now->d);
        f_acc = composition(f_acc, now->lz);
        S sm = e();
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
                    sm = op(mapping(f, nd->rch->d), sm);
                    nd = nd->lch;
                } else {
                    a = m;
                    nd = nd->rch;
                }
            }
            sm = op(mapping(f, nd->d), sm);
        }
        {
            Node *nd = now->rch;
            F f = f_acc;
            int d = c; // [d, b)
            while(b != r) {
                f = composition(f, nd->lz);
                int m = (d + b) >> 1;
                if (m < r) {
                    d = m;
                    sm = op(sm, mapping(f, nd->lch->d));
                    nd = nd->rch;
                } else {
                    b = m;
                    nd = nd->lch;
                }
            }
            sm = op(sm, mapping(f, nd->d));
        }
        return sm;
    }
    S all_prod(int rev=-1) {
        Node *now = (rev == -1 ? root[last_root] : root[rev]);
        return now->d;
    }
    int apply(int p, F f, int rev=-1) {
        const Node *root_orig = rev == -1 ? root[last_root] : root[rev];
        Node *new_root = &node_pool[++last_node];
        root[++last_root] = new_root;

        F f_acc = id();
        Node *now = new_root;
        Node *now_orig = root_orig;
        Node *path[height + 1];
        path[0] = now;
        int idx = 0;
        int l = 0, r = n;
        while(r - l != 1) {
            f_acc = composition(f_acc, now_orig->lz);
            now->lz = id();
            int m = (l + r) >> 1;
            if (p < m) {
                r = m;
                Node *rnode = &node_pool[++last_node], *r_orig = now_orig->rch;
                rnode->lch = r_orig->lch;
                rnode->rch = r_orig->rch;
                rnode->d = mapping(f_acc, r_orig->d);
                rnode->lz = composition(f_acc, r_orig->lz);
                now->rch = rnode;
                Node *lnode = &node_pool[++last_node], *l_orig = now_orig->lch;
                now->lch = lnode;
                now = lnode;
                now_orig = l_orig;
                path[++idx] = now;
            } else {
                l = m;
                Node *lnode = &node_pool[++last_node], *l_orig = now_orig->lch;
                lnode->lch = l_orig->lch;
                lnode->rch = l_orig->rch;
                lnode->d = mapping(f_acc, l_orig->d);
                lnode->lz = composition(f_acc, l_orig->lz);
                now->lch = lnode;
                Node *rnode = &node_pool[++last_node], *r_orig = now_orig->rch;
                now->rch = rnode;
                now = rnode;
                now_orig = r_orig;
                path[++idx] = now;
            }
        }
        now->d = mapping(f, now_orig->d);
        for(--idx; idx >= 0; --idx) {
            Node *nd = path[idx];
            nd->d = op(nd->lch->d, nd->rch->d);
        }
        return last_root;
    }
    int apply(const int l, const int r, F f, int rev=-1) {
        const Node *root_orig = rev == -1 ? root[last_root] : root[rev];
        if (l >= r) {
            node_pool[++last_node] = *root_orig;
            root[++last_root] = &node_pool[last_node];
            return last_root;
        }
        Node *new_root = &node_pool[++last_node];
        root[++last_root] = new_root;

        Node *now = new_root;
        Node *now_orig = root_orig;
        Node *path[height + 1];
        path[0] = now;
        int idx = 0;
        int a = 0, b = n;
        F f_acc = id();
        while(true) {
            int m = (a + b) >> 1;
            if (r <= m) {
                b = m;

                f_acc = composition(f_acc, now_orig->lz);
                now->lz = id();

                Node *rnode = &node_pool[++last_node], *r_orig = now_orig->rch;
                rnode->lch = r_orig->lch;
                rnode->rch = r_orig->rch;
                rnode->d = mapping(f_acc, r_orig->d);
                rnode->lz = composition(f_acc, r_orig->lz);
                now->rch = rnode;
                Node *lnode = &node_pool[++last_node], *l_orig = now_orig->lch;
                now->lch = lnode;
                now = lnode;
                now_orig = l_orig;
                path[++idx] = now;

            } else if (m <= l) {
                a = m;

                f_acc = composition(f_acc, now_orig->lz);
                now->lz = id();

                Node *lnode = &node_pool[++last_node], *l_orig = now_orig->lch;
                lnode->lch = l_orig->lch;
                lnode->rch = l_orig->rch;
                lnode->d = mapping(f_acc, l_orig->d);
                lnode->lz = composition(f_acc, l_orig->lz);
                now->lch = lnode;
                Node *rnode = &node_pool[++last_node], *r_orig = now_orig->rch;
                now->rch = rnode;
                now = rnode;
                now_orig = r_orig;
                path[++idx] = now;

            } else break;
        }
        if (a == l && b == r) {
            now->lch = now_orig->lch;
            now->rch = now_orig->rch;
            now->d = mapping(f_acc, now_orig->d);
            now->lz = composition(f_acc, now_orig->lz);
        } else {
            f_acc = composition(f_acc, now_orig->lz);
            now->lz = id();
            
            int c = (a + b) >> 1;
            {
                F ft = f_acc;
                int d = c; // [a, d)
                int idy = idx;
                Node *nd = &node_pool[++last_node], *nd_orig = now_orig->lch;
                now->lch = nd;
                while(a != l) {
                    ft = composition(ft, nd_orig->lz);
                    nd->lz = id();

                    int m = (a + d) >> 1;
                    if (l < m) {
                        d = m;

                        Node *rnode = &node_pool[++last_node], *r_orig = nd_orig->rch;
                        rnode->lch = r_orig->lch;
                        rnode->rch = r_orig->rch;
                        F ff = composition(f, ft);
                        rnode->d = mapping(ff, r_orig->d);
                        rnode->lz = composition(ff, r_orig->lz);
                        nd->rch = rnode;
                        Node *lnode = &node_pool[++last_node], *l_orig = nd_orig->lch;
                        nd->lch = lnode;
                        nd = lnode;
                        nd_orig = l_orig;
                        path[++idy] = nd;

                    } else {
                        a = m;

                        Node *lnode = &node_pool[++last_node], *l_orig = nd_orig->lch;
                        lnode->lch = l_orig->lch;
                        lnode->rch = l_orig->rch;
                        lnode->d = mapping(ft, l_orig->d);
                        lnode->lz = composition(ft, l_orig->lz);
                        nd->lch = lnode;
                        Node *rnode = &node_pool[++last_node], *r_orig = nd_orig->rch;
                        nd->rch = rnode;
                        nd = rnode;
                        nd_orig = r_orig;
                        path[++idy] = nd;

                    }
                }
                nd->lch = nd_orig->lch;
                nd->rch = nd_orig->rch;
                nd->d = mapping(ft, nd_orig->d);
                nd->lz = composition(ft, nd_orig->lz);
                for(--idy; idy > idx; --idy) {
                    Node *nd = path[idy];
                    nd->d = op(nd->lch->d, nd->rch->d);
                }
            }
            {
                F ft = f_acc;
                int d = c; // [d, b)
                int idy = idx;
                Node *nd = &node_pool[++last_node], *nd_orig = now_orig->rch;
                now->rch = nd;
                while(b != r) {
                    ft = composition(ft, nd_orig->lz);
                    nd->lz = id();

                    int m = (d + b) >> 1;
                    if (m < r) {
                        d = m;

                        Node *lnode = &node_pool[++last_node], *l_orig = nd_orig->lch;
                        lnode->lch = l_orig->lch;
                        lnode->rch = l_orig->rch;
                        F ff = composition(f, ft);
                        lnode->d = mapping(ff, l_orig->d);
                        lnode->lz = composition(ff, l_orig->lz);
                        nd->lch = lnode;
                        Node *rnode = &node_pool[++last_node], *r_orig = nd_orig->rch;
                        nd->rch = rnode;
                        nd = rnode;
                        nd_orig = r_orig;
                        path[++idy] = nd;

                    } else {
                        b = m;

                        Node *rnode = &node_pool[++last_node], *r_orig = nd_orig->rch;
                        rnode->lch = r_orig->lch;
                        rnode->rch = r_orig->rch;
                        rnode->d = mapping(ft, r_orig->d);
                        rnode->lz = composition(ft, r_orig->lz);
                        nd->rch = rnode;
                        Node *lnode = &node_pool[++last_node], *l_orig = nd_orig->lch;
                        nd->lch = lnode;
                        nd = lnode;
                        nd_orig = l_orig;
                        path[++idy] = nd;

                    }
                }
                nd->lch = nd_orig->lch;
                nd->rch = nd_orig->rch;
                nd->d = mapping(ft, nd_orig->d);
                nd->lz = composition(ft, nd_orig->lz);
                for(--idy; idy > idx; --idy) {
                    Node *nd = path[idy];
                    nd->d = op(nd->lch->d, nd->rch->d);
                }
            }

            now->d = op(now->lch->d, now->rch->d);
        }
        for(--idx; idx >= 0; --idx) {
            Node *nd = path[idx];
            nd->d = op(nd->lch->d, nd->rch->d);
        }
        return last_root;
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