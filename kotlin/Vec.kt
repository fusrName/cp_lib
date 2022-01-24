private typealias Vec = Pair<Long, Long>
private inline fun Vec.cross(rhs:Vec)=this.first*rhs.second-this.second*rhs.first
private inline fun Vec.dot(rhs:Vec)=this.first*rhs.first+this.second*rhs.second
private inline operator fun Vec.plus(rhs:Vec)=Vec(this.first+rhs.first,this.second+rhs.second)
private inline operator fun Vec.minus(rhs:Vec)=Vec(this.first-rhs.first,this.second-rhs.second)

private fun Vec.isOnLineSegment(u: Vec, v: Vec): Boolean {
    val x = u - this
    val y = v - this
    return x.cross(y) == 0L && x.dot(y) <= 0
}