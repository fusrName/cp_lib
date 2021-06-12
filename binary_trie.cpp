struct binary_trie {
    using val_type = unsigned int;
    static constexpr int k = 8 * sizeof(val_type);

    struct Node {
        unsigned int count = 0;
        int idx[2] = {-1, -1};
    };
    std::vector<Node> d{{}};

    void add(val_type x) {
        int now = 0;
        d[0].count++;
        for(int i = k - 1; i >= 0; i--) {
            bool bit = x >> i & 1;
            if (d[now].idx[bit] == -1) {
                d[now].idx[bit] = d.size();
                d.emplace_back();
            }
            now = d[now].idx[bit];
            d[now].count++;
        }
    }

    val_type xor_min(val_type x) {
        assert(d[0].count);
        int now = 0;
        val_type z = 0;
        for(int i = k - 1; i >= 0; i--) {
            bool bit = x >> i & 1;
            if (d[now].idx[bit] == -1) bit = !bit;
            z = z << 1 | bit;
            now = d[now].idx[bit];
        }
        return x ^ z;
    }
};