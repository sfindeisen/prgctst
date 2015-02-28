#include <iostream>

using namespace std;

int main() {
    long n = 0;
    cin >> n;

    long p0 = 1;
    long p1 = 1;

    for (long j = 2; j <= n; ++j) {
        long k = (p0 + p1) % 10;
        p0 = p1;
        p1 = k;
    }

    cout << (p1 % 10) << endl;
    return 0;
}
