import scala.collection.JavaConversions._

// you can use println for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def dominatorIndex(A: Array[Int]): Int = {
        val n = A.length
        var k: Int = 1
        var j: Int = 0
        
        for (i <- 1 to n-1) {
            if (A(j)==A(i))
                k += 1
            else {
                k -= 1
                
                if (k < 0) {
                    j = i
                    k = 1
                }
            }
        }
        
        // check
        if (2 * A.count(_ == A(j)) > n) j else -1
    }

    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n  = A.length
        val di = dominatorIndex(A)
        if (di<0) return 0
        val d  = A(di)
        val dc = A.count(_ == d)
        var k  = 0
        var j  = 0

        for (i <- 0 to n-2) {
            if (d == A(i))
                k += 1

            if ((2*k > i+1) && (2*(dc-k) > (n-i-1))) {
                j += 1
            }
        }
        
        j
    }
}
