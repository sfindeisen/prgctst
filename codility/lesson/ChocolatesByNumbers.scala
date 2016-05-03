import scala.collection.JavaConversions._

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def euclid(a: Int, b: Int): Int = {
        if ((a < 0) || (b < 0))
            return euclid(Math.abs(a), Math.abs(b))
        if (b < a)
            return euclid(b, a)
        if (0 == a)
            return b
        return euclid(b % a, a)
    }

    def solution(N: Int, M: Int): Int = {
        // write your code in Scala 2.10
        return (N / euclid(N,M))
    }
}
