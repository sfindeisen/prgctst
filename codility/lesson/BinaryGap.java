// you can also use imports, for example:
// import java.util.*;

// you can write to stdout for debugging purposes, e.g.
// System.out.println("this is a debug message");

class Solution {
    public int solution(int N) {
        // write your code in Java SE 8
        int j=-1;
        int x=0;

        for (int i=0; (0!=N); ++i) {
            if (1 == (N % 2)) {
                if (0 <= j)
                    x = Math.max(x, i-j-1);
                j=i;
            }
            N /= 2;
        }

        return x;
    }
}

