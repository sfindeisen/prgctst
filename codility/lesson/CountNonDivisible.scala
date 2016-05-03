import scala.collection.JavaConversions._

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Array[Int] = {
        // write your code in Scala 2.10
        val n = A.length
        if (n <= 1)
            return Array.fill[Int](0 max n)(0)

        val mxv = A.reduceRight(_ max _)
        val ctr: Array[Int] = Array.fill[Int](1+mxv)(0)
        A.foreach(x => ctr(x) += 1)
        //ctr.foreach(x => println("ctr: " + x))
        val sieve: Array[Int] = Array.fill[Int](1+mxv)(0)

        for (i <- 0 to n-1) {
            val x = A(i)
            if (1 <= ctr(x)) {
                var j = x
                while (j <= mxv) {
                    sieve(j) += ctr(x)
                    j += x
                }
                ctr(x) = 0
            }
        }
        //sieve.foreach(x => println("sieve: " + x))
        return Array.range(0,n).map(i => n-sieve(A(i)))
    }
}
