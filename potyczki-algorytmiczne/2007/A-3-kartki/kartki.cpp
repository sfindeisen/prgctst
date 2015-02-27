#include <iostream>

using namespace std;

typedef long long int LongInt;

inline LongInt suma(LongInt a, LongInt b) {
    if (b < a)
        return 0;
    LongInt n = b - a + 1;
    return (n * (a + b)) / 2;
}

LongInt kartki(LongInt N, LongInt K, LongInt R) {
    // cout << N << " " << K << " " << R << endl;

    if (1 == K)
        return ((R % N) ? (R % N) : N);

    LongInt no = 0;     // offset
    LongInt ne = N;
    LongInt r  = R;

    LongInt s  = 0;
    LongInt kp = 1;     // K^p

    while (r && (no < ne)) {
        const LongInt n = ne - no;

        LongInt no2 = no;
        LongInt ne2 = ne;
        LongInt s2  = s;
        const LongInt kp2 = kp * K;
        const LongInt k1  = (n/kp2);
        LongInt kr = (n%kp2);

        if (r <= k1)
            return suma(1 + no + (r-1) * kp2, no + r * kp2);

        r -= k1;

        const LongInt w = suma(1 + ne - kr, ne);
        ne2 -= kr;
        kr  /= kp;

        const LongInt i = K - kr - (s ? 1 : 0);
        if (k1 <= i)
            return suma(1, N);

        no2 += i * kp2;
        s2  += w + suma(1 + no, no2);
        r   -= 1;

        no = no2;
        ne = ne2;
        s  = s2;
        kp = kp2;
    }

    return s;
}

int main() {
    long t, n, k, r;
    cin >> t;

    for (int i = 0; i < t; ++i) {
        cin >> n >> k >> r;
        cout << kartki(n, k, r) << endl;
    }

    return 0;
}
