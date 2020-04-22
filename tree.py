def iig(vn, en):
    res = [[] for _ in range(vn)]
    for _ in range(en):
        ai, bi = map(lambda x: int(x) - 1, input().split())
        res[ai].append(bi)
        res[bi].append(ai)
    return res

def sep_pc(graph, root=0):
    parent = [-1 for _ in range(n)]
    children = [None for _ in range(n)]
    def f(x, p):
        parent[x] = p
        children[x] = [y for y in graph[x] if y != p]
        for y in graph[x]:
            if y != p:
                f(y, x)
        return
    f(root, -1)
    return parent, children

def itree(n, root=0):
    return sep_pc(iig(n, n - 1), root)

# parent, children = itree(n)

def get_tsize(children, root=0):
    res = [0 for _ in children]
    def f(x):
        res[x] = sum((f(y) for y in children[x]), 1)
        return res[x]
    f(root)
    return res

