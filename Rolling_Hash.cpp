#include<random>
#include<chrono>
constexpr unsigned long long RH_MOD = (1ULL << 61) - 1;
std::default_random_engine RNG(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<unsigned long long> Distribution(1, RH_MOD - 1);
unsigned long long B = Distribution(RNG);

unsigned long long rhmul_no_mod(unsigned long long x, unsigned long long y) {
    __uint128_t z = __uint128_t(x) * y;
    return (z >> 61) + (z & RH_MOD);
}
unsigned long long rhmod(unsigned long long x) {
    unsigned long long y = (x >> 61) + (x & RH_MOD);
    return y >= RH_MOD ? y - RH_MOD : y;
}
unsigned long long rhmul(unsigned long long x, unsigned long long y) {
    return rhmod(rhmul_no_mod(x, y));
}

struct Rolling_Hash {
    using ull = unsigned long long;
    int n;
    std::vector<ull> hash;
    std::vector<ull> powB;
    template<class T> Rolling_Hash(const T& s): n(s.size()), hash(n + 1), powB(n + 1) {
        compute_hash_lazy_mod(s);
        compute_powB();
    }
    template<class T> void compute_hash_lazy_mod(const T& s) {
        ull cur_hash = 0;
        for(int i = 0; i < n; i++) {
            cur_hash = rhmod(rhmul_no_mod(cur_hash, B) + s[i]);
            hash[i + 1] = cur_hash;
        }
    }
    void compute_powB() {
        ull bi = 1;
        for(int i = 0; i <= n; i++) powB[i] = bi, bi = rhmul(bi, B);
    }
    ull operator()(int i, int j) {
        assert(0 <= i && i <= j && j <= n);
        return rhmod(hash[j] + 4 * RH_MOD - rhmul_no_mod(hash[i], powB[j - i]));
    }
};