constexpr int FACT_SIZE = 1000000;
mint Fact[FACT_SIZE + 1];
mint iFact[FACT_SIZE + 1];
const auto fact_init = [] {
    Fact[0] = mint::raw(1);
    for(int i = 1; i <= FACT_SIZE; ++i) {
        Fact[i] = Fact[i-1] * i;
    }
    iFact[FACT_SIZE] = Fact[FACT_SIZE].inv();
    for(int i = FACT_SIZE; i; --i) {
        iFact[i-1] = iFact[i] * i;
    }
    return false;
}();

mint comb(int n, int k) {
    if (k == 0) return mint::raw(1);
    assert(n >= 0);
    if (k < 0) return mint::raw(0);
    return Fact[n] * iFact[n - k] * iFact[k];
}

mint fact(int n) {return Fact[n];}
mint perm(int n, int k) {
    assert(0 <= n);
    return Fact[n] * iFact[n - k];
}