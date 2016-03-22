import java.util.Arrays;

class Solution {
    public int[] solution(int N, int[] A) {
        int[] t = new int[N];
        Arrays.fill(t, 0);
        int maxk = 0;
        int cmax = 0;

        for (int i=0; i<A.length; ++i) {
            final int op = A[i];

            if ((1 <= op) && (op <= N)) {
                t[op-1] = 1 + Math.max(t[op-1], maxk);
                cmax = Math.max(cmax, t[op-1]);
            } else if ((N+1) == op) {
                maxk = cmax;
            }
        }
        
        for (int i=0; i<N; ++i) {
            t[i] = Math.max(t[i], maxk);
        }

        return t;
    }
}
