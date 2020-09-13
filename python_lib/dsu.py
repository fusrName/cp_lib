class dsu:
    def __init__(self, n=0):
        self._n = n
        self.parent_or_size = [-1] * n
    
    def merge(self, a: int, b: int) -> int:
        x = self.leader(a)
        y = self.leader(b)
        if x == y:
            return x
        if self.parent_or_size[x] > self.parent_or_size[y]:
            x, y = y, x
        self.parent_or_size[x] += self.parent_or_size[y]
        self.parent_or_size[y] = x
        return x
    
    def same(self, a: int, b: int) -> bool:
        return self.leader(a) == self.leader(b)
    
    def leader(self, a: int) -> int:
        x = a
        while self.parent_or_size[x] >= 0:
            x = self.parent_or_size[x]
        while a != x:
            self.parent_or_size[a], a = x, self.parent_or_size[a]
        return x
    
    def size(self, a: int) -> int:
        return -self.parent_or_size[self.leader(a)]
    
    def groups(self):
        g = [[] for _ in range(self._n)]
        for i in range(self._n):
            g[self.leader(i)].append(i)
        return list(c for c in g if c)
