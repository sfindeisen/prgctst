// you can also use imports, for example:
// import java.util.*;

// you can write to stdout for debugging purposes, e.g.
// System.out.println("this is a debug message");

import java.util.HashSet;

class Solution {
    public int solution(int[] A) {
        // write your code in Java SE 8
        HashSet<Integer> s = new HashSet<>();
        for (int i=0; i < A.length; ++i) {
            if (s.contains(A[i]))
                s.remove(A[i]);
            else
                s.add(A[i]);
        }
        return s.iterator().next();
    }
}
