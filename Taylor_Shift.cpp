vector<mint> Taylor_Shift(vector<mint> f, mint c) {
    const int n = f.size();
    for(int k = 0; k < n; k++) f[k] *= Fact[k];
    vector<mint> g(n);
    mint ck = 1;
    for(int k = 0; k < n; k++, ck *= c) {
        g[n - 1 - k] = ck * iFact[k];
    }
    auto h = convolution(f, g);
    for(int k = 0; k < n; k++) h[k] = h[n - 1 + k] * iFact[k];
    h.resize(n);
    return h;
}