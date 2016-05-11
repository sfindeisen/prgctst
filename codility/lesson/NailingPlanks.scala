import scala.collection.JavaConversions._
import scala.collection.mutable.Queue

// you can write to stdout for debugging purposes, e.g.
// println("this is a debug message")

abstract class Tree
case class Node(count: Int, minl: Int, maxr: Int, lc: Tree, rc: Tree) extends Tree
case class Leaf(alive: Boolean, i: Int) extends Tree

object Solution {
    def solution(A: Array[Int], B: Array[Int], C: Array[Int]): Int = {
        // write your code in Scala 2.10
        val n = A.length min B.length
        val m = C.length
        val p = (0 to n-1).sortWith((i,j)=>(A(i)<A(j)) || ((A(i)==A(j)) && (B(i)<B(j))))

        def treeCount(t: Tree): Int = t match {
            case Node(c,_,_,_,_) => c
            case Leaf(b,_) => if (b) 1 else 0
        }

        def minLeft(t: Tree): Int = t match {
            case Node(_,k,_,_,_) => k
            case Leaf(b,i) => if (b) A(p(i)) else Int.MaxValue
        }

        def maxRight(t: Tree): Int = t match {
            case Node(_,_,k,_,_) => k
            case Leaf(b,i) => if (b) B(p(i)) else Int.MinValue
        }

        def buildTree(a: Int, b: Int): Tree = {
            if (a < b) {
                val m    = (a+b)/2
                val lc   = buildTree(a,m)
                val rc   = buildTree(m+1,b)
                val cnt  = treeCount(lc)+treeCount(rc)
                val minl = minLeft(lc)
                val maxr = maxRight(lc) max maxRight(rc)
                Node(cnt, minl, maxr, lc, rc)
            } else
                Leaf(true, a)
            }

        def nailDown(tree: Tree, nail: Int): Tree = tree match {
            case Node(cnt, minl, maxr, lc, rc) =>
                if ((1<=cnt) && (minl<=nail) && (nail<=maxr)) {
                    val lc2   = nailDown(lc, nail)
                    val rc2   = nailDown(rc, nail)
                    val cnt2  = treeCount(lc2) + treeCount(rc2)
                    val minl2 = minLeft(lc2) min minLeft(rc2)
                    val maxr2 = maxRight(lc2) max maxRight(rc2)
                    Node(cnt2, minl2, maxr2, lc2, rc2)
                } else
                    tree
            case Leaf(b, k) =>
                if (b && (A(p(k)) <= nail) && (nail <= B(p(k))))
                    Leaf(false, k)
                else
                    tree
        }

        var root = buildTree(0,n-1)
        for (i <- 0 to m-1) {
            root = nailDown(root, C(i))
            if (0 == treeCount(root))
                return (1+i)
        }

        return -1
    }
}

