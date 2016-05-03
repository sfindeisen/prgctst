import scala.collection.JavaConversions._

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(N: Int): Int = {
        // write your code in Scala 2.10
        if (N <= 1)
            return 4
        val qr: Int = Math.round(Math.sqrt(N).toFloat)
        for (i <- qr to 2 by -1) {
            if (0 == (N % i)) {
                return 2*(i + N/i)
            }
        }
        return 2*(N+1)
    }
}
