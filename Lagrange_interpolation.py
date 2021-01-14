def interpolate(dataset, x):
    n = len(dataset)
    rv = 0
    pf = [1] * (n + 1)
    pb = [1] * (n + 1)
    for i in range(n):
        pf[i+1] = pf[i] * (x - i) % MOD
    for i in range(n - 1, -1, -1):
        pb[i] = pb[i+1] * (x - i) % MOD
    for i in range(n):
        rv += pf[i] * pb[i + 1] * ifact[n - 1 - i] * ifact[i] * (-1 if n - 1 - i & 1 else 1) * dataset[i]
        rv %= MOD
    return rv
