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

    def isDivSubset(a: Int, b: Int): Boolean = {
        if ((a < 0) || (b < 0))
            return isDivSubset(Math.abs(a), Math.abs(b))
        if ((0 == b) || (1 == a))
            return true
        if ((0 == a) || (1 == b))
            return false
        val d = euclid(a,b)
        if (1 == d)
            return (1 == a)
        else
            return isDivSubset(a/d, b)
    }

    def solution(A: Array[Int], B: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = Math.min(A.length, B.length)
        var k = 0
        for (i <- 0 to n-1) {
            val a = A(i)
            val b = B(i)
            if (isDivSubset(a,b) && isDivSubset(b,a))
                k += 1
        }
        return k
    }
}
