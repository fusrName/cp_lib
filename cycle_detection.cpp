// returns (len_of_cycle, first_in-cycle_index)
template<class S, class F>
std::pair<int, int> detect_cycle(F f, S x) {
    S p = x, q = x;
    int l = 0;
    int ub = 1;
    do {
        if (l == ub) {
            ub <<= 1;
            l = 0;
            p = q;
        }
        q = f(q);
        l++;
    } while(p != q);
    p = q = x;
    for(int it = l; it > 0; it--) q = f(q);
    int m = 0;
    while(p != q) p = f(p), q = f(q), m++;
    return {l, m};
}