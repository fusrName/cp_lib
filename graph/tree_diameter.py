from collections import deque
def tree_diameter(g):
    n = len(g)
    p = [-1] * n
    q = deque()
    q.append(0)
    while q:
        u = q.popleft()
        for v in g[u]:
            if v != p[u]:
                q.append(v)
                p[v] = u
    q.append(u)
    p[u] = -1
    while q:
        u = q.popleft()
        for v in g[u]:
            if v != p[u]:
                q.append(v)
                p[v] = u
    ret = 0
    while p[u] != -1:
        u = p[u]
        ret += 1
    return ret