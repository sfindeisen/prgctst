#include <iostream>

const int MaxN = 30000;
      int    N =     0;

int t[MaxN + 2];

using namespace std;

void init() {
    memset(t, 0, sizeof(t));
}

void czytaj() {
    cin >> N;

    for (int i = 1; i <= N; i++)
        cin >> t[i];
}

void swap(int i, int j) {
    int x = t[i];
    t[i] = t[j];
    t[j] = x;
}

void qsort(int left, int right) {
    if (left < right) {
        swap(left, (left + right) / 2);
        int m = t[left];
        int k = left;

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

int oblicz() {
    int result = 0;
    int blast  = N;
    int bfirst = N - 2;

    while (3 <= blast) {
        while ((1 <= bfirst) && (t[blast] < (t[bfirst] + t[bfirst + 1])))
            bfirst--;
        result = max(result, blast - bfirst);
        if (! bfirst)
            break;
        blast--;
    }

    return result;
}

int main() {
    init();
    czytaj();
    qsort(1, N);
    int result = oblicz();
    cout << result << '\n';
    return 0;
}

