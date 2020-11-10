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
    const int n;
    static constexpr int psz4q = height == 0 ? update_query_num : update_query_num * (1 + 2 * (height + height - 1));
    //Node node_pool[update_query_num * (1 + 2 * (height + height - 1))];
    Node *const node_pool;
    int last_node = -1;
    Node nil;
    Node *root[1 + update_query_num];
    int last_root = 0;
    //persistent_lazy_segtree(int n): n(n), node_pool(new Node[psz4q]) {
    persistent_lazy_segtree(int n): n(n), node_pool(static_cast<Node*>(operator new(psz4q * sizeof(Node)))) {
        assert(1 <= n && n <= (1 << height));
        nil.lch = nil.rch = &nil;
        nil.d = e();
        nil.lz = id();
        root[0] = &nil;
    }
    //persistent_lazy_segtree(const std::vector<S>& v): n(v.size()), node_pool(new Node[psz4q + n]) {
    persistent_lazy_segtree(const std::vector<S>& v): n(v.size()), node_pool(static_cast<Node*>(operator new((psz4q + n) * sizeof(Node)))) {
        assert(1 <= n && n <= (1 << height));
        Node *now = &node_pool[++last_node];
        root[0] = now;
        struct Path {
            Node *nd;
            int l, r;
            bool took_left;
        };
        Path path[height];
        int idx = -1;
        int l = 0, r = n;
        while(true) {
            while(r - l > 1) {
                path[++idx] = {now, l, r, true};
                int m = (l + r) >> 1;
                r = m;
                now->lch = &node_pool[++last_node];
                now = now->lch;
            }
            now->d = v[l];
            for(; idx >= 0; --idx) {
                Path& p = path[idx];
                now = p.nd;
                if (p.took_left) {
                    p.took_left = false;
                    r = p.r;
                    l = (p.l + r) >> 1;
                    now->rch = &node_pool[++last_node];
                    now = now->rch;
                    break;
                } else {
                    now->d = op(now->lch->d, now->rch->d);
                    now->lz = id();
                }
            }
            if (idx == -1) return;
        }
    }
    ~persistent_lazy_segtree() {
        //operator delete[] node_pool;
        operator delete(node_pool);
    }
    int set(int p, S x, int rev=-1) {
        Node *const root_orig = rev == -1 ? root[last_root] : root[rev];
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
        if (l + 1 == r) return get(l, rev);
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
    int apply(int p, F f, int rev) {
        Node *const root_orig = rev == -1 ? root[last_root] : root[rev];
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
        now->d = mapping(composition(f, f_acc), now_orig->d);
        for(--idx; idx >= 0; --idx) {
            Node *nd = path[idx];
            nd->d = op(nd->lch->d, nd->rch->d);
        }
        return last_root;
    }
    int apply(const int l, const int r, const F f, int rev) {
        Node *const root_orig = rev == -1 ? root[last_root] : root[rev];
        if (l >= r) {
            node_pool[++last_node] = *root_orig;
            root[++last_root] = &node_pool[last_node];
            return last_root;
        }
        if (l + 1 == r) return apply(l, f, rev);
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
            F ff = composition(f, f_acc);
            now->d = mapping(ff, now_orig->d);
            now->lz = composition(ff, now_orig->lz);
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
                path[++idy] = nd;
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
                F ff = composition(f, ft);
                nd->d = mapping(ff, nd_orig->d);
                if (d - a > 1) nd->lz = composition(ff, nd_orig->lz);
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
                path[++idy] = nd;
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
                F ff = composition(f, ft);
                nd->d = mapping(ff, nd_orig->d);
                if (b - d > 1) nd->lz = composition(ff, nd_orig->lz);
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

    template <bool (*g)(S)> int max_right(int l, int rev=-1) {
        return max_right(l, [](S x) { return g(x); }, rev);
    }
    template <class G> int max_right(int l, G g, int rev=-1) {
        assert(0 <= l && l <= n);
        assert(g(e()));
        if (l == n) return n;
        Node *now = rev == -1 ? root[last_root] : root[rev];
        Node *path[height + 1];
        bool took_left[height];
        F f_acc[height + 1];
        //struct R {
        //    int a, b;
        //};
        //R range[height];
        path[0] = now;
        int idx = 0;
        f_acc[0] = id();
        int a = 0, b = n;
        while(b - a > 1) {
            //range[idx] = {a, b};
            f_acc[idx+1] = composition(f_acc[idx], now->lz);
            int m = (a + b) >> 1;
            if (l < m) {
                b = m;
                took_left[idx] = true;
                now = now->lch;
                path[++idx] = now;
            } else {
                a = m;
                took_left[idx] = false;
                now = now->rch;
                path[++idx] = now;
            }
        }
        S sm = mapping(f_acc[idx], now->d);
        if (g(sm) == false) return l;
        for(--idx; idx >= 0; --idx) {
            if (took_left[idx]) {
                S sm2 = op(sm, mapping(f_acc[idx+1], path[idx]->rch->d));
                if (g(sm2) == true) {
                    sm = sm2;
                } else {
                    int a = 0, b = n;
                    for(int i = 0; i < idx; ++i) {
                        int m = (a + b) >> 1;
                        if (took_left[i]) b = m;
                        else a = m;
                    }
                    Node *now = path[idx]->rch;
                    a = (a + b) >> 1; // adding [a, b) fails
                    F ft = f_acc[idx+1];
                    while(b - a > 1) {
                        ft = composition(ft, now->lz);
                        int m = (a + b) >> 1;
                        S sm2 = op(sm, mapping(ft, now->lch->d));
                        if (g(sm2) == true) {
                            sm = sm2;
                            a = m;
                            now = now->rch;
                        } else {
                            b = m;
                            now = now->lch;
                        }
                    }
                    return a;
                }
            }
        }
        return n;
    }

    template <bool (*g)(S)> int min_left(int r, int rev=-1) {
        return min_left(r, [](S x) { return g(x); }, rev);
    }
    template <class G> int min_left(int r, G g, int rev=-1) {
        assert(0 <= r && r <= n);
        assert(g(e()));
        if (r == 0) return 0;
        Node *now = rev == -1 ? root[last_root] : root[rev];
        Node *path[height + 1];
        bool took_left[height];
        F f_acc[height + 1];
        //struct R {
        //    int a, b;
        //};
        //R range[height];
        path[0] = now;
        int idx = 0;
        f_acc[0] = id();
        int a = 0, b = n;
        while(b - a > 1) {
            //range[idx] = {a, b};
            f_acc[idx+1] = composition(f_acc[idx], now->lz);
            int m = (a + b) >> 1;
            if (r <= m) {
                b = m;
                took_left[idx] = true;
                now = now->lch;
                path[++idx] = now;
            } else {
                a = m;
                took_left[idx] = false;
                now = now->rch;
                path[++idx] = now;
            }
        }
        S sm = mapping(f_acc[idx], now->d);
        if (g(sm) == false) return r;
        for(--idx; idx >= 0; --idx) {
            if (!took_left[idx]) {
                S sm2 = op(sm, mapping(f_acc[idx+1], path[idx]->lch->d));
                if (g(sm2) == true) {
                    sm = sm2;
                } else {
                    int a = 0, b = n;
                    for(int i = 0; i < idx; ++i) {
                        int m = (a + b) >> 1;
                        if (took_left[i]) b = m;
                        else a = m;
                    }
                    Node *now = path[idx]->lch;
                    b = (a + b) >> 1; // adding [a, b) fails
                    F ft = f_acc[idx+1];
                    while(b - a > 1) {
                        ft = composition(ft, now->lz);
                        int m = (a + b) >> 1;
                        S sm2 = op(sm, mapping(ft, now->rch->d));
                        if (g(sm2) == true) {
                            sm = sm2;
                            b = m;
                            now = now->lch;
                        } else {
                            a = m;
                            now = now->rch;
                        }
                    }
                    return b;
                }
            }
        }
        return 0;
    }

    std::vector<S> reconstruct_array(int rev=-1) {
        Node *rt = (rev == -1 ? root[last_root] : root[rev]);
        std::vector<S> ret;
        ret.reserve(n);
        int l = 0, r = n;
        struct node_range {
            Node *x;
            int l, r;
            F f;
        } nr[height + 1];
        nr[0] = {rt, 0, n, id()};
        for(int idx = 0; idx >= 0;) {
            Node *now = nr[idx].x;
            int l = nr[idx].l, r = nr[idx].r;
            F f = nr[idx].f;
            --idx;
            if (r - l == 1) {
                ret.push_back(mapping(f, now->d));
            } else {
                int m = (r + l) >> 1;
                F nf = composition(f, now->lz);
                nr[++idx] = {now->rch, m, r, nf};
                nr[++idx] = {now->lch, l, m, nf};
            }
        }
        return ret;
    }
};