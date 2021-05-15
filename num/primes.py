Primes_MAX = 10 ** 6 + 100
Primes = []
Sieve = [0] * Primes_MAX
Sieve[0] = Sieve[1] = -1
for p in range(2, Primes_MAX):
    if not Sieve[p]:
        Primes.append(p)
        Sieve[p] = p
        for q in range(p * p, Primes_MAX, p):
            if not Sieve[q]:
                Sieve[q] = p