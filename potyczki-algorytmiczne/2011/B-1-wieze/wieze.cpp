#include <cstring>

#include <iostream>

using namespace std;

const int MaxN = 1000;
const char CE = '.';
const char CW = 'W';
int  ri [MaxN + 2];
int  ci [MaxN + 2];
char buf[MaxN + 3];
int n = 0;

inline bool isWieza(char c) {
    return (CW == c);
}

void parseRow(int k) {
    // cout << "got line: " << buf << endl;

    for (int i = 0; i < n; ++i) {
        if (isWieza(buf[i])) {
            ci[i+1] = k;
            ri[k]   = i+1;
            // cout << "jest: " << k << " " << (i+1) << endl;
            break;
        }
    }
}

void printRow(int k) {
    for (int i = 1; i < k; ++i) {
        cout << CE;
    }
    cout << CW;
    for (int i = 1 + k; i <= n; ++i) {
        cout << CE;
    }
    cout << endl;
}

int main() {
    memset(ri,  0, sizeof(ri));
    memset(ci,  0, sizeof(ci));
    memset(buf, 0, sizeof(buf));

    cin >> n;

    for (int i = 1; i <= n; ++i) {
        cin.getline(buf, 1 + n);

        if ((CE == (buf[0])) || (isWieza(buf[0]))) {
            // proper board row
            parseRow(i);
        } else {
            --i;
        }

        buf[0] = 0;
    }

    int k = 1;

    for (int i = 1; i <= n; ++i) {
        if (0 == (ri[i])) {
            while (ci[k])
                ++k;
            ri[i] = k;
            ci[k] = i;
            // cout << "dost: " << i << " " << k << endl;
        }

        printRow(ri[i]);
    }

    return 0;
}
