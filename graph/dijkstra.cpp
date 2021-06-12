template<class T>
std::vector<unsigned long long> dijkstra(std::vector<std::vector<std::pair<int,T>>>& to, int s=0) {
    const unsigned long long INF = 1002003004005006007;
    struct QueElem {
        int v;
        unsigned long long c;
        bool operator<(const QueElem a) const {return c > a.c;}
        QueElem(int v, unsigned long long c): v(v), c(c) {}
    };
    std::priority_queue<QueElem> q;
    std::vector<unsigned long long> dist(to.size(), INF);
    dist[s] = 0;
    q.emplace(s, 0);
    while(!q.empty()) {
        QueElem qe = q.top(); q.pop();
        int u = qe.v;
        unsigned long long c = qe.c;
        if (c > dist[u]) continue;
        for(auto vc: to[u]) {
            int v = vc.first;
            unsigned long long nc = c + vc.second;
            if (nc < dist[v]) {
                dist[v] = nc;
                q.emplace(v, nc);
            }
        }
    }
    return dist;
}



// returns dists and previous nodes
const unsigned long long INF = 1002003004005006007;
template<class T>
std::pair<std::vector<unsigned long long>, std::vector<int>> dijkstra(std::vector<std::vector<std::pair<int,T>>>& to, int s=0) {
    struct QueElem {
        int v;
        unsigned long long c;
        bool operator<(const QueElem a) const {return c > a.c;}
        QueElem(int v, unsigned long long c): v(v), c(c) {}
    };
    std::pair<std::vector<unsigned long long>, std::vector<int>> res;
    std::vector<unsigned long long>& dist = res.first;
    std::vector<int>& pre = res.second;
    dist.resize(to.size(), INF);
    pre.resize(to.size(), -1);
    std::priority_queue<QueElem> q;
    dist[s] = 0;
    pre[s] = -1;
    q.emplace(s, 0);
    while(!q.empty()) {
        QueElem qe = q.top(); q.pop();
        int u = qe.v;
        unsigned long long c = qe.c;
        if (c > dist[u]) continue;
        for(auto vc: to[u]) {
            int v = vc.first;
            unsigned long long nc = c + vc.second;
            if (nc < dist[v]) {
                dist[v] = nc;
                pre[v] = u;
                q.emplace(v, nc);
            }
        }
    }
    return res;
}