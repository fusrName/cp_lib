import heapq
class mcf_graph:
    def __init__(self, n=0):
        self._n = n
        self.g = [[] for _ in range(n)]
        self.pos = []
    
    def add_edge(self, frm, to, cap, cost):
        m = len(self.pos)
        e1 = self._edge(to, cap, cost)
        e2 = self._edge(frm, 0, -cost)
        e1.rev = e2
        e2.rev = e1
        self.pos.append(e1)
        self.g[frm].append(e1)
        self.g[to].append(e2)
        return m
    
    class edge:
        def __init__(self, frm, to, cap, flow, cost):
            self.frm = frm
            self.to = to
            self.cap = cap
            self.flow = flow
            self.cost = cost
        
        def __iter__(self):
            yield self.frm
            yield self.to
            yield self.cap
            yield self.flow
            yield self.cost
    
    def get_edge(self, i):
        e = self.pos[i]
        re = e.rev
        return self.edge(re.to, e.to, e.cap + re.cap, re.cap, e.cost)
    
    def edges(self):
        return [self.get_edge(i) for i in range(len(self.pos))]
    
    def flow(self, s, t, flow_limit=float('inf')):
        return self.slope(s, t, flow_limit)[-1]
    
    def slope(self, s, t, flow_limit=float('inf')):
        pop, push = heapq.heappop, heapq.heappush
        n = self._n
        g = self.g
        dual = [0] * n
        dist = [0] * n
        pv = [0] * n
        pe = [None] * n
        vis = [False] * n
        INF = float('inf')    
        flow = 0
        cost = 0
        prev_cost = -1
        result = [(flow, cost)]
        while flow < flow_limit:
            for i in range(n):
                dist[i] = INF
                vis[i] = False
            dist[s] = 0
            que = [(0, s)]
            while que:
                _, v = pop(que)
                if vis[v]:
                    continue
                vis[v] = True
                if v == t:
                    break
                for e in g[v]:
                    w = e.to
                    if vis[w] or not e.cap:
                        continue
                    ndist = dist[v] + e.cost - dual[w] + dual[v]
                    if ndist < dist[w]:
                        dist[w] = ndist
                        pv[w] = v
                        pe[w] = e
                        push(que, (dist[w], w))
            else:
                break
            for v in range(n):
                if not vis[v]:
                    continue
                dual[v] -= dist[t] - dist[v]
            c = flow_limit - flow
            v = t
            while v != s:
                c = min(c, pe[v].cap)
                v = pv[v]
            v = t
            while v != s:
                e = pe[v]
                e.cap -= c
                e.rev.cap += c
                v = pv[v]
            d = -dual[s]
            flow += c
            cost += c * d
            if prev_cost == d:
                result[-1] = (flow, cost)
            else:
                result.append((flow, cost))
            prev_cost = d
        return result
    
    class _edge:
        def __init__(self, to, cap, cost):
            self.to = to
            self.cap = cap
            self.cost = cost
    
    class _edge:
        def __init__(self, to, cap, cost):
            self.to = to
            self.cap = cap
            self.cost = cost
