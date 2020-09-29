template<class T>
struct Optional {
    bool valid;
    T x;
    operator bool() {return valid;}
    Optional(const T& x): valid(true), x(x) {}
    Optional(): valid(false) {}
    T& value() {return x;}
};

template <class S,
          S (*op)(S, S),
          class F,
          F (*composition)(F, F),
          S (*mapping)(F, S)>
class lazy_lazy_segtree {
    template<class T, T (*f)(T, T)>
    static Optional<T> funcmap(Optional<T> x, Optional<T> y) {
        if (!x) return y;
        if (!y) return x;
        return f(x.value(), y.value());
    }
    template<class T>
    static Optional<T> idfunc() { return {};};
    static Optional<S> Optional_mapping(Optional<F> f, Optional<S> s) {
        if (f && s) {
            return mapping(f.value(), s.value());
        } else {
            return s;
        }
    }
    template<bool (*g)(S)> static bool map_g(Optional<S> x) {return x ? g(x.value()) : true;}
public:
    lazy_segtree<Optional<S>,funcmap<S, op>,idfunc<S>,Optional<F>,Optional_mapping,funcmap<F, composition>,idfunc<F>> seg;
    lazy_lazy_segtree() : lazy_lazy_segtree(0) {}
    lazy_lazy_segtree(int n) : seg(std::vector<Optional<S>>(n)) {}
    lazy_lazy_segtree(const std::vector<S>& v) : seg(std::vector<Optional<S>>(v.begin(),v.end())) {}
    void set(int p, S x) {seg.set(p, x);}
    S get(int p) {
        auto x = seg.get(p);
        return x ? x.value() : S();
    }
    S prod(int l, int r) {
        auto x = seg.prod(l, r);
        return x ? x.value() : S();
    }
    S all_prod() {
        auto x = seg.all_prod();
        return x ? x.value() : S();
    }
    void apply(int p, F f) {seg.apply(p, f);}
    void apply(int l, int r, F f) {seg.apply(l, r, f);}
    template <bool (*g)(S)> int max_right(int l) {return seg.template max_right<map_g<g>>(l);}
    template <class G> int max_right(int l, G g) {return seg.max_right(l, [&g](Optional<S> x){return x ? g(x.value()) : true;});}
    template <bool (*g)(S)> int min_left(int r) {return seg.template min_left<map_g<g>>(r);}
    template <class G> int min_left(int r, G g) {return seg.min_left(r, [&g](Optional<S> x){return x ? g(x.value()) : true;});}
};

struct S {
    int x;
};
S op(S l, S r) {return l.x + r.x;}
struct F {
    int x;
}
F composition(F f, F g) { return f.x + f.y;}
S mapping(F f, S x) { return x.x + f.x;}