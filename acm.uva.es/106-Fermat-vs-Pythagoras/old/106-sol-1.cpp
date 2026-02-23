#include <cmath>
#include <iostream>
#include <set>

using namespace std;

int gcd(int a, int b) {
    if (0 == a)
        return b;
    if (0 == b)
        return a;
    if (a < b)
        return gcd(b % a, a);
    return gcd(a % b, a);
}

int main(char** args) {
    long n = 0;
    long n2 = 0;
    set<long> s;
    long relPrime = 0;

    while (! cin.eof()) {
        cin >> n;
        if (cin.bad() || cin.fail())
            return 0;

        cout << "---------------------------- n = " << n << endl;

        n2 = n * n;
        s.clear();
        relPrime = 0;
        bool ok = true;

        for (long i = 1; ok && (i <= n); i++) {
            long i2 = i * i;
            for (long j = i + 1; j <= n; j++) {
                long j2 = j * j;
                long two = i2 + j2;
                if (n2 < two) {
                    // cout << "break on i=" << i << ", j=" << j << endl;
                    if (j == i + 1)
                        ok = false;
                    break;
                }

                long k = (long) sqrt(two);
                if ((j < k) && ((k * k) == two)) {
                    // cout << "  " << i << " " << j << " " << k << endl;
                    s.insert(i);
                    s.insert(j);
                    s.insert(k);

                    if ((1 == gcd(i, j)) && (1 == gcd(j, k)) && (1 == gcd(i, k))) {
                        relPrime++;
                        cout << "  " << i << " " << j << " " << k << endl;
                    }
                }
            }
        }

        cout << "WYNIK (bf): " << relPrime << ' ' << (n - s.size()) << endl;
    }

    return 0;
}
