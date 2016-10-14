#include <iostream>

typedef unsigned long long ullong;

const long c1 = 142857;
const long c2 = 285714;
const long c3 = 428571;
const long c4 = 571428;
const long c5 = 714285;
const long c6 = 857142;

using namespace std;

int main() {
    ullong n;
    cin >> n;

    if (n <= 9)
        cout << n;
    else if (n <= c1)
        cout << c1;
    else if (n <= c2)
        cout << c2;
    else if (n <= c3)
        cout << c3;
    else if (n <= c4)
        cout << c4;
    else if (n <= c5)
        cout << c5;
    else if (n <= c6)
        cout << c6;
    else
        cout << "BRAK";

    cout << endl;
    return 0;
}
