#include <iostream>

typedef long long bigint;

using namespace std;

const short SUp    = 0;
const short SDown  = 1;
const short SStart = 2;

bigint N = 0;
bigint h = 0;
bigint d = 0;
short  s = SStart;

inline void startNewChain(bigint ropeLen) {
    // cerr << "start new chain (" << ropeLen << ")" << endl;
    h = ropeLen;
    d = ropeLen;
    s = SUp;
}

bigint czytajLicz() {
    bigint p = 0;
    bigint A, B;

    cin >> N;

    if (N <= 0)
        return 0;

    cin >> A >> B;
    startNewChain(A + B);

    for (bigint i = 1; i < N; ++i) {
        cin >> A >> B;
        bigint rLen = A + B;

        if (SUp == s) {
            if (h <= rLen) {
                h = rLen - h;
                s = SDown;
                ++p;
            } else {
                if (rLen + d < h) {
                    startNewChain(rLen);
                } else {
                    d -= (h - rLen);
                    h  = 0;
                    s  = SDown;
                    ++p;
                }
            }
        } else {
            if (h <= rLen) {
                h = rLen - h;
                d = min(d, h);
                s = SUp;
                ++p;
            } else {
                startNewChain(rLen);
            }
        }

        // cerr << "Loop: i=" << i << " h=" << h << " s=" << s << " d=" << d << " r=" << rLen << endl;
    }

    return p;
}

int main() {
    bigint p = czytajLicz();
    cout << (p) << endl;
    return 0;
}
