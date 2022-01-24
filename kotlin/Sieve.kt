private class Sieve(val n: Int) {
    val lpf = IntArray(n + 1)
    val primes = ArrayList<Int>()
    init {
        if (n >= 2) {
            for (i in 2..n) {
                if (lpf[i] == 0) {
                    lpf[i] = i
                    primes.add(i)
                }
                val p = lpf[i]
                for (q in primes) {
                    val j = i * q
                    if (j > n) break
                    lpf[j] = p
                    if (q == p) break
                }
            }
        }
    }
}