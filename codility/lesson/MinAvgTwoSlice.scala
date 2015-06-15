import scala.collection.JavaConversions._

// you can use println for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = A.length        
        var best_ind = 0
        var best_sum: Long = A.sum
        var best_len = n        
        var j = n
        var s: Long = 0
        
        for (i <- n-1 to 0 by -1) {
            s += A(i)
            
            while ((i+3 <= j) && (s*(j-i-1) > (s-A(i))*(j-i))) {
                s -= A(j-1)
                j -= 1
                
                if ((s*best_len) <= best_sum*(j-i)) {
                    best_sum = s
                    best_len = j-i
                    best_ind = i
                }
            }
            
            if ((i+2 <= j) && ((s*best_len) <= best_sum*(j-i))) {
                best_sum = s
                best_len = j-i
                best_ind = i
            }
        }
        
        best_ind
    }
}
