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
        return mint(self.i * (m.i if isinstance(m, mint) else m) % MOD)
    def __sub__(self, m):
        t = self.i - (m.i if isinstance(m, mint) else (m % MOD))
        if t < 0:
            t += MOD
        return mint(t)
    def __pow__(self, m):
        i = self.i
        return mint(pow(i, m, MOD))
    def __truediv__(self, m):
        return mint(self.i * pow(m, MOD - 2, MOD) % MOD)
    def __repr__(self):
        return repr(self.i)

