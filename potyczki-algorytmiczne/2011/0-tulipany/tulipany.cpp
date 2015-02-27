#include <cstring>
#include <iostream>

using namespace std;

const int MaxN = 15000;
char t[MaxN + 2];

int main() {
    memset(t, 0, sizeof(t));

    int g = 0;
    int k = 0;

    cin >> g;

    for (int i = 0; i < g; ++i) {
        cin >> k;
        t[k] = 1;
    }

    int j = 0;

    for (int i = 0; i <= MaxN; ++i) {
        if (t[i])
            ++j;
    }

    cout << (15000 - j) << endl;
}
