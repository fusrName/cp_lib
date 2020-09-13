class mf_graph:
    def __init__(self, n=0):
        self._n = n
        self.g = [[] for _ in range(n)]
        self.pos = []
    
    def add_edge(self, frm, to, cap):
        m = len(self.pos)
        e1 = self._edge(to, cap)
        e2 = self._edge(frm, 0)
        e1.rev = e2
        e2.rev = e1
        self.pos.append(e1)
        self.g[frm].append(e1)
        self.g[to].append(e2)
        return m
    
    class edge:
        def __init__(self, frm, to, cap, flow):
            self.frm = frm
            self.to = to
            self.cap = cap
            self.flow = flow
        
        def __iter__(self):
            yield self.frm
            yield self.to
            yield self.cap
            yield self.flow
    
    def get_edge(self, i):
        e1 = self.pos[i]
        e2 = e1.rev
        return self.edge(e2.to, e1.to, e1.cap + e2.cap, e2.cap)
    
    def edges(self):
        return [self.get_edge(i) for i in range(len(self.pos))]
    
    def change_edge(self, i, new_cap, new_flow):
        e = self.pos[i]
        e.cap = new_cap - new_flow
        e.rev.cap = new_flow
    
    def flow(self, s, t, flow_limit=0XFFFFFFFFFFFFFFF):
        g = self.g
        flow = 0
        es = [[] for _ in range(self._n)]
        que = [None] * self._n
        while flow < flow_limit:
            level = [-1] * self._n
            level[s] = 0
            ql = 0
            qr = 1
            que[0] = s
            unreached = s != t
            lvl = 1
            for p in es:
                p.clear()
            while unreached and ql < qr:
                nqr = qr
                while ql < qr:
                    v = que[ql]
                    ql += 1
                    for e in g[v]:
                        to = e.to
                        if e.cap and not 0 <= level[to] < lvl:
                            es[to].append(e.rev)
                            if level[to] < 0:
                                level[to] = lvl
                                if to == t:
                                    unreached = False
                                que[nqr] = to
                                nqr += 1
                qr = nqr
                lvl += 1
            if unreached:
                return flow
            stack = []
            v = t
            up = flow_limit - flow
            res = 0
            while True:
                if v == s or not es[v]:
                    if v == s:
                        res = up
                    while stack:
                        tmp = res
                        e, up, res = stack.pop()
                        e.cap -= tmp
                        e.rev.cap += tmp
                        res += tmp
                        if res < up:
                            v = e.to
                            break
                    else:
                        flow += res
                        break
                else:
                    e = es[v].pop()
                    stack.append((e.rev, up, res))
                    v = e.to
                    up = min(up, e.rev.cap)
                    res = 0
        return flow
    
    def min_cut(self, s):
        visited = [False] * self._n
        que = [None] * self._n
        ql = 0
        qr = 1
        que[0] = s
        visited[s] = True
        while ql < qr:
            p = que[ql]
            ql += 1
            for e in self.g[p]:
                if e.cap and not visited[e.to]:
                    visited[e.to] = True
                    que[qr] = e.to
                    qr += 1
        return visited
    
    class _edge:
        def __init__(self, to, cap):
            self.to = to
            self.cap = cap
