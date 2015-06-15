import scala.collection.JavaConversions._

// you can use println for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
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
}
