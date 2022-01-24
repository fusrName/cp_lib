class Mat(val size: Int) {
    val d = Array(size) { newMintArray(size) }
    operator fun times(rhs: Mat): Mat {
        val res = Mat(size)
        for (i in 0 until size) {
            for (j in 0 until size) {
                for (k in 0 until size) {
                    res[i][k] += this[i][j] * rhs[j][k]
                }
            }
        }
        return res
    }
    fun makeIdentity() {
        d.forEach { for (i in 0 until size) it[i] = Mint(0) }
        for (i in 0 until size) d[i][i] = Mint(1)
    }
    inline operator fun get(i: Int) = d[i]
    fun pow(n: Long): Mat {
        require(n >= 0L)
        var res = Mat(size)
        res.makeIdentity()
        var k = n
        var mk = this
        while(k != 0L) {
            if (k and 1 == 1L) {
                res *= mk
            }
            mk *= mk
            k = k shr 1
        }
        return res
    }
}

class Mat2( val v00: Mint = Mint(1), val v01: Mint = Mint(0),
            val v10: Mint = Mint(0), val v11: Mint = Mint(1) ) {
    operator fun times(rhs: Mat2) =
        Mat2(
            v00*rhs.v00+v01*rhs.v10, v00*rhs.v10+v01*rhs.v11,
            v10*rhs.v00+v11*rhs.v10, v10*rhs.v10+v11*rhs.v11
        )
    fun pow(n: Long): Mat2 {
        require(n >= 0)
        var x = n
        var res = Mat2()
        var mk = this
        while (x != 0L) {
            if (x and 1 == 1L) {
                res *= mk
            }
            x = x shr 1
            mk *= mk
        }
        return res
    }
}