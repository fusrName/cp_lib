struct Trie {
    struct Node {
        int next[36];
        Node() {for(int i = 0; i < 36; i++) next[i] = -1;}
        bool data = false;
    };
    std::vector<Node> nodes;
    static constexpr int head = 0;
    Trie(): nodes(1) {}
    bool insert(const string& s, int p = head) {
        int now = p;
        for(char c: s) {
            const int x = '0' <= c && c <= '9' ? c - '0' : 10 + c - 'a';
            if (nodes[now].next[x] == -1) {
                int nxt_idx = nodes.size();
                nodes.resize(nxt_idx + 1);
                nodes[now].next[x] = nxt_idx;
            }
            now = nodes[now].next[x];
        }
        if (nodes[now].data) return false;
        nodes[now].data = true;
        return true;
    }
};