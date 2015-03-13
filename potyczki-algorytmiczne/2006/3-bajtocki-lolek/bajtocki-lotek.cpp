/* The official solution or problem statement is buggy, what a shame!! */

// #include <iostream>
#include <cstdio>

using namespace std;

long N = 0;
bool gotnz = false;

inline int F(long i) {
    if (i <= 9)
        return i;
    return F((i / 10) + (i % 10));
}

int main() {
    long j = 0;
    long z = 1;

    // cin >> N;
    scanf("%lu", &N);

    for (long i = 0; i < N; ++i) {
        // cin >> j;
        scanf("%lu", &j);

        if (0 != j) {
            gotnz = true;
            z *= (1+j);
            z = F(z);
        }
    }

    if (gotnz)
        z += 8;   // minus pusty iloczyn
    else
        z = 0;

    // cout << F(z) << endl;
    printf("%u\n", F(z));

    return 0;
}
