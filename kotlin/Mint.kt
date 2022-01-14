const val Mod=998244353

fun mintOf(i: Int): Mint {
    val r = i % Mod
    return Mint(if (r >= 0) r else r + Mod)
}
fun mintOf(i: Long): Mint {
    val r = (i % Mod).toInt()
    return Mint(if (r >= 0) r else r + Mod)
}
// @JvmInline
// value class Mint(val v: Int) {
inline class Mint(val v: Int) {
    operator fun plus(rhs: Mint): Mint {
        val res = v + rhs.v
        return Mint(if (res > Mod) res - Mod else res)
    }
    operator fun minus(rhs: Mint): Mint {
        val res = v - rhs.v
        return Mint(if (res < 0) res + Mod else res)
    }
    operator fun times(rhs: Mint) = Mint((v.toLong() * rhs.v % Mod).toInt())
    override fun toString() = v.toString()
    fun pow(n: Long): Mint {
        require(n >= 0)
        var n_ = n
        var x = this
        var r = Mint(1)
        while(n_ != 0L) {
            if (n_ and 1 != 0L) r *= x
            x *= x
            n_ = n_ shr 1
        }
        return r
    }
    fun pow(n: Int) = pow(n.toLong())
    fun inv() = pow(Mod - 2)
    operator fun div(rhs: Mint) = this * rhs.inv()
}
