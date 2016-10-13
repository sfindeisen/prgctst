#include <cmath>
#include <iostream>

typedef unsigned long long ullong;

using namespace std;

int main() {
    ullong n;
    cin >> n;

    ullong b = n;
    ullong s = 0;

    for (ullong i=1; (i<=b); ++i) {
        ullong d = n/i;

        if (i <= d)
            s += d;
        if (d < b) {
            s += (b-d)*(i-1);
            b = d;
        }

        // cerr << "i=" << i << " d=" << d << " b=" << b << " s=" << s << endl;
    }

    cout << s << endl;
    return 0;
}
