MOD = 10**9 + 7
class mint:
    def __init__(self, i):
        self.i = i
    def __add__(self, m):
        t = self.i + (m.i if isinstance(m, mint) else m)
        if t > MOD:
            t -= MOD
        return mint(t)
    def __radd__(self, m):
        t = self.i + (m.i if isinstance(m, mint) else m)
        if t > MOD:
            t -= MOD
        return mint(t)
    def __mul__(self, m):
        return mint(self.i * m.i % MOD)
    def __sub__(self, m):
        t = self.i - m.i
        if t < 0:
            t += MOD
        return mint(t)
    def __pow__(self, m):
        i = self.i
        res = 1
        while(m > 0):
            if m & 1:
                res = res * i % MOD 
            i = i * i % MOD
            m >>= 1
        return mint(res)
    def __truediv__(self, m):
        return mint(self.i * (m ** (MOD - 2)).i % MOD)
    def __repr__(self):
        return repr(self.i)

