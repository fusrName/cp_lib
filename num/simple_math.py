def divisors(x):
    d = 1
    res1 = []
    res2 = []
    while d * d < x:
        if x % d == 0:
            res1.append(d)
            res2.append(x // d)
        d += 1
    if d * d == x:
        res1.append(d)
    while res2:
        res1.append(res2.pop())
    return res1

def factorize(x):
    ret = []
    p = 2
    while p * p <= x:
        cnt = 0
        while x % p == 0:
            x //= p
            cnt += 1
        if cnt:
            ret.append((p, cnt))
        p += 1
    if x > 1:
        ret.append((x, 1))
    return ret
