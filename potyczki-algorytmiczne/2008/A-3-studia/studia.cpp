#include <cstring>
#include <iostream>

const long MaxEtapCount = 300 + 1;

typedef struct {
    long long shortestPathLen;  // dlugosc najkrotszej drogi
          int hasPath;          // czy droga w ogole istnieje
} TNodePair;

TNodePair mi[MaxEtapCount + 1][MaxEtapCount + 1];
bool  result[MaxEtapCount + 1];

long    EtapCount = 0;
long PodanieCount = 0;

using namespace std;

/*
void printDebug() {
    printf("========== EtapCount=%ld, PodanieCount=%ld\n", EtapCount, PodanieCount);

    for (int i = 1; i <= EtapCount; i++) {
        for (int j = 1; (j <= EtapCount); j++) {
            bool p1 = mi[i][j].hasPath();
            bool p2 = mi[j][i].hasPath();

            printf(" %2d (%d) ", i, result[i]);

            if (p2)
                printf("<");
            else
                printf("=");
            printf("=");
            if (p1)
                printf(">");
            else
                printf("=");

            printf(" %2d (%d) ", j, result[j]);
            printf("next: %2d; SPL=%ld\n", mi[i][j].next, mi[i][j].shortestPathLen);
        }
    }
}
*/

void init() {
    memset(mi,     0, sizeof(mi));
    memset(result, 0, sizeof(result));
}

void czytaj() {
    long a, b, c;

    cin >> EtapCount >> PodanieCount;

    for (long i = 0; i < PodanieCount; i++) {
        cin >> a >> b >> c;

        TNodePair& pair      = mi[a][b];
        pair.shortestPathLen = (-c);        // odwracamy znaki na krawedziach
        pair.hasPath         = true;
    }
}

void oblicz() {
    // Najkrotsze sciezki proste (lub cos lepszego):
    for (int k = 1; k <= EtapCount; k++) {
        for (int i = 1; i <= EtapCount; i++) {
            for (int j = 1; j <= EtapCount; j++) {
                if ((mi[i][k].hasPath) && (mi[k][j].hasPath)) {
                    const long long lenCombined = mi[i][k].shortestPathLen + mi[k][j].shortestPathLen;
                    TNodePair& pairIJ = mi[i][j];

                    if ((! pairIJ.hasPath) || (lenCombined < pairIJ.shortestPathLen)) {
                        pairIJ.hasPath         = true;
                        pairIJ.shortestPathLen = lenCombined;
                    }
                }
            }
        }
    }

    // cykle bazowe (generatory)
    for (int i = 1; i <= EtapCount; i++) {
        for (int j = 1; j <= EtapCount; j++) {
            if ((mi[i][j].hasPath) && (mi[j][i].hasPath)) {
                const long long cycleLen = mi[i][j].shortestPathLen + mi[j][i].shortestPathLen;
                if (cycleLen < 0) {
                    result[i] = true;
                    result[j] = true;
                }
            }
        }
    }

    //printDebug();

    for (int i = 1; i <= EtapCount; i++) {
        for (int j = 1; (j <= EtapCount); j++) {
            if (result[j] && (mi[i][j].hasPath) && (mi[j][i].hasPath)) {
                // j jest generatorem i mozna pojsc i wrocic
                // wiec i tez jest generatorem
                result[i] = true;
            }
        }
    }

    //printDebug();
}

void wypisz() {
    int k = 0;
    int j = 0;  // max
    for (int i = 1; i <= EtapCount; i++) {
        if (result[i]) {
            k++;
            j=i;
        }
    }

    cout << k << '\n';
    for (int i = 1; i <= EtapCount; i++) {
        if (result[i]) {
            cout << i;
            if (i < j) {
                cout << ' ';
            }
        }
    }

    if (1 <= k)
        cout << '\n';
}

int main() {
    init();
    czytaj();
    oblicz();
    wypisz();
    return 0;
}
