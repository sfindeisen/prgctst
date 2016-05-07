import scala.collection.JavaConversions._

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = A.length
        if (n <= 2)
            return 0
        val peaks = (0 to n).map(i => ((1<=i) && (i<=n-2) && (A(i-1)<A(i)) && (A(i+1)<A(i))))
        val pkdir = (0 to n).scanRight(1+n)((i,k)=>if (peaks(i)) i else k)
        val qr: Int = Math.round(Math.sqrt(n).toFloat)
        for (i <- 1+qr to 1 by -1) {
            var k = 0
            var j = 0
            while ((j < n-1) && (k < i)) {
                if (j==pkdir(j)) {
                    k += 1
                    j += i
                } else {
                    j = pkdir(j)
                }
            }
            if (i <= k)
                return i
        }
        return 0
    }
}
