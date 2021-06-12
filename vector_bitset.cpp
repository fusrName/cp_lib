struct vector_bitset {
    std::vector<unsigned long long> d;
    const unsigned int n;
    vector_bitset(unsigned int n=0): d((n + 63) >> 6), n(n) {}
    bool operator[](unsigned int i) {
        assert(i < n);
        return d[i >> 6] >> (i & 63) & 1;
    }
    void set(unsigned int i) {
        assert(i < n);
        d[i >> 6] |= 1ULL << (i & 63);
    }
    void reset(unsigned int i) {
        assert(i < n);
        d[i >> 6] &= ~(1ULL << (i & 63));
    }
    vector_bitset& operator|=(const vector_bitset& rhs) {
        int sz = std::min(d.size(), rhs.d.size());
        for(int i = 0; i < sz; i++) d[i] |= rhs.d[i];
        return *this;
    }
};