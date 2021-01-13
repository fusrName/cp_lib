MOD = 10**9 + 7

fact_range = 10**6
facts = [1] * (fact_range + 1)
for i in range(0, fact_range):
    facts[i+1] = facts[i] * (i + 1) % MOD

ifacts = [1] * (fact_range + 1)
ifacts[fact_range] = pow(facts[fact_range], MOD - 2, MOD)
for i in range(fact_range, 0, -1):
    ifacts[i-1] = ifacts[i] * i % MOD

def comb(n, k):
    if k < 0 or n < k:
        return 0
    else:
        return facts[n] * ifacts[n-k] % MOD * ifacts[k] % MOD

