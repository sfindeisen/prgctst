import scala.collection.JavaConversions._

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(N: Int): Int = {
        // write your code in Scala 2.10
        if (N <= 0)
            return 0

        val qr: Int = Math.round(Math.sqrt(N).toFloat)
        var k  = 2
        for (i <- 2 to qr) {
            if (0 == (N % i))
                k += 2
        }
        if (N == qr*qr)
            k -= 1

        return k
    }
}
