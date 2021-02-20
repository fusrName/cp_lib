// e is only used as retval of empty range query
template<class T, T (*e)()=std::numeric_limits<T>::max>
struct RmQ {
    const unsigned int n;
    const std::vector<T>& a;
    std::vector<std::vector<T>> btable;
    std::vector<unsigned int> ibtable;
    std::vector<int> floor_log2;

    explicit RmQ(const std::vector<T>& a): a(a), n(a.size()), ibtable(n), floor_log2(n + 1) {
        preproc_block_RmQ();
        preproc_in_block_RmQ();
        preproc_floor_log2();
    }

    void preproc_block_RmQ() {
        if (n == 0) return;
        const unsigned int m = (n + 31) >> 5; // block count
        const unsigned int k = ceil_pow2(m);
        btable.resize(k + 1);
        for(int i = 0; i <= k; i++) btable[i].resize(m);
        T now = a[n - 1];
        for(int i = n - 1; i >= 0; i--) {
            if (a[i] < now) now = a[i];
            if ((i & 31) == 0) {
                btable[0][i >> 5] = now;
                if (i) {
                    now = a[--i];
                }
            }
        }
        for(unsigned int i = 0; i < k; i++) {
            unsigned int p1 = 0, p2 = 1 << i;
            for(; p2 < m; p1++, p2++) {
                T x = btable[i][p1], y = btable[i][p2];
                btable[i + 1][p1] = (x < y ? x : y);
            }
            for(; p1 < m; p1++) btable[i + 1][p1] = btable[i][p1];
        }
    }

    void preproc_in_block_RmQ() {
        const unsigned int m = (n + 31) >> 5; // block count
        unsigned int st[32];
        for(unsigned int b = 0; b < m; b++) {
            unsigned int s = b << 5, t = std::min((b + 1) << 5, n);
            int top = -1;
            for(unsigned int i = s; i < t; i++) {
                while(top != -1 && a[st[top]] >= a[i]) top--;
                unsigned int x = 1U << (i & 31);
                if (top != -1) x |= ibtable[st[top]];
                ibtable[i] = x;
                st[++top] = i;
            }
        }
    }

    void preproc_floor_log2() {
        int lg = 0;
        int now = 1;
        for(int i = 1; i <= n; i++) {
            if (!(now & i)) {
                lg++;
                now <<= 1;
            }
            floor_log2[i] = lg;
        }
    }

    T operator()(int l, int r) {
        if (l >= r) return e();
        r--; // [l, r]
        const unsigned int bl = l >> 5, ibl = l & 31, br = r >> 5, ibr = r & 31;
        if (bl == br) {
            return a[(bl << 5) | __builtin_ctz(ibtable[r] & (~0U << ibl))];
        } else if (bl + 1 == br) {
            return std::min(a[bl << 5 | __builtin_ctz(ibtable[l | 31] & (~0U << ibl))], a[(br << 5) | __builtin_ctz(ibtable[r])]);
        } else {
            // [bl + 1, br)
            int len = br - bl - 1;
            int k = floor_log2[len];
            // int k = 31 - __builtin_clz(len);
            return std::min(
                std::min(btable[k][bl + 1], btable[k][br - (1 << k)]),
                std::min(a[bl << 5 | __builtin_ctz(ibtable[l | 31] & (~0U << ibl))], a[(br << 5) | __builtin_ctz(ibtable[r])])
            );
        }
    }

    // excerpt from internal_bit in ACL
    // @param n `0 <= n`
    // @return minimum non-negative `x` s.t. `n <= 2**x`
    int ceil_pow2(int n) {
        int x = 0;
        while ((1U << x) < (unsigned int)(n)) x++;
        return x;
    }
};



// Verification

// // https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/3/DSL_3_D
// int main() {
//     
//     using namespace std;
//     int n, l;
//     scanf("%d%d", &n, &l);
//     vector<int> a(n);
//     for(int i = 0; i < n; i++) scanf("%d", &a[i]);
//     RmQ<int> d(a);
//     for(int i = l; i <= n; i++) printf("%d%c", d(i - l, i), " \n"[i == n]);
// }

// // https://judge.yosupo.jp/problem/staticrmq
// 
// int main() {
//     using namespace std;
//     ios::sync_with_stdio(false);
//     cin.tie(0);
//     int n, q;
//     cin >> n >> q;
//     vector<int> a(n);
//     for(int i = 0; i < n; i++) cin >> a[i];
//     RmQ<int> d(a);
//     while(q--) {
//         int l, r;
//         cin >> l >> r;
//         cout << d(l, r) << '\n';
//     }
// }