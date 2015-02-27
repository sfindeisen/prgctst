#include <iostream>
#include <cstring>
#include <string>

const long MaxN = 500000;

char tekst[MaxN + 3];
long ptab [MaxN + 3];           // KMP "P array": ptab[i] is max k<i : there is prefix/suffix of length k in tekst[1..i]
long N = 0;

typedef struct {
    long prev, next;
} TListItem;

TListItem candidates[MaxN + 3];
long      candFirst = 0;

using namespace std;

void init() {
    memset(tekst,      0, sizeof(tekst));
    memset(ptab,       0, sizeof(ptab));
    memset(candidates, 0, sizeof(candidates));
}

/** tekst must be 1-based string! (tekst[1], tekst[2]...) */
void tablicaP() {
    long k  = 0;
    ptab[0] = 0;
    ptab[1] = 0;

    for (long i = 2; i <= N; ++i) {
        const char c = tekst[i];
        while (k && (tekst[k+1] != c))
            k = ptab[k];
        if (tekst[k+1] == c)
            ++k;
        ptab[i] = k;
        //cerr << "tabp: " << i << " => " << k << std::endl;
    }
}

void removeCandidate(long p) {
    //cerr << "remove candidate: " << p << std::endl;
    const long pn = candidates[p].next;
    const long pp = candidates[p].prev;

    if (p == candFirst) {
        candFirst = pn;
    } else {
        candidates[pp].next = pn;
    }

    if (pn)
        candidates[pn].prev = pp;

    candidates[p].prev = 0;
    candidates[p].next = 0;
}

/** cand1 spans cand2; must be cand1 < cand2 */
short spans(long cand1, long cand2) {
          long c1off   = ptab[cand1];                      // this many chars is already matched in c1
          long c2off   = ptab[cand2] - c1off;              // occurence candidate offset
          long lastOcc = ptab[cand2] + 1 - cand1;          // last occurence of cand1 in cand2 (so far)
    const long occGoal = cand2 + 1 - ptab[cand2] - cand1;  // if we reach this, we are OK
    //cerr << "spans: " << cand1 << ", " << cand2 << ", lastOcc=" << lastOcc << ", occGoal=" << occGoal << std::endl;

    while ((lastOcc < occGoal) && (c2off < (cand2 - ptab[cand2]))) {
        //cerr << "  - spans: c1off=" << c1off << ", c2off=" << c2off << std::endl;

        long i = 0;
        for (i = c1off + 1; (i <= cand1) && (tekst[i] == tekst[c2off + i]); ++i);
        --i;        // length of c1 prefix that is matched
        //cerr << "  i = " << i << std::endl;

        if (cand1 <= i) {
            //cerr << cand1 << " occurs in " << cand2 << " at pos: " << (c2off + 1) << std::endl;

            if ((lastOcc + cand1) < (c2off + 1)) {
                //cerr << cand1 << " gaps   in " << cand2 << std::endl;
                return 0;
            }

            lastOcc = c2off + 1;
        }

        if (0 == i)
            ++i;

        c1off  = ptab[i];
        c2off += i - c1off;
    }

    //cerr << "  + spans: c1off=" << c1off << ", c2off=" << c2off << std::endl;
    return (occGoal <= lastOcc);
}

long findMinSpan() {
    while (candidates[candFirst].next) {
        const long candNext = candidates[candFirst].next;

        if (spans(candFirst, candNext)) {
            removeCandidate(candNext);
        } else {
            removeCandidate(candFirst);
        }
    }

    return candFirst;
}

void initCandidates() {
    long nextCand = 0;

    for (long p = N; p; p = ptab[p]) {
        //cerr << "candidate: " << p << std::endl;
        if (nextCand)
            candidates[nextCand].prev = p;
        candidates[p].next = nextCand;
        nextCand = p;
    }

    candFirst = nextCand;
}

void readData() {
    cin >> (1 + tekst);
    N = strlen(1 + tekst);
    //cerr << "napis to: " << (1 + tekst) << " (" << N << ")#" << std::endl;
}

int main() {
    init();
    readData();
    tablicaP();
    initCandidates();

    const long res = findMinSpan();
    cout << res << std::endl;
}

