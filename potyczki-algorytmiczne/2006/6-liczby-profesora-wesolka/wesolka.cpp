#include <cstdio>

using namespace std;

typedef unsigned long long ULL;

ULL p;
ULL e;
int k;
ULL n;

ULL gcd(ULL a, ULL b) {
    if (b < a)
        return gcd(b, a);
    if (0 == a)
        return b;
    return gcd(b % a, a);
}

/* n^w mod p */
ULL pown(ULL w) {
    ULL akk = 1;
    ULL p2  = 2;
    ULL w2  = 1;
    ULL sq  = n % p;

    for (; 1 <= w; ++w2, p2*=2, sq=(sq*sq)%p) {
        ULL r = w % p2;
        if (r) {
            akk *= sq;
            akk %= p;
            w -= r;
        }
    }

    return akk;
}

int main() {
    scanf("%llu %llu", &p, &e);
    scanf("%d", &k);

    for (int i = 0; i < k; ++i) {
        scanf("%llu", &n);
        ULL wyk = (p-1)/gcd(e,p-1);
        bool b = ((1 == pown(wyk)) ? true : false);
        printf(b ? "TAK\n" : "NIE\n");
    }

    return 0;
}
