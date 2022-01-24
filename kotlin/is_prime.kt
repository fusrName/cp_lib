// Ported from: https://ei1333.github.io/library/math/number-theory/fast-prime-factorization.cpp
private fun miller_rabin_primality_test_uint64(n: Long): Boolean {
    val mod = BigInteger.valueOf(n)
    var d = n - 1
    while(d % 2 == 0L) d /= 2
//    val e = BigInteger.valueOf(1)
    val rev = BigInteger.valueOf(n - 1)
    // http://miller-rabin.appspot.com/  < 2^64
    for(a in longArrayOf(2, 325, 9375, 28178, 450775, 9780504, 1795265022)) {
        if(n <= a) break
        var t = d
        var y = BigInteger.valueOf(a).modPow(BigInteger.valueOf(t), mod)
        while(t != n - 1 && y != BigInteger.ONE && y != rev) {
            y = y * y % mod
            t *= 2
        }
        if(y != rev && t % 2 == 0L) return false
    }
    return true
}

private fun miller_rabin_primality_test_uint32(n: UInt): Boolean {
//    Mod32::set_mod(n);
    var d = n - 1U
    while(d % 2U == 0U) d /= 2U
//    val e = 1UL
    val rev: ULong = n - 1UL
    fun modpow(x: ULong, k: UInt): ULong {
        var xk = x
        var k = k
        var res = 1UL
        while (k != 0U) {
            if (k and 1U == 1U) res = res * xk % n
            k = k shr 1
            xk = xk * xk % n
        }
        return res
    }
    for(a in uintArrayOf(2U, 7U, 61U)) {
        if(n <= a) break
        var t = d
        var y = modpow(a.toULong(), t)
        while(t != n - 1U && y != 1UL && y != rev) {
            y = y * y % n
            t *= 2U
        }
        if(y != rev && t % 2U == 0U) return false
    }
    return true
}

private fun is_prime(n: Long): Boolean {
    if(n == 2L) return true
    if(n == 1L || n % 2 == 0L) return false
    if(n < 1L shl 31) return miller_rabin_primality_test_uint32(n.toUInt())
    return miller_rabin_primality_test_uint64(n)
}