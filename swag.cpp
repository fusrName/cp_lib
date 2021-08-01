template <class S, S (*op)(S, S), S (*e)()>
struct SWAG {
    std::vector<S> ar, sl, sr;
    SWAG() : sl{e()}, sr{e()} {}
    explicit SWAG(const std::vector<S>& v) : sr{e()} {
        const int n = v.size();
        S x = e();
        sl.reserve(n + 1);
        sl.push_back(x);
        for (int i = n - 1; i >= 0; i--) {
            x = op(v[i], x);
            sl.push_back(x);
        }
    }

    void push(S x) {
        ar.push_back(x);
        sr.push_back(op(sr.back(), x));
    }

    void pop() {
        assert(sl.size() != 1 || ar.size() != 0);
        if (sl.size() != 1) {
            sl.pop_back();
        } else {
            const int n = ar.size();
            S x = e();
            for (int i = n - 1; i >= 1; i--) {
                x = op(ar[i], x);
                sl.push_back(x);
            }
            ar.resize(0);
            sr.resize(1);
        }
    }

    S all_prod() { return op(sl.back(), sr.back()); }

    int size() { return (int)sl.size() - 1 + ar.size(); }
};