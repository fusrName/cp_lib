def scc_dag(scc, g):
    n = len(g)
    d = [-1] * n
    for i, c in enumerate(scc):
        for v in c:
            d[v] = i
    m = len(scc)
    to = [[] for _ in range(m)]
    added = [-1] * m
    for i, c in enumerate(scc):
        added[i] = i
        for u in c:
            for v in (d[x] for x in g[u]):
                if added[v] != i:
                    added[v] = i
                    to[i].append(v)
    return to