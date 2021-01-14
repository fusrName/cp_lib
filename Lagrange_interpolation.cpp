mint interpolate(std::vector<mint>& dataset, int x) {
    int n = dataset.size();
    std::vector<mint> prods_f(n + 1), prods_b(n + 1);
    prods_f[0] = prods_b[n] = mint::raw(1);
    for(int i = 0; i < n; ++i) {
        prods_f[i + 1] = prods_f[i] * (x - i);
    }
    for(int i = n - 1; i >= 0; --i) {
        prods_b[i] = prods_b[i + 1] * (x - i);
    }
    mint ret = mint::raw(0);
    for(int i = 0; i < n; ++i) {
        int rcnt = n - 1 - i;
        if (rcnt & 1) {
            ret -= prods_f[i] * prods_b[i + 1] * iFact[i] * iFact[rcnt] * dataset[i];
        } else {
            ret += prods_f[i] * prods_b[i + 1] * iFact[i] * iFact[rcnt] * dataset[i];
        }
    }
    return ret;
}