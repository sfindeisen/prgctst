import scala.collection.JavaConversions._

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = A.length
        if (n <= 1)
            return 0
        val peaks: Array[Int] = Array.fill[Int](n)(0)
        for (i <- 1 to n-2) {
            if ((A(i-1) < A(i)) && (A(i+1) < A(i))) {
                peaks(i) = 1
            }
        }
        val pksum: Array[Int] = Array.fill[Int](n)(0)
        pksum(0) = peaks(0)
        for (i <- 1 to n-1) {
            pksum(i) = pksum(i-1) + peaks(i)
        }
        for (i <- 2 to n) {
            if (0 == (n%i)) {
                val k = n/i;
                var j = 1
                var p = 0
                while ((j <= k) && ((1+p) <= pksum(j*i-1))) {
                    p  = pksum(j*i-1)
                    j += 1
                }
                if (k < j)
                    return k
            }
        }
        return 0
    }
}
