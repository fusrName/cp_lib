template<class S>
struct value_compression : vector<S> {
    value_compression(int rsv) { this->reserve(rsv); }
    template<class T>
    void add(const T& v) {
        if constexpr (is_arithmetic_v<T>) this->push_back(v);
        else for(auto&& x: v) this->push_back(x);
    }
    void build() {
        sort(this->begin(), this->end());
        this->erase(unique(this->begin(), this->end()), this->end());
    }
    template<class T>
    int operator()(T x) { return lower_bound(this->begin(), this->end(), x) - this->begin(); }
    template<class T>
    void convert_iter(T& v) { for(auto& x: v) x = (*this)(x); }
};



// // example
// #include<bits/stdc++.h>
// using namespace std;
// int main() {
//     int n;
//     cin >> n;
//     vector<int> a(n);
//     for(int i = 0; i < n; i++) cin >> a[i];
//     value_compression<int> d2i(n + 2);
//     d2i.add(a);
//     d2i.add(-1);
//     d2i.add(1001001001);
//     d2i.build();
//     for(int x: a) cout << d2i(x) << ' ';
//     d2i.convert_iter(a);
//     for(int x: a) cout << x << ' ';
//     cout << d2i.size() << endl;
// }