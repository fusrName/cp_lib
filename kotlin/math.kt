import kotlin.math.abs

private fun gcd(a: Int, b: Int): Int {
    tailrec fun rec(x: Int, y: Int): Int {
        return if (y == 0) x else rec(y, x % y)
    }
    return rec(abs(a), abs(b))
}

private fun lcm(a: Int, b: Int) = a / gcd(a, b) * b

private fun divisors(n: Long): ArrayList<Long> {
    val divs1 = ArrayList<Long>()
    val divs2 = ArrayList<Long>()
    var d = 1L
    while (d * d < n) {
        if (n % d == 0L) {
            divs1 += d
            divs2 += n / d
        }
        d += 1
    }
    if (d * d == n) divs1 += d
    divs2.reverse()
    divs1 += divs2
    return divs1
}

private fun factorize(n: Long): ArrayList<Pair<Long, Int>> {
    var n = n
    val res = ArrayList<Pair<Long, Int>>()
    var p = 2L
    while (p * p <= n) {
        var cnt = 0
        while (n % p == 0L) {
            n /= p
            cnt++
        }
        if (cnt > 0) res += Pair(p, cnt)
        p += 1
    }
    if (n != 1L) res += Pair(n, 1)
    return res
}