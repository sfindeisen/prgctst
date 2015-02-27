/**
 *   2007-12-04
 *   STF
 */

#include <iostream>

const int MaxN =  50000;
const int MaxW = 100000;

// szeregi zolnierzy
long sz[MaxN + 2][2];

// wystapienia liczb (wzrostow) w szeregach
long wwi[MaxW + 2][2]; // wzrost -> index
char wws[MaxW + 2][2]; // wzrost -> ktory szereg (0/1)

// visited cycles
char cc[MaxN + 2];

long N = 0;
long Result = 0;

using namespace std;

void init() {
    memset(sz,  0, sizeof(sz));
    memset(wwi, 0, sizeof(wwi));
    memset(wws, 0, sizeof(wws));
    memset(cc,  0, sizeof(cc));
}

long min(long a, long b) {
    return (a <= b) ? a : b;
}

void czytaj_szereg(int si) {
    long w = 0;

    for (long i = 1; i <= N; i++) {
        cin >> w;
        sz[i][si] = w;

        if (wwi[w][0]) {
            wwi[w][1] = i;
            wws[w][1] = si;
        } else {
            wwi[w][0] = i;
            wws[w][0] = si;
        }
    }
}

void czytaj() {
    cin >> N;

    czytaj_szereg(0);
    czytaj_szereg(1);
}

void licz_cykl_dir(long start, int dir, long res[2]) {
    // cout << "Licz cykl: start=" << start << ", dir=" << dir << '\n';

    long turns    = 0;
    long cycleLen = 1;
    cc[start]     = 1;  // we count the first element

    // liczba ktora znalezlismy poprzednio (i do ktorej teraz linkujemy):
    long prevIndex  = start;
    int  prevScN    = dir;    // ktory szereg - poczatkowo
    int  prevScR    = dir;    // ktory szereg - po ewentualnym odwroceniu

    bool continueCycle = true;
    while (continueCycle) {
        long prevHeight = sz[prevIndex][prevScN];

        // Find next cycle element:
        long ji = wwi[prevHeight][0];
         int jc = wws[prevHeight][0];

        if ((ji == prevIndex) && (jc == prevScN)) {
            // try another occurence
            ji = wwi[prevHeight][1];
            jc = wws[prevHeight][1];
        }

        if (1 <= ji) {
            // found another occurence
            // cout << "    occ: " << ji << " " << jc << " (" << prevHeight << "); turns=" << turns << ", cLen=" << cycleLen << "\n";

            if (cc[ji]) {
                // cycle end
                continueCycle = false;
            } else {
                cc[ji] = 1;
                cycleLen++;

                bool turned = false;
                if (jc == prevScR) {
                    // ten sam szereg - trzeba odwrocic
                    turns++;
                    turned = true;
                }

                // bedziemy szukac po drugiej liczbie z tej pary:
                prevIndex  = ji;
                prevScN    = 1 - jc;
                prevScR    = turned ? jc : prevScN;
            }
        } else {
            // occurence not found, cycle end
            continueCycle = false;
        }
    }

    res[0] = cycleLen;
    res[1] = turns;
}

void zeruj(long t[2]) {
    t[0] = 0;
    t[1] = 0;
}

long licz_cykl(long start) {
    // cout << "Licz cykl: " << start << '\n';

    long cycleStats[2];
    zeruj(cycleStats);
    licz_cykl_dir(start, 0, cycleStats);

    long cycleLen = cycleStats[0];
    long turns    = cycleStats[1];

    zeruj(cycleStats);
    licz_cykl_dir(start, 1, cycleStats);

    cycleLen += cycleStats[0] - 1;
    turns    += cycleStats[1];

    long result = min(turns, cycleLen - turns);
    // cout << "Licz cykl: " << start << " -> " << turns << ", " << cycleLen << " -> " << result << '\n';
    return result;
}

void licz() {
    for (long i = 1; i <= N; i++) {
        if (! cc[i]) {
            Result += licz_cykl(i);
        }
    }
}

void pisz() {
    cout << Result << '\n';
}

int main() {
    init();
    czytaj();
    licz();
    pisz();

    return 0;
}
