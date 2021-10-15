pair<double, pair<int, int>> minimum_distance2(vector<pair<double, double>> p) {
    const int n = p.size();
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) { return p[i].first < p[j].first; });
    int i_best = -1, j_best = -1;
    double d2_best = numeric_limits<double>::max();
    set<pair<double, int>> y_idx;
    int ptr_l = 0;
    for(int ptr = 0; ptr < n; ptr++) {
        int i = ord[ptr];
        auto [xi, yi] = p[i];
        double d_best = sqrt(d2_best);
        for(; ptr_l < ptr; ptr_l++) {
            int j = ord[ptr_l];
            if (xi - p[j].first < d_best) break;
            y_idx.erase({p[j].second, j});
        }
        auto it_begin = y_idx.upper_bound({yi - d_best, n});
        auto it_end = y_idx.lower_bound({yi + d_best, 0});
        for(auto it = it_begin; it != it_end; it++) {
            int j = it->second;
            double dx = xi - p[j].first;
            double dy = yi - p[j].second;
            double d2 = dx * dx + dy * dy;
            if (d2 < d2_best) d2_best = d2, i_best = j, j_best = i;
        }
        y_idx.emplace(yi, i);
    }
    return {d2_best, {i_best, j_best}};
}