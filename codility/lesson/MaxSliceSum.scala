import scala.collection.JavaConversions._

// you can use println for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = A.length
        var s: Long = 0
        var j = 0
        var best: Long = A.max

        for (i <- 0 to n-1) {
            s += A(i)
            
            while (s < 0) {
                s -= A(j)
                j += 1

                if ((j <= i) && (best < s))
                    best = s
            }
            
            if ((j <= i) && (best < s))
                best = s
        }
        
        best.toInt
    }
}
