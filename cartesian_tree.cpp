template<class T>
std::vector<int> max_heap_cartesian_tree(const std::vector<T>& a) {
    const unsigned int n = a.size();
    std::vector<int> p(n, -1);
    std::stack<int> s;
    for(unsigned int i = 0; i < n; i++) {
        int last_popped = -1;
        while(!s.empty() && a[s.top()] <= a[i]) {
            last_popped = s.top(); s.pop();
        }
        if (!s.empty()) p[i] = s.top();
        s.push(i);
        if (last_popped != -1) p[last_popped] = i;
    }
    return p;
}


template<class T>
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> max_heap_cartesian_tree(const std::vector<T>& a) {
    const unsigned int n = a.size();
    std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> res;
    auto& p = std::get<0>(res);
    auto& l = std::get<1>(res);
    auto& r = std::get<2>(res);
    p.resize(n, -1);
    l.resize(n, -1);
    r.resize(n, -1);
    std::stack<int> s;
    for(unsigned int i = 0; i < n; i++) {
        int last_popped = -1;
        while(!s.empty() && a[s.top()] <= a[i]) {
            last_popped = s.top(); s.pop();
        }
        if (!s.empty()) {
            r[s.top()] = i;
            p[i] = s.top();
        }
        s.push(i);
        if (last_popped != -1) {
            l[i] = last_popped;
            p[last_popped] = i;
        }
    }
    return res;
}