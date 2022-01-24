fun linearRecurrence(a: MintArray, n: Long): MintArray {
    val d = a.size
    require(d > 0 && n >= 0)
    var f = newMintArray(d)
    f[0] = Mint(1)
    if (n == 0L) return f
    for (k in lsb(n.takeHighestOneBit()) downTo 0) {
        val g = f * f
        for (i in 2 * d - 2 downTo d) {
            for (j in 0 until d) {
                g[i - 1 - j] += g[i] * a[j]
            }
        }
        if (n shr k and 1 == 1L) {
            for (i in d downTo 1) {
                g[i] = g[i - 1]
            }
            g[0] = Mint(0)
            for (j in 0 until d) {
                g[d - 1 - j] += g[d] * a[j]
            }
        }
        g.a.copyInto(f.a, 0, 0, d)
    }
    return f
}