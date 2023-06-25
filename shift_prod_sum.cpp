struct ShiftProdSum {
  int ha, wa, hb, wb, w;
  vector<mint> d;
  ShiftProdSum(const vector<vector<mint>>& a, const vector<vector<mint>>& b) {
    ha = a.size();
    wa = ha > 0 ? a[0].size() : 0;
    hb = b.size();
    wb = hb > 0 ? b[0].size() : 0;
    w = wa + wb - 1;
    if (wa == 0 || wb == 0) return;
    vector<mint> f(w * (ha - 1) + wa), g(w * (hb - 1) + wb);
    for (int i = 0; i < ha; i++) {
      for (int j = 0; j < wa; j++) {
        f[i * w + j] = a[i][j];
      }
    }
    for (int i = 0; i < hb; i++) {
      for (int j = 0; j < wb; j++) {
        g[(hb - 1 - i) * w + (wb - 1 - j)] = b[i][j];
      }
    }
    d = convolution(f, g);
  }
  mint get(int i, int j) {
    if (max(0, i) >= min(ha, i + hb) || max(0, j) >= min(wa, j + wb)) {
      return 0;
    }
    return d[(i + hb - 1) * w + j + wb - 1];
  }
};
