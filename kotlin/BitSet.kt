class BitSet(val size: Int) {
    val bits = LongArray((size + 63) shr 6)
    fun copyInto(destination: BitSet, destinationOffset: Int, startIndex: Int, endIndex: Int): BitSet {
        assert(0 <= startIndex && startIndex <= endIndex && endIndex <= size)
        assert(0 <= destinationOffset && destinationOffset + (endIndex - startIndex) <= destination.size)
        var si = startIndex shr 6
        var so = startIndex and 63
        var ei = endIndex shr 6
        var eo = endIndex and 63
        var desti = destinationOffset shr 6
        var desto = destinationOffset and 63
        val dest = destination.bits
        if (si == ei) {
            val len = eo - so
            val len1 = 64 - desto
            val x = bits[si] ushr so and (0L.inv() shl len).inv()
            if (len < len1) {
                dest[desti] = dest[desti] and ((0L.inv() shl len).inv() shl desto).inv() or (x shl desto)
            } else {
                dest[desti] = dest[desti] and (0L.inv() shl desto).inv() or (x shl desto)
                if (len != len1) {
                    desti++
                    dest[desti] = dest[desti] and (0L.inv() shl len - len1) or (x ushr len1)
                }
            }
        } else if (so == desto) {
            val mask = 0L.inv() shl so
            dest[desti] = dest[desti] and mask.inv() or (bits[si] and mask)
            desti++
            si++
            bits.copyInto(dest, desti, si, ei)
            if (eo != 0) {
                desti += ei - si
                val mask = 0L.inv() shl eo
                dest[desti] = dest[desti] and mask or (bits[ei] and mask.inv())
            }
        } else {
            var buff: Long
            var buffSize: Int
            var restSize: Int
            if (so < desto) {
                dest[desti] = dest[desti] and (0L.inv() shl desto).inv() or (bits[si] ushr so shl desto)
                desti++
                buffSize = desto - so
                restSize = 64 - buffSize
                buff = bits[si++] ushr restSize
            } else {
                buff = dest[desti] and (0L.inv() shl desto).inv() or (bits[si++] ushr so shl desto)
                buffSize = desto + (64 - so)
                restSize = 64 - buffSize
            }
            while (si < ei) {
                val x = bits[si++]
                dest[desti++] = buff or (x shl buffSize)
                buff = x ushr restSize
            }
            val len = buffSize + eo
            if (len < 64) {
                val mask = 0L.inv() shl len
                dest[desti] = dest[desti] and mask or
                        if (eo == 0) buff else (buff or (bits[ei] shl buffSize) and mask.inv())
            } else {
                val x = bits[ei]
                dest[desti++] = buff or (x shl buffSize)
                if (len != 64) {
                    val mask = 0L.inv() shl len - 64
                    dest[desti] = dest[desti] and mask or (x ushr restSize and mask.inv())
                }
            }
        }
        return destination
    }
    operator fun get(i: Int) = bits[i shr 6] ushr (i and 63) and 1 != 0L
    operator fun set(i: Int, b: Boolean) {
        val idx = i shr 6
        val offset = i and 63
        bits[idx] = bits[idx] and (1L shl offset).inv() or
                ((if (b) 1L else 0L) shl offset)
    }
    infix fun and(rhs: BitSet): BitSet {
        val res = BitSet(size)
        val s = res.bits
        val t = rhs.bits
        for (i in 0 until bits.size) s[i] = bits[i] and t[i]
        return res
    }
    infix fun or(rhs: BitSet): BitSet {
        val res = BitSet(size)
        val s = res.bits
        val t = rhs.bits
        for (i in 0 until bits.size) s[i] = bits[i] or t[i]
        return res
    }
    infix fun xor(rhs: BitSet): BitSet {
        val res = BitSet(size)
        val s = res.bits
        val t = rhs.bits
        for (i in 0 until bits.size) s[i] = bits[i] xor t[i]
        return res
    }
    fun inplaceXor(rhs: BitSet) {
        val t = rhs.bits
        for (i in 0 until bits.size) bits[i] = bits[i] xor t[i]
    }
    companion object {
        @JvmStatic val memo = intArrayOf(0,1,2,53,3,7,54,27,4,38,41,8,34,55,48,28,62,5,39,46,44,42,22,9,24,35,59,56,49,18,29,11,63,52,6,26,37,40,33,47,61,45,43,21,23,58,17,10,51,25,36,32,60,20,57,16,50,31,19,15,30,14,13,12)
        @JvmStatic val seq = 0x022fdd63cc95386d
        fun lsb(x: Long) = memo[(((x and -x) * seq) ushr 58 and 63).toInt()]
    }
    fun findFirst(): Int {
        val sz = bits.size
        for (i in 0 until sz) if (bits[i] != 0L) {
            return 64 * i + lsb(bits[i])
        }
        return size
    }
    fun findNext(i: Int): Int {
        var idx = i ushr 6
        val offset = i and 63
        if (offset != 63) {
            val x = bits[idx] and (0L.inv() shl offset + 1)
            if (x != 0L) return 64 * idx + lsb(x)
        }
        idx++
        val sz = bits.size
        while(idx < sz) {
            if (bits[idx] != 0L) return 64 * idx + lsb(bits[idx])
            idx++
        }
        return size
    }
}