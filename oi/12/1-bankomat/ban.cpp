/**
 *   2007-11-19
 *   STF
 */

#include <iostream>

using namespace std;

const int MaxScen = 1000;
const int MaxScenaLen = 10000;
const int NotFound = -1;

typedef struct {
    long dlugosc;
    char sekwencja[MaxScenaLen + 1];
} TScena;

// [scena][cyfra] - pierwsze wystapienie cyfry w tej scenie
int first[MaxScen + 1][10];

// [scena][cyfra] - ostatnie wystapienie cyfry w tej scenie
int  last[MaxScen + 1][10];

// [scena][cyfra1][cyfra2] - pierwsze wystapienie cyfry2 w tej scenie takie ze
// gdzies przed nim (nieostro) wystepuje cyfra1
int firstAfter[MaxScen + 1][10][10];

// [scena][cyfra1][cyfra2] - ostatnie wystapienie cyfry2 w tej scenie takie ze
// gdzies po nim (nieostro) wystepuje jeszcze cyfra1
int lastBefore[MaxScen + 1][10][10];

int liczbaScen = 0;

// liczba pasujacych PINow (wynik)
int matchingCodeCount = 0;

void wypisz_wynik() {
    // cout << "WYNIK KONCOWY: ";
    cout << matchingCodeCount << '\n';
}

void czytaj_scene(TScena * scena) {
    cin >> scena->dlugosc;

    if (0 == scena->dlugosc)
        return;

    char c = 0;
    do {
        cin >> c;
    } while ((c < '0') || (c > '9'));
    scena->sekwencja[0] = c - '0';

    for (int j = 1; j < scena->dlugosc; j++) {
        cin >> c;
        scena->sekwencja[j] = c - '0';
    }
}

int czytaj_wejscie_i_wypelniaj_tabelki() {
    cin >> liczbaScen;

    TScena scena;

    for (int index = 0; index < liczbaScen; index++) {
        memset(&scena, 0, sizeof(scena));
        czytaj_scene(&scena);

        if (0 == scena.dlugosc) {
            // no matching codes at all
            return -1;
        }

        for (int d1 = 0; d1 <= 9; d1++) {
            first[index][d1] = NotFound;
             last[index][d1] = NotFound;

            for (int d2 = 0; d2 <= 9; d2++) {
                firstAfter[index][d1][d2] = NotFound;
                lastBefore[index][d1][d2] = NotFound;
            }
        }

        // jedziemy od poczatku do konca; wypelniamy last i firstAfter
        for (int j = 0; j < scena.dlugosc; j++) {
            int digit = scena.sekwencja[j];
            last[index][digit] = j;
            for (int dp = 0; dp <= 9; dp++) {
                if ((NotFound != last[index][dp]) && (NotFound == firstAfter[index][dp][digit])) {
                    firstAfter[index][dp][digit] = j;
                }
            }
        }

        // jedziemy od konca do poczatku; wypelniamy first i lastBefore
        for (int j = scena.dlugosc - 1; 0 <= j; j--) {
            int digit = scena.sekwencja[j];
            first[index][digit] = j;
            for (int dp = 0; dp <= 9; dp++) {
                if ((NotFound != first[index][dp]) && (NotFound == lastBefore[index][dp][digit])) {
                    lastBefore[index][dp][digit] = j;
                }
            }
        }
    }

    return 0;
}

void sprawdz_kody() {
    for (int d1 = 0; d1 <= 9; d1++) {
        for (int d2 = 0; d2 <= 9; d2++) {
            for (int d3 = 0; d3 <= 9; d3++) {
                for (int d4 = 0; d4 <= 9; d4++) {
                    bool ok = true;
                    for (int index = 0; ok && (index < liczbaScen); index++) {
                        int i1 = first[index][d1];
                        int i2 = firstAfter[index][d1][d2];
                        int i3 = lastBefore[index][d4][d3];
                        int i4 = last[index][d4];

                        if ((NotFound == i1) || (NotFound == i2) || (NotFound == i3) || (NotFound == i4))
                            ok = false;
                        if (! ((i1 <= i2) && (i2 <= i3) && (i3 <= i4)))
                            ok = false;
                    }

                    if (ok) {
                        // PIN found!
                        // cout << "PIN: " << d1 << " " << d2 << " " << d3 << " " << d4 << "\n";
                        matchingCodeCount++;
                    }
                }
            }
        }
    }
}

int main() {
    memset(first, 0, sizeof(first));
    memset(last,  0, sizeof(last));
    memset(firstAfter, 0, sizeof(firstAfter));
    memset(lastBefore, 0, sizeof(lastBefore));

    int i = czytaj_wejscie_i_wypelniaj_tabelki();
    if (0 <= i)
        sprawdz_kody();
    wypisz_wynik();

    return 0;
}
