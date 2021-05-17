// coefficients of nth element of linear recurrence relation
vector<mint> lrr(vector<mint> c, long long n) {
    int k = c.size();
    vector<char> d;
    while(n >= k) {
        d.push_back(n & 1);
        n >>= 1;
    }
    vector<mint> now(k), tmp;
    now[n] = 1;
    while(!d.empty()) {
        tmp.assign(2 * k, 0);
        char x = d.back(); d.pop_back();
        for(int i = 0; i < k; i++) for(int j = 0; j < k; j++) {
            tmp[x + i + j] += now[i] * now[j];
        }
        for(int i = k - 2 + x; i >= 0; i--) {
            mint v = tmp[i + k];
            for(int j = 0; j < k; j++) tmp[i + j] += c[j] * v;
        }
        swap(now, tmp);
    }
    now.resize(k);
    return now;
}