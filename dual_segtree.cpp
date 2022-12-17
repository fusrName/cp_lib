template <class F, F (*composition)(F, F), F (*id)()> struct dual_segtree {
  public:
    dual_segtree() : dual_segtree(0) {}
    explicit dual_segtree(int n) : dual_segtree(std::vector<F>(n, id())) {}
    explicit dual_segtree(const std::vector<F>& v) : _n(int(v.size())) {
        auto ceil_log2 = [](int n) {
            int x = 0;
            while ((1U << x) < (unsigned int)(n)) x++;
            return x;
        };
        log = ceil_log2(_n);
        size = 1 << log;
        lz = std::vector<F>(2 * size, id());
        for (int i = 0; i < _n; i++) lz[size + i] = v[i];
    }

    void set(int p, F x) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        lz[p] = x;
    }

    F get(int p) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return lz[p];
    }

    void apply(int p, F f) {
        assert(0 <= p && p < _n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        lz[p] = composition(f, lz[p]);
    }
    void apply(int l, int r, F f) {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r) return;

        l += size;
        r += size;

        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        while (l < r) {
            if (l & 1) all_apply(l++, f);
            if (r & 1) all_apply(--r, f);
            l >>= 1;
            r >>= 1;
        }
    }

    void push_all() {
        for (int i = 1; i < size; i++) push(i);
    }
    F get_raw(int p) {
        assert(0 <= p && p < _n);
        return lz[size + p];
    }

  private:
    int _n, size, log;
    std::vector<F> lz;

    void all_apply(int k, F f) {
        lz[k] = composition(f, lz[k]);
    }
    void push(int k) {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = id();
    }
};