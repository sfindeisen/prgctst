#include <cstdio>

long czytaj_i_licz() {
    long N = 0;
    char c = 0;
    bool r = true;      // poprzedni spinacz byl prawy

    scanf("%lu", &N);

    for (long spinacz = 0; spinacz < N; ) {
        scanf("%c", &c);

        switch (c) {
            case 'A':   // typ: lewy
            case 'C':
                spinacz++;
                if (! r)
                    return (N - 1);
                r = false;
                break;
            case 'B':   // typ: prawy
            case 'D':
                spinacz++;
                if (r)
                    return (N - 1);
                r = true;
                break;
            default:
                break;
        }
    }

    return (r ? N : (N-1));
}

int main() {
    long res = czytaj_i_licz();
    printf("%lu\n", res);
    return 0;
}
