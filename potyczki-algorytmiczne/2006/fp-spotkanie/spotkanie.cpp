#include <cstring>
#include <iostream>

typedef long long int LLInt;

using namespace std;

LLInt n;
LLInt t[10][10];

int oblicz() {
    if (n <= 0)
        return 0;
    if (1 == n)
        return 1;
    if (2 == n)
        return 2;
    if (3 == n)
        return 4;

    int f0=1;
    int f1=1;
    LLInt i = 2;

    while (i <= n-1) {
        int k = (f0 + f1) % 10;
        f0 = f1;
        f1 = k;

        //cerr << i << ": " << f0 << " " << f1 << endl;

        if (t[f0][f1]) {
            int d = i - t[f0][f1];
            //cerr << "cycle: " << i << " " << f0 << " " << f1 << " len=" << d << endl;

            if (d < n-1-i) {
                LLInt r = (n-1-i) % d;
                i = n-r;
                continue;
            }
        }

        t[f0][f1] = i++;
    }

    return ((f1 + 2*f0) % 10);
}

int main() {
    memset(t, 0, sizeof(t));

    cin >> n;
    // cout << sizeof(t) << endl;
    cout << oblicz() << endl;

    return 0;
}
