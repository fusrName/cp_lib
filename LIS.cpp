int LIS(std::vector<int>& x) {
    int n = x.size();
    std::vector<int> d;
    d.reserve(n);
    for(int xi: x) {
        auto it = std::lower_bound(d.begin(), d.end(), xi);
        if (it == d.end()) d.push_back(xi);
        else *it = xi;
    }
    return d.size();
}

int LNDS(std::vector<int>& x) {
    int n = x.size();
    std::vector<int> d;
    d.reserve(n);
    for(int xi: x) {
        auto it = std::upper_bound(d.begin(), d.end(), xi);
        if (it == d.end()) d.push_back(xi);
        else *it = xi;
    }
    return d.size();
}