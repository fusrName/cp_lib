std::pair<std::vector<int>,std::vector<int>> primes_sieve(int n) {
    std::pair<std::vector<int>,std::vector<int>> rv;
    std::vector<int>& primes = rv.first;
    std::vector<int>& sieve = rv.second;
    primes.reserve(n / 10);
    sieve.resize(n + 1, 0);
    for(int i = 2; i <= n; ++i) {
        if (sieve[i] == 0) {
            sieve[i] = i;
            primes.push_back(i);
            for(int p: primes) {
                int x = i * p;
                if (x > n) break;
                sieve[x] = p;
            }
        } else {
            int si = sieve[i];
            for(int p: primes) {
                int x = i * p;
                if (x > n) break;
                sieve[x] = p;
                if (p == si) break;
            }
        }
    }
    return rv;
}

std::vector<int> sieve(int n) {
    std::vector<int> min_factor(n + 1);
    std::iota(min_factor.begin(), min_factor.end(), 0);
    for(int i = 4; i <= n; i += 2) min_factor[i] = 2;
    for(int i = 9; i <= n; i += 6) min_factor[i] = 3;
    bool f = false;
    for(int p = 5; p * p <= n; f = !f, p += f ? 2 : 4) {
        if (min_factor[p] == p) {
            long long p2 = (long long)p * p;
            if (p2 <= n) for(int i = p2; i <= n; i += p) {
                if (min_factor[i] == i) min_factor[i] = p;
            }
        }
    }
    return min_factor;
};

std::vector<int> primes(int n) {
    auto sv = sieve(n);
    std::vector<int> ps;
    ps.reserve(n/10);
    for(int p = 2; p <= n; ++p) {
        if (sv[p] == p) ps.push_back(p);
    }
    return ps;
}

std::vector<std::vector<int>> divisors(int n) {
    std::vector<std::vector<int>> divs(n + 1);
    for(int d = 1; d <= n; ++d) {
        for(int i = d; i <= n; i += d) {
            divs[i].push_back(d);
        }
    }
    return divs;
}

std::vector<std::vector<std::pair<int,int>>> pfactors(int n) {
    auto sv = primes_sieve(n).second;
    std::vector<std::vector<std::pair<int,int>>> pfs(n + 1);
    for(int i = 2; i <= n; ++i) {
        int x = i;
        auto& s = pfs[x];
        do {
            int cnt = 0;
            int p = sv[x];
            do {
                x /= p;
                cnt++;
            } while(x % p == 0);
            s.emplace_back(p, cnt);
        } while(x != 1);
    }
    return pfs;
}

// global variable Sieve must be defined
std::vector<std::pair<int,int>> factorize(int n) {
    std::vector<std::pair<int,int>> fs;
    while(n > 1) {
        int p = Sieve[n];
        int cnt = 0;
        do {
            n /= p;
            cnt++;
        } while(n % p == 0);
        fs.emplace_back(p, cnt);
    }
    return fs;
}