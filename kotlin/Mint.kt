const val Mod = 998244353
//const val Mod = 1000000007
inline class Mint(val v: Int) {
    inline operator fun plus(rhs: Mint): Mint {
        val res = v + rhs.v
        return Mint(if (res > Mod) res - Mod else res)
    }
    inline operator fun minus(rhs: Mint): Mint {
        val res = v - rhs.v
        return Mint(if (res < 0) res + Mod else res)
    }
    inline operator fun times(rhs: Mint) = Mint((v.toLong() * rhs.v % Mod).toInt())
    override fun toString() = v.toString()
    fun pow(n: Long): Mint {
        require(n >= 0)
        var n = n
        var x = this
        var r = Mint(1)
        while (n != 0L) {
            if (n and 1 != 0L) r *= x
            x *= x
            n = n shr 1
        }
        return r
    }
    inline fun pow(n: Int) = pow(n.toLong())
    inline fun inv() = pow(Mod - 2)
    inline operator fun div(rhs: Mint) = this * rhs.inv()
}
inline fun mintOf(i: Int): Mint {
    val r = i % Mod
    return Mint(if (r >= 0) r else r + Mod)
}
inline fun mintOf(i: Long): Mint {
    val r = (i % Mod).toInt()
    return Mint(if (r >= 0) r else r + Mod)
}
class MintIterator(private val d: MintArray): Iterator<Mint> {
    private var i = 0
    override fun next() = d[i++]
    override fun hasNext() = i < d.size
}
inline class MintArray(val a: IntArray) {
    inline val size get() = a.size
    inline operator fun get(i: Int) = Mint(a[i])
    inline operator fun set(i: Int, x: Mint) { a[i] = x.v }
    operator fun iterator() = MintIterator(this)
}
inline fun newMintArray(size: Int) = MintArray(IntArray(size))
inline fun mintArrayOf(vararg elements: Int) = MintArray(intArrayOf(*elements))