/**
 *   2007-12-04
 *   STF
 *
 *   Algorytm: dla poziomych ulic pamietamy drzewo binarne zrownowazone (?)
 *             w ktorym w lisciach na dole sa poziome ulice (po kolei) a w wezlach sa max()
 *             po liczbach pasazerow ktorych w sumie mozna zebrac startujac z jakiegos
 *             przystanku tej ulicy.
 *
 *             Poczatkowo drzewo jest wyzerowane.
 *
 *             Jedziemy od prawej do lewej i od gory do dolu (po kolejnych przystankach)
 *             szukajac w drzewie ulicy z najlepszym przystankiem i ktora zarazem jest osiagalna.
 *             Po znalezieniu ulicy przystanek moze przewiezc wlasnie tyle pasazerow ile bylo
 *             max dla ulicy + to co ma on sam.
 *             Nowy max poziomej ulicy przystanku to rowniez jest ta suma.
 */

#include <iostream>

/* Numery lisci w drzewie liczone sa od lewej, od 1-ki. */

typedef struct {
    long x;
    long y;
    long p;
    long leafNumber; // numer liscia w drzewie
} TSkrzyzowanie;

typedef struct {
    long maxPassengers;
    long leafNumber; // numer liscia w poddrzewie ktory daje najwieksza liczbe pasazerow (globalny)
} TNode;

const long MaxN = 1000000000;
const long MaxM = 1000000000;
const long MaxK = 100000 + 2; // 2 rezerwowe na poczatek i na koniec
const long MaxTreeBase = 0x1FFFF; // najwieksza potega 2 wieksza lub rowna MaxK; jest to licznosc podstawy drzewa, ktore bedzie zawierac fantomowe wezly
const long MaxTreeSize = (2 * MaxTreeBase) - 1;

long N = 0;
long M = 0;
long K = 0;

TSkrzyzowanie skrzyzowania[MaxK + 1];

TNode drzewo[MaxTreeSize];
long leafCount = 0;
int leafCountRUP = 0; // sufit liczby lisci (potega 2) (wykladnik)
long leafOffset = 0; // liczba wezlow w drzewie ktore lisciami nie sa

const int Pow2Size = 20;
long pow2[Pow2Size];

long Result = 0;

using namespace std;

void zamien(TSkrzyzowanie * t, long i, long j) {
    TSkrzyzowanie tmp;
    memcpy(&tmp, t + i, sizeof(TSkrzyzowanie));
    memcpy(t + i, t + j, sizeof(TSkrzyzowanie));
    memcpy(t + j, &tmp, sizeof(TSkrzyzowanie));

/*    TSkrzyzowanie* ti = t[i];
    TSkrzyzowanie* tj = t[j];

    ulong x = ti.x;
    ulong y = ti.y;
    ulong p = ti.p;
    int treeLeafIndex = ti.treeLeafIndex;

    ti.x = tj.x;
    ti.y = tj.y;
    ti.p = tj.p;
    ti.treeLeafIndex = tj.treeLeafIndex;

    tj.x = x;
    tj.y = y;
    tj.p = p;
    tj.treeLeafIndex = treeLeafIndex;
*/
}

void quickSort(TSkrzyzowanie t[], long left, long right, int (*cmp)(TSkrzyzowanie * p1, TSkrzyzowanie * p2)) {
    if (left >= right)
        return;

    long middle = (left + right) / 2;
    long partElem = middle;

    int clr = (*cmp)(t + left,   t + right);
    int clm = (*cmp)(t + left,   t + middle);
    int cmr = (*cmp)(t + middle, t + right);

    if ((clm <= 0) && (cmr >= 0)) {
        if (clr <= 0)
            partElem = right;
        else
            partElem = left;
    } else if ((clm >= 0) && (cmr <= 0)) {
        if (clr <= 0)
            partElem = left;
        else
            partElem = right;
    }

    if (left != partElem)
        zamien(t, left, partElem);

    long rb = right + 1;

    for (long i = left + 1; i < rb; ) {
        int c = (*cmp)(t + i, t + left);
        if (c <= 0) {
            i++;
        } else {
            // moze byc zamiana w miejscu
            zamien(t, i, (--rb));
        }
    }
    zamien(t, left, rb - 1);

    quickSort(t, left, rb - 2, cmp);
    quickSort(t, rb, right, cmp);
}

int cmp(long a, long b) {
    if (a < b)
        return -1;
    if (b < a)
        return +1;
    return 0;
}

int cmp_y_sort(TSkrzyzowanie * p1, TSkrzyzowanie * p2) {
    return cmp(p1->y, p2->y);
}

int cmp_xy_sort(TSkrzyzowanie * p1, TSkrzyzowanie * p2) {
    long x1 = p1->x;
    long x2 = p2->x;

    if (x1 < x2)
        return -1;
    if (x2 < x1)
        return +1;
    return cmp(p1->y, p2->y);
}

void sortuj_po_y() {
    quickSort(skrzyzowania, 0, K - 1, &cmp_y_sort);
}

void sortuj_po_xy() {
    quickSort(skrzyzowania, 0, K - 1, &cmp_xy_sort);
}

long inline leafIndexToNumber(long leafIndex) {
    return leafIndex - leafOffset;
}

long inline leafNumberToIndex(long leafNumber) {
    return leafOffset + leafNumber;
}

