// you can also use imports, for example:
// import java.util.*;

// you can write to stdout for debugging purposes, e.g.
// System.out.println("this is a debug message");

class Solution {
    public int[] solution(int[] A, int K) {
        // write your code in Java SE 8
        final int N = A.length;
        if (N <= 1)
            return A;
        K %= N;
        if (0 == K)
            return A;
        int[] b = new int[N];
        System.arraycopy(A, N-K, b, 0, K);
        System.arraycopy(A, 0, b, K, N-K);
        return b;
    }
}
