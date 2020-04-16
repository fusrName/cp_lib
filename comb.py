mod = 10**9 + 7
facts = [1] * 100001
for i in range(0, 100000):
	facts[i+1] = facts[i] * (i + 1) % mod

ifacts = [1] * 100001
ifacts[100000] = pow(facts[100000], mod - 2, mod)
for i in range(100000, 0, -1):
	ifacts[i-1] = ifacts[i] * i % mod

def comb(n, k):
	return facts[n] * ifacts[n-k] % mod * ifacts[k] % mod
