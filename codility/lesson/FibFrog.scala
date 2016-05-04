import scala.collection.JavaConversions._
import scala.collection.mutable.ArrayBuffer

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(A: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = A.length;
        if (n <= 2)
            return 1
        val t = new Array[Int](2+n);
        Array.copy(A, 0, t, 1, n)
        t(0)   = 1
        t(n+1) = 1

        val fibo = new ArrayBuffer[Int]();
        var f0 = 0
        var f1 = 1
        fibo += f0
        fibo += f1
        while (f0 <= n) {
            val f2 = f0 + f1
            fibo += f2
            f0 = f1
            f1 = f2
        }

        val skok = Array.fill[Int](2+n)(0)

        for (i <- 0 to 1+n) {
            if (1==t(i)) {
                // println ("i=" + i)
                var k = 1+i
                for (j <- fibo.size-1 to 1 by -1) {
                    val fj = fibo(j)
                    if ((i==fj) || ((fj < i) && (1 <= skok(i-fj)))) {
                        k = k min (1+skok(i-fj))
                        // println ("  fj=" + fj + " -> " + k)
                    }
                }
                if (k <= i) {
                    skok(i) = k
                    // println ("  " + i + " -> " + skok(i))
                }
            }
        }

        return if (0==skok(1+n)) -1 else skok(1+n)
    }
}
