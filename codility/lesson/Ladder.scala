import scala.collection.JavaConversions._

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int], B: Array[Int]): Array[Int] = {
        // write your code in Scala 2.10
        val n    = Math.min(A.length, B.length)
        val f    = Array.fill[Int](2+n)(0)
        val p30  = 0x40000000
        val r    = 30
        val pow2 = Array.fill[Int](1+r)(1)

        f(1) = 1
        for (i <- 2 to 1+n) {
            f(i) = (f(i-1)+f(i-2)) % p30
        }

        for (i <- 1 to r) {
            pow2(i) = 2*pow2(i-1)
        }

        Array.range(0,n).map(i => f(1+A(i)) % pow2(B(i)))
    }
}
