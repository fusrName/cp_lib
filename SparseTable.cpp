template <class S, S (*op)(S, S), S (*e)()> struct SparseTable {
    int n;
    std::vector<int> floor_lg;
    std::vector<std::vector<S>> d;
    explicit SparseTable(std::vector<S>&& a): n(a.size()), floor_lg(n + 1) {
        if (n == 0) return;
        int frog = 0;
        for(int i = 1; i <= n; frog++) {
            int j = std::min(n + 1, 2 * i);
            for(; i < j; i++) floor_lg[i] = frog;
        }
        d.resize(frog);
        d[0] = std::move(a);
        for(int p = 0, w = 1; p < frog - 1; p++, w *= 2) {
            int last = n - 2 * w;
            d[p + 1].resize(last + 1);
            for(int i = 0; i <= last; i++) d[p + 1][i] = op(d[p][i], d[p][i + w]);
        }
    }
    explicit SparseTable(const std::vector<S>& a): SparseTable(std::vector<S>(a)) {}

    S query(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        if (l >= r) return e();
        int w = r - l;
        int p = floor_lg[w];
        return op(d[p][l], d[p][r - (1 << p)]);
    }
};



// // yosupo judge
// #include<bits/stdc++.h>
// using namespace std;
// int op(int x, int y) { return min(x, y); }
// int e() { return 1001001001; }

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int n, q;
//     cin >> n >> q;
//     vector<int> a(n);
//     for(int i = 0; i < n; i++) cin >> a[i];
//     SparseTable<int, op, e> rmq(move(a));
//     while(q--) {
//         int l, r;
//         cin >> l >> r;
//         cout << rmq.query(l, r) << '\n';
//     }
// }