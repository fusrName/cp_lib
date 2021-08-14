#include<atcoder/all>
#include<unordered_map>
using mint = atcoder::modint;

template<class mint>
int BSGS(mint a, mint b) {
    // constraint: a.inv() is defined if a != 0
    // returns x s.t. a ^ x = b
    const int p = mint::mod();
    if (p == 1) return 0;
    if (a == 0) {
        if (b == 0) return 1;
        else if (b == 1) return 0;
        else return -1;
    }
    const int sq_ceil = [p]{
        int l = 0, r = 46340;
        while(r - l > 1) {
            int c = (r + l) >> 1;
            (c * c >= p ? r : l) = c;
        }
        return r;
    }();
    std::unordered_map<int, int> log_a;

    mint aj = 1;
    for(int j = 0; j < sq_ceil; j++, aj *= a) {
        if (!log_a.count(aj.val())) {
            log_a[aj.val()] = j;
        }
    }

    mint a_inv = a.inv();
    mint step = a_inv.pow(sq_ceil);
    mint t = b;
    for(int i = 0; i < p; i += sq_ceil, t *= step) {
        auto it = log_a.find(t.val());
        if (it != log_a.end()) return i + it->second;
    }
    return -1;
}

template<class mint1, class mint2=atcoder::dynamic_modint<-4817491>>
int mod_log(mint1 a, mint1 b) {
    int m = mint1::mod();
    if (m == 1) return 0;
    if (a == 0) {
        if (b == 0) return 1;
        else if (b == 1) return 0;
        else return -1;
    }
    int now = a.val(), b_int = b.val();
    int i = 0;
    int x = 1;
    mint1 acc = 1;
    while(true) {
        if (x == b_int) return i;
        int g = std::gcd(now, m);
        if (g == 1) break;
        if (b_int % g != 0) return -1;
        b_int /= g;
        m /= g;
        acc *= now / g;
        x = (long long)x * acc.val() % m;
        i++;
        now = g;
    }
    mint2::set_mod(m);
    mint2 x_inv = mint2(x).inv();
    int r = BSGS<mint2>(a.val(), b_int * x_inv);
    return r == -1 ? -1 : i + r;
}

long long BSGS_ll(long long a_, long long b_, long long m_) {
    // constraint: gcd(a, m) = 1 (or a == 0)
    // return: x s.t. a ^ x = b
    if (m_ == 1) return 0;
    a_ %= m_;
    if (a_ < 0) a_ += m_;
    b_ %= m_;
    if (b_ < 0) b_ += m_;
    if (a_ == 0) {
        if (b_ == 0) return 1;
        else if (b_ == 1) return 0;
        else return -1;
    }

    unsigned long long a = a_, b = b_, m = m_;

    const unsigned long long sq_ceil = [m]{
        unsigned long long l = 0, r = 1;
        while(r * r < m) l = r, r *= 2;
        while(r - l > 1) {
            unsigned long long c = (r + l) >> 1;
            (c * c >= m ? r : l) = c;
        }
        return r;
    }();

    std::unordered_map<unsigned long long, unsigned int> log_a;
    unsigned long long aj = 1;
    for(unsigned int j = 0; j < sq_ceil; j++, aj = (__int128)aj * a % m) {
        if (!log_a.count(aj)) log_a[aj] = j;
    }

    const unsigned __int128 step = [&]{
        unsigned __int128 x = atcoder::inv_mod(a, m), xk = 1;
        unsigned long long k = sq_ceil;
        while(k) {
            if (k & 1) xk = xk * x % m;
            k >>= 1;
            x = x * x % m;
        }
        return xk;
    }();
    unsigned long long t = b;
    for(unsigned int i = 0; i < sq_ceil; i++, t = t * step % m) {
        auto it = log_a.find(t);
        if (it != log_a.end()) return i * sq_ceil + it->second;
    }
    return -1;
}

long long mod_log_ll(long long a, long long b, long long m) {
    assert(m > 0);
    if (m == 1) return 0;
    a %= m;
    if (a < 0) a += m;
    b %= m;
    if (b < 0) b += m;
    if (a == 0) {
        if (b == 0) return 1;
        else if (b == 1) return 0;
        else return -1;
    }
    long long now = a;
    int i = 0;
    long long x = 1, acc = 1;
    while(true) {
        if (x == b) return i;
        long long g = std::gcd(now, m);
        if (g == 1) break;
        if (b % g != 0) return -1;
        b /= g;
        m /= g;
        acc = (__int128)acc * (now / g) % m;
        x = (__int128)x * acc % m;
        i++;
        now = g;
    }
    long long x_inv = atcoder::inv_mod(x, m);
    b = (__int128)x_inv * b % m;
    long long r = BSGS_ll(a, b, m);
    return r == -1 ? -1 : i + r;
}










// test code

int naive(mint a, mint b) {
    const int p = mint::mod();
    mint now = 1;
    for(int i = 0; i < p; i++, now *= a) {
        if (now == b) return i;
    }
    return -1;
}

#include<bits/stdc++.h>
using namespace std;
int main_() {
    // while(true) {
    //     long long a, b, m;
    //     cin >> a >> b >> m;
    //     mint::set_mod(m);
    //     cout << mod_log_ll(a, b, m) << ' ' << mod_log<mint>(a, b) << endl;
    // }
    // return 0;
    int cnt = 0;
    for(int m = 1; m <= 1000; m++) {
        cout << m << ' ' << cnt << endl;
        // for(int a = 0; a < m; a++) for(int b = 0; b < m; b++) {
        //     if (gcd(a, m) != 1) continue;
        //     mint::set_mod(m);
        //     int r1 = naive(a, b);
        //     int r2 = BSGS(a, b);
        //     int r3 = BSGS_ll(a, b, m);
        //     if (!(r1 == r2 && r2 == r3)) {
        //         cout << a << ' ' << b << ' ' << m << '\n' << r1 << ' ' << r2 << ' ' << r3 << endl;
        //         return 0;
        //     }
        //     cnt++;
        // }
        mint::set_mod(m);
        for(int a = 0; a < m; a++) for(int b = 0; b < m; b++) {
            int r1 = naive(a, b);
            int r2 = mod_log<mint>(a, b);
            if (r1 != r2) {
                cout << a << ' ' << b << ' ' << m << endl;
                cout << r1 << ' ' << r2 << endl;
                return 0;
            }
            cnt++;

        }
    }
    cout << "ok" << endl;
    return 0;
}


#include<bits/stdc++.h>
#include<atcoder/all>
using namespace std;
using namespace atcoder;
#define rep(i,n)for (int i = 0; i < int(n); ++i)
#define rrep(i,n)for (int i = int(n)-1; i >= 0; --i)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
template<class T> void chmax(T& a, const T& b) {a = max(a, b);}
template<class T> void chmin(T& a, const T& b) {a = min(a, b);}
using ll = long long;
using P = pair<int,int>;
using VI = vector<int>;
using VVI = vector<VI>;
using VL = vector<ll>;
using VVL = vector<VL>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int tt;
    cin >> tt;
    while(tt--) {
        int x, y, m;
        cin >> x >> y >> m;
        mint::set_mod(m);
        cout << mod_log<mint, mint>(x, y) << '\n';
    }
    exit(0);
}