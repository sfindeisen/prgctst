#include <iostream>
#include <iomanip>

typedef unsigned long long bigint;

bigint Dist   = 0;
bigint WStart = 0;
bigint Cap    = 0;

using namespace std;

double oblicz() {
    if (WStart <= Cap)
        return WStart - Dist;

    double dist = Dist;
    bigint wat  = WStart;
    bigint r    = WStart % Cap;
    bigint k    = WStart / Cap;
    bigint p    = 2*k+1;

    if (r) {
        double d = ((double) r / p);
        // cerr << "r=" << r << ", d=" << d << endl;

        if (d > dist) {
            return wat - (dist * p);
        } else {
            dist -= d;
            wat  -= r;
            p    -= 2;
        }
    } else {
        p -= 2;
    }

    while (k) {
        // cerr << "Loop: dist=" << dist << " wat=" << wat << " k=" << k << " p=" << p << endl;

        if (wat <= Cap) {
            return wat - dist;
        }

        double d = ((double) Cap / p);
        // cerr << "Loop: d = " << d << endl;

        if (d > dist) {
            return wat - (dist * p);
        } else {
            dist -= d;
            wat  -= Cap;
            k    -= 1;
            p    -= 2;
        }
    }

    return wat;
}

int main() {
    cin >> Dist >> WStart >> Cap;
    //cerr << "Dist   = " << Dist << endl;
    //cerr << "WStart = " << WStart << endl;
    //cerr << "Cap    = " << Cap << endl;

    double res = oblicz();
    cout << setprecision (3) << fixed << res << endl;

    return 0;
}
