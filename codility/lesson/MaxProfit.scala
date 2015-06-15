import scala.collection.JavaConversions._
import scala.math.max

// you can use println for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = A.length
        var smallIdx = 0
        var best = 0

        for (i <- 1 to n-1) {
            if (A(i) < A(smallIdx))
                smallIdx = i
            else
                best = max (best, A(i) - A(smallIdx))
        }
        
        best
    }
}
