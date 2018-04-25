/* FIFO queue */

#include <iostream>
#include <cstring>
#include <queue>

const long MaxN = 2000;
      long    N = 0;
      long    M = 0;
      long    K = 0;

long   t[MaxN + 2][MaxN + 2];         // pole
long csp[MaxN + 2][MaxN + 2];         // column sum prefix (col, row)
long rsp[MaxN + 2][MaxN + 2];         // row    sum prefix (row, col)

/** current rectangle in BFS search */
typedef struct {
    int ci1;                  // left  column
    int ci2;                  // right column
    int ri1;                  // first bad row (top)
    int ri2;                  // last  bad row (bottom)
} TNode;

using namespace std;

void init() {
    memset(t, 0, sizeof(t));
}

void readData() {
    cin >> K >> M >> N;

    for (int i = 1; (i <= N); ++i) {
        for (int j = 1; (j <= M); ++j) {
            cin >> t[i][j];
        }
    }
}

void prefixSums() {
    memset(csp, 0, sizeof(csp));
    memset(rsp, 0, sizeof(rsp));

    for (int i = 1; i <= N; ++i) {
        long s = 0;

        for (int j = 1; j <= M + 1; ++j) {
            s += t[i][j];
            rsp[i][j] = s;
        }
    }

    for (int i = 1; i <= M; ++i) {
        long s = 0;

        for (int j = 1; j <= N + 1; ++j) {
            s += t[j][i];
            csp[i][j] = s;
        }
    }
}

inline void swapt(int x1, int x2, int y1, int y2) {
    long x = t[x1][x2];
    t[x1][x2] = t[y1][y2];
    t[y1][y2] = x;
}

void transpose() {
    long d    = min(N, M);
    long dmax = max(N, M);

    if (d < dmax) {
        for (int i = d + 1; (i <= dmax); ++i) {
            for (int j = 1; (j <= d); ++j) {
                swapt(i, j, j, i);
            }
        }
    }

    for (int i = 1; (i <= d); ++i) {
        for (int j = 1; (j < i); ++j) {
            swapt(i, j, j, i);
        }
    }

    // swap dimensions
    long u = N;
    N = M;
    M = u;
}

inline long colsum1(const TNode& node) {
    return csp[node.ci1][node.ri2] - csp[node.ci1][node.ri1 - 1];
}
inline long colsum2(const TNode& node) {
    return csp[node.ci2][node.ri2] - csp[node.ci2][node.ri1 - 1];
}
inline long rowsum1(const TNode& node) {
    return rsp[node.ri1][node.ci2] - rsp[node.ri1][node.ci1 - 1];
}
inline long rowsum2(const TNode& node) {
    return rsp[node.ri2][node.ci2] - rsp[node.ri2][node.ci1 - 1];
}

short moveRows(TNode& node) {
    while ((1 <= node.ri2) && (rowsum2(node) <= K))
        node.ri2--;
    while ((node.ri1 <= N) && (rowsum1(node) <= K))
        node.ri1++;
    return (node.ri2 <= 0);
}

long oblicz() {
    TNode start;
    start.ci1 = 1;
    start.ci2 = M;
    start.ri1 = 1;
    start.ri2 = N;

    if (moveRows(start))
        return N;           // wszystkie skiby poziome sa OK

    // BFS init
    char v[MaxN + 2][MaxN + 2];
    memset(v, 0, sizeof(v));
    queue<TNode> bfs;
    bfs.push(start);

    while (! (bfs.empty())) {
        const TNode& cur(bfs.front());
        const long ci1 = cur.ci1;
        const long ci2 = cur.ci2;

        // cout << "DEBUG: " << cur.ri1 << "," << cur.ci1 << " .. " << cur.ri2 << "," << cur.ci2 << std::endl;

        if (ci1 < ci2) {
            if (! (v[ci1 + 1][ci2])) {
                if (colsum1(cur) <= K) {
                    TNode node(cur);
                    node.ci1 = ci1 + 1;

                    if (moveRows(node)) {
                        // cout << "DEBUG 2: OK" << std::endl;
                        return N + M - (ci2 - ci1);
                    }

                    bfs.push(node);
                    v[ci1 + 1][ci2] = 1;
                }
            }

            if (! (v[ci1][ci2 - 1])) {
                if (colsum2(cur) <= K) {
                    TNode node(cur);
                    node.ci2 = ci2 - 1;

                    if (moveRows(node)) {
                        // cout << "DEBUG 3: OK" << std::endl;
                        return N + M - (ci2 - ci1);
                    }

                    bfs.push(node);
                    v[ci1][ci2 - 1] = 1;
                }
            }
        }

        bfs.pop();
    }

    return -1;      // error
}

int main() {
    init();
    readData();

    prefixSums();
    long res1 = oblicz();

    transpose();

    prefixSums();
    long res2 = oblicz();

    //cout << "res1=" << res1 << " / res2=" << res2 << std::endl;

    long res = 0;
    if (res1 < 0)
        res  = res2;
    else
    if (res2 < 0)
        res  = res1;
    else
        res  = min(res1, res2);

    cout << res << std::endl;
}

