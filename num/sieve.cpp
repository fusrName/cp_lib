std::pair<std::vector<int>,std::vector<int>> primes_lpf(const int n) {
    std::pair<std::vector<int>,std::vector<int>> rv;
    std::vector<int>& primes = rv.first;
    std::vector<int>& lpf = rv.second;
    primes.reserve(n / 10);
    lpf.resize(n + 1);
    for(int i = 2; i <= n; i += 2) lpf[i] = 2;
    for(int i = 3; i <= n; i += 6) lpf[i] = 3;
    if (2 <= n) primes.push_back(2);
    if (3 <= n) primes.push_back(3);
    // 5 * x <= n, x <= floor(n / 5)
    const int n5 = n / 5;
    int x = 5;
    bool one_mod6 = false;
    // x_1 <= n5, x_2 = x_1 + 2 <= n5 + 2 <= n
    for(; x <= n5; one_mod6 = !one_mod6, x += (one_mod6 ? 2 : 4)) {
        if (lpf[x] == 0) {
            lpf[x] = x;
            primes.push_back(x);
        }
        int px = lpf[x];
        for(int i = 2;; ++i) {
            int q = primes[i];
            int y = q * x;
            if (y > n) break;
            lpf[y] = q;
            if (q == px) break;
        }
    }
    for(; x <= n; one_mod6 = !one_mod6, x += (one_mod6 ? 2 : 4)) {
        if (lpf[x] == 0) {
            lpf[x] = x;
            primes.push_back(x);
        }
    }
    return rv;
}

auto [primes, lpf] = primes_lpf(1000000);

// global variable lpf must be defined
std::vector<std::pair<int,int>> factorize(int n) {
    std::vector<std::pair<int,int>> fs;
    while(n > 1) {
        int p = lpf[n];
        int cnt = 0;
        do {n /= p; cnt++;} while(n % p == 0);
        fs.emplace_back(p, cnt);
    }
    return fs;
}