// Tablica skrzyzowania jest posortowana po Y
void buduj_drzewo() {
    long currY = -1;
    long j = 0;  // liczba roznych Y

    for (long i = 0; i < K; i++) {
        if (currY < skrzyzowania[i].y) {
            currY = skrzyzowania[i].y;
            j++;
        }
    }
    leafCount = j;

    // calculate leaf offset
    int k = 1;
    while (pow2[k] < leafCount)
        k++;
    leafOffset = pow2[k] - 1;
    leafCountRUP = k;

    //cout << "leafCount=" << leafCount << ", leafOffset=" << leafOffset << ", leafCountRUP=" << leafCountRUP << "\n";

    // wpisz numery lisci do skrzyzowan
    currY = - 1;
    long leafNumber = 0;
    for (long i = 0; i < K; i++) {
        if (currY < skrzyzowania[i].y) {
            currY = skrzyzowania[i].y;
            leafNumber++;
        }
        skrzyzowania[i].leafNumber = leafNumber;
    }
}

// dodaje do drzewa
void ustaw(long leafNumber, long maxPassengers) {
    long index = leafNumberToIndex(leafNumber);

    while ((index >= 1) && (maxPassengers > drzewo[index].maxPassengers)) {
        drzewo[index].maxPassengers = maxPassengers;
        drzewo[index].leafNumber = leafNumber;
        index >>= 1;    // go to parent
    }
}

// nodeIndex to index biezacego wezla (korzenia poddrzewa w ktorym szukamy)
// plonSize to liczba lisci w poddrzewie (zaokraglone w gore) (wykladnik 2)
// leafNumber to numer liscia (w poddrzewie!) wzgledem ktorego szukamy
long szukaj(long nodeIndex, int plonSize, long leafNumber) {

    //cout << "szukaj: root=" << nodeIndex << ", plonSz=" << plonSize << ", leafNumber=" << leafNumber << "\n";

    if (plonSize == 0) // drzewo 1-elementowe
        return nodeIndex;

    // children indexes:
    long lci = nodeIndex << 1;
    long rci = lci + 1;

    if (leafNumber > pow2[plonSize - 1]) {
        return szukaj(rci, plonSize - 1, leafNumber - pow2[plonSize - 1]);
    } else {
        long leftLeafIndex = szukaj(lci, plonSize - 1, leafNumber);
        if (drzewo[leftLeafIndex].maxPassengers > drzewo[rci].maxPassengers)
            return leftLeafIndex;
        return leafNumberToIndex(drzewo[rci].leafNumber);
    }
}

// Zwraca numer liscia z najlepszym wynikiem i takiego, ze jest
// nieostro powyzej (Y) danego.
long szukaj(long leafNumber) {
    long leafIndex = szukaj(1, leafCountRUP, leafNumber);
    if (leafOffset < leafIndex)
        return leafIndexToNumber(leafIndex);
    return 0; // not found!
}

// tablica skrzyzowania jest posortowana po X,Y
void policz() {
    for (long i = K-1; 0 <= i; i--) {
        //cout << "policz: x=" << skrzyzowania[i].x << " y=" << skrzyzowania[i].y << "\n";

        long leafNumber = skrzyzowania[i].leafNumber;
        //cout << "policz: leafNumber=" << leafNumber << "\n";
        long stopNumber = szukaj(leafNumber);
        //cout << "policz: stopNumber=" << stopNumber << "\n";

        long maxPassengers = skrzyzowania[i].p;
        if (1 <= stopNumber)
            maxPassengers += drzewo[leafNumberToIndex(stopNumber)].maxPassengers;

        //cout << "policz: maxPassengers=" << maxPassengers << "\n";
        ustaw(leafNumber, maxPassengers);

        if (maxPassengers > Result)
            Result = maxPassengers;
    }
}

void wypisz_wynik() {
    // cout << drzewo[leafNumberToIndex(skrzyzowania[0].leafNumber)].maxPassengers << "\n";
    cout << Result << '\n';
}

void wczytaj_dane() {
    cin >> N >> M >> K;
    //cout << "N=" << N << ", M=" << M << ", K=" << K << "\n";

    // long index_11 = -1;
    // long index_NM = -1;

    long x, y, p;
    for (long i = 0; i < K; i++) {
        cin >> x >> y >> p;
        skrzyzowania[i].x = x;
        skrzyzowania[i].y = y;
        skrzyzowania[i].p = p;

        //if ((1 == x) && (1 == y))
        //    index_11 = i;
        //if ((N == x) && (M == y))
        //    index_NM = i;
    }

    // dodajemy sztuczne skrzyzowania:
/*    if (index_11 < 0) {
        skrzyzowania[K].x = 1;
        skrzyzowania[K].y = 1;
        skrzyzowania[K].p = 0;
        K++;
    }
*/
/*    if (index_NM < 0) {
        skrzyzowania[K].x = N;
        skrzyzowania[K].y = M;
        skrzyzowania[K].p = 0;
        K++;
    }
*/
}

void init() {
    memset(skrzyzowania, 0, sizeof(skrzyzowania));
    memset(drzewo, 0, sizeof(drzewo));

    long p2 = 1;
    for (int i = 0; i < Pow2Size; i++, (p2 <<= 1)) {
        pow2[i] = p2;
    }
}

int main() {
    init();

    wczytaj_dane();
    sortuj_po_y();
    buduj_drzewo();

    sortuj_po_xy();
    policz();
    wypisz_wynik();

    return 0;
}
