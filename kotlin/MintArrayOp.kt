// O(NM)
operator fun MintArray.times(rhs: MintArray): MintArray {
    val sz1 = this.size
    val sz2 = rhs.size
    val res = newMintArray(sz1 + sz2 - 1)
    for (i in 0 until sz1) {
        for (j in 0 until sz2) {
            res[i + j] += this[i] * this[j]
        }
    }
    return res
}