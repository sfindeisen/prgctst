#include <iostream>

const long MaxN = 250000;
      long    N =      0;

long t[MaxN + 3];  // buildings (just heights)

using namespace std;

void init() {
    memset(t, 0, sizeof(t));
}

void czytaj() {
    long width;
    cin >> N;

    for (long i = 1; i <= N; i++) {
        // we discard width
        cin >> width >> t[i];
    }
}

long oblicz() {
    long matches = 0;
    long u[MaxN + 3];
    memset(u, 0, sizeof(u));
    long uc = 0;
    long curHeight;

    for (long i = 1; i <= N; i++) {
        curHeight = t[i];
        while ((1 <= uc) && (curHeight < t[u[uc]]))
            uc--;
        if ((1 <= uc) && (curHeight == t[u[uc]])) {
            matches++;
            uc--;
        }
        u[++uc] = i;
    }

    return N - matches;
}

int main() {
    init();
    czytaj();
    long result = oblicz();
    cout << result << '\n';
    return 0;
}

