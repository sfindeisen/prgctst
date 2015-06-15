import scala.collection.JavaConversions._
import scala.collection.mutable.Stack

// you can use println for debugging purposes, e.g.
// println("this is a debug message")

object Solution {
    def solution(S: String): Int = {
        // write your code in Scala 2.10
        val s = Stack[Char]()

        for (c <- S) {
            if (('['==c) || ('('==c) || ('{'==c))
                s.push(c)
            else {
                if (s.isEmpty)
                    return 0
                val d = s.pop()
                if (! ((('['==d) && (']'==c)) || (('('==d) && (')'==c)) || (('{'==d) && ('}'==c))))
                    return 0
            }
        }
        
        if (s.isEmpty) 1 else 0
    }
}
