std::vector<long long> divisors(long long x) {
    std::vector<long long> res1, res2;
    long long d = 1;
    for(; d * d < x; d++) {
        if (x % d == 0) {
            res1.push_back(d);
            res2.push_back(x / d);
        }
    }
    if (d * d == x) res1.push_back(d);
    int sz = res2.size();
    res1.reserve(res1.size() + sz);
    for(int i = sz - 1; i >= 0; i--) res1.push_back(res2[i]);
    return res1;
}

std::vector<std::pair<long long, int>> factorize(long long x) {
    std::vector<std::pair<long long, int>> ret;
    for(long long p = 2; p * p <= x; p++) {
        int cnt = 0;
        while(x % p == 0) x /= p, cnt++;
        if (cnt) ret.emplace_back(p, cnt);
    }
    if (x > 1) ret.emplace_back(x, 1);
    return ret;
}