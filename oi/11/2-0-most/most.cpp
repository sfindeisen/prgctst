#include <iostream>

const long MaxN = 100000;
      long N = 0;

long t[MaxN + 1];

using namespace std;

void init() {
    memset(t, 0, sizeof(t));
}

void czytaj() {
    cin >> N;
    for (long i = 0; i < N; i++) {
        cin >> t[i];
    }
}

/*
void swap(long i, long j) {
    long x = t[i];
    t[i] = t[j];
    t[j] = x;
}

void qsort(long left, long right) {
    if (left < right) {
        swap(left, (left + right) / 2);
        long m = t[left];
        long k = left;

        for (long i = left + 1; i <= right; i++) {
            if (t[i] <= m) {
                swap(i, ++k);
            }
        }

        swap(left, k);
        qsort(left, k - 1);
        qsort(k + 1, right);
    }
}
*/

long oblicz() {
    switch (N) {
        case 0: return 0;
        case 1: return t[0];
        case 2: return t[1];
        case 3: return (t[0] + t[1] + t[2]);
        default:
            /* qsort(0, N - 1);
            cout << "SORTED: " << endl;
            for (long z = 0; z < N; z++) {
                cout << t[z] << " ";
            }
            cout << endl;
            */

            long  mr = t[0] + t[1] + t[1];
            long  m2 = t[0] + t[0];
            long res = 0;
            long   j = N - 2;
            for (; j >= 2; j -= 2) {
                res += (min(m2 + t[j], mr) + t[j+1]);
                // cout << "Kolejny cykl (" << t[j] << "," << t[j+1] << "): res=" << res << endl;
            }
            if (0 == j)
                res += t[1];
            if (1 == j)
                res += t[0] + t[1] + t[2];
            // cout << "Koniec: res=" << res << endl;
            return res;
    }
}

int main() {
    init();
    czytaj();

    cout << oblicz() << endl;
    return 0;
}

