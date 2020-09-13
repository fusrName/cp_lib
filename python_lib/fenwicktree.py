class fenwick_tree:
    def __init__(self, n=0, T=int):
        self._n = n
        self.data = [T() for _ in range(n)]
        self.T = T
    
    def add(self, p, x):
        p += 1
        while p <= self._n:
            self.data[p - 1] += x
            p += p & -p
    
    def sum(self, l, r):
        return self._sum(r) - self._sum(l)
    
    def _sum(self, r):
        s = self.T()
        while r > 0:
            s += self.data[r - 1]
            r -= r & -r
        return s