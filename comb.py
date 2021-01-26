MOD = 10**9 + 7

fact_range = 10**6
fact = [1] * (fact_range + 1)
for i in range(0, fact_range):
    fact[i+1] = fact[i] * (i + 1) % MOD

ifact = [1] * (fact_range + 1)
ifact[fact_range] = pow(fact[fact_range], MOD - 2, MOD)
for i in range(fact_range, 0, -1):
    ifact[i-1] = ifact[i] * i % MOD

def comb(n, k):
    if k < 0 or n < k:
        return 0
    else:
        return fact[n] * ifact[n-k] % MOD * ifact[k] % MOD

