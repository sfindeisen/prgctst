/**
 *  Przekatne
 *
 *  \date   2008-05-16
 *  \author STF
 */

#include <iostream>

const long MaxNodes     =  1000000;
const long MaxNodesPow2 = 0x100000; // 1048576
const long MaxDiags     = 10000000;

typedef struct {
    long maxEnd;              // 0 if none
    long maxEndIndex;         // pierwotny indeks przekatnej; 0 if none
    long minBegin;
    long minBeginIndex;       // pierwotny indeks przekatnej; 0 if none

    bool hasMaxEnd() {
        return maxEndIndex;
    }
    bool hasMinBegin() {
        return minBeginIndex;
    }
} TNode;

typedef struct {
    long rangeFrom;             // inclusive
    long rangeTo;               // inclusive
    long childLeft;
    long childRight;

    long minBeginNodeIndex;     // numer wierzcholka z najmniejszym minBegin;
                                // UWAGA: ten wierzcholek moze nie miec wchodzacych przekatnych!

    bool hasLeftChild() {
        return childLeft;
    }
    bool hasRightChild() {
        return childRight;
    }
} TRangeSummary;

TNode          t[MaxNodes + 2];
TRangeSummary rs[2 * MaxNodesPow2 + 2];

long NodeC = 0;
long DiagC = 0;

using namespace std;

void init() {
    memset( t,  0, sizeof(t));
    memset(rs,  0, sizeof(rs));

    // inicjalizacja wartownikow
    for (long i = 0; i < MaxNodes + 2; i++) {
        t[i].minBegin = (MaxNodesPow2 + 2);
    }
}

void czytaj() {
    long a, b, c;

    cin >> NodeC >> DiagC;

    for (long i = 1; i <= DiagC; i++) {
        cin >> a >> b;
        if (b < a) {
            c = a;
            a = b;
            b = c;
        }

        // krawedz a --> b
        //cout << a << " --> " << b << '\n';

        if (t[a].maxEnd < b) {
            t[a].maxEnd      = b;
            t[a].maxEndIndex = i;
        }

        if (a < t[b].minBegin) {
            t[b].minBegin      = a;
            t[b].minBeginIndex = i;
        }
    }
}

inline long smaller_minBegin(long n1, long n2) {
    if (t[n1].minBegin <= t[n2].minBegin)
        return n1;
    else
        return n2;
}

/** returns number of TRangeSummary records written */
long init_range_summary(long writeFrom, long rangeFrom, long rangeTo) {
    if (rangeFrom <= rangeTo) {
        rs[writeFrom].rangeFrom = rangeFrom;
        rs[writeFrom].rangeTo   = rangeTo;

        if (rangeFrom == rangeTo) {
            rs[writeFrom].minBeginNodeIndex = rangeFrom;
            //printf(" LEAF init_range_summary: %2ld .. %2ld -> %2ld (%2ld)\n", rangeFrom, rangeTo, rangeFrom, t[rangeFrom].minBegin);
            return 1;
        } else {
            long    rangeH = (rangeFrom + rangeTo) / 2;
            long  leftRecs = init_range_summary(writeFrom + 1,            rangeFrom,  rangeH);
            long rightRecs = init_range_summary(writeFrom + 1 + leftRecs, rangeH + 1, rangeTo);
            long    minBNI = rangeFrom; // minBeginNodeIndex

            if (1 <= leftRecs) {
                rs[writeFrom].childLeft =    writeFrom + 1;
                minBNI                  = rs[writeFrom + 1].minBeginNodeIndex;
            }
            if (1 <= rightRecs) {
                long          childRight = writeFrom + 1 + leftRecs;
                rs[writeFrom].childRight = childRight;
                long         minBNIRight = rs[childRight].minBeginNodeIndex;

                minBNI = smaller_minBegin(minBNI, minBNIRight);
            }

            rs[writeFrom].minBeginNodeIndex = minBNI;
            //printf("      init_range_summary: %2ld .. %2ld -> %2ld (%2ld)\n", rangeFrom, rangeTo, minBNI, t[minBNI].minBegin);
            return 1 + leftRecs + rightRecs;
        }
    }

    return 0;
}

/** queryFrom and queryTo are both inclusive */
long find_min_begin(long rsIndex, long queryFrom, long queryTo) {
    const TRangeSummary& smr = rs[rsIndex];
    long              minBNI = smr.minBeginNodeIndex;

    // exact match:
    if ((queryFrom == smr.rangeFrom) && (queryTo == smr.rangeTo)) {
        //printf("EXACT find_min_begin[%2ld] [%2ld..%2ld] (%2ld..%2ld) = %2ld (%2ld)\n", rsIndex, smr.rangeFrom, smr.rangeTo, queryFrom, queryTo, minBNI, t[minBNI].minBegin);
        return minBNI;
    }

    if (smr.rangeFrom < smr.rangeTo) {
        // This is not a leaf, so it has 2 children!
        long rangeH = rs[smr.childLeft].rangeTo;

        if (queryTo <= rangeH)
            minBNI = find_min_begin(smr.childLeft, queryFrom, queryTo);
        else if (rangeH < queryFrom)
            minBNI = find_min_begin(smr.childRight, queryFrom, queryTo);
        else {
            long minBNILeft  = find_min_begin(smr.childLeft,  queryFrom,  rangeH);
            long minBNIRight = find_min_begin(smr.childRight, rangeH + 1, queryTo);
            minBNI = smaller_minBegin(minBNILeft, minBNIRight);
        }
    }

    //printf("      find_min_begin[%2ld] [%2ld..%2ld] (%2ld..%2ld) = %2ld (%2ld)\n", rsIndex, smr.rangeFrom, smr.rangeTo, queryFrom, queryTo, minBNI, t[minBNI].minBegin);
    return minBNI;
}

void output(long i, long j) {
    cout << i << ' ' << j << '\n';
}

void mieszaj() {
    init_range_summary(0, 1, NodeC);

    for (long i = 1; i <= NodeC; i++) {
        if (t[i].hasMaxEnd()) {
            //printf("======== krawedz: %2ld --> %2ld\n", i, t[i].maxEnd);
            long c = find_min_begin(0, i + 1, t[i].maxEnd - 1);
            if (t[c].minBegin < i) {
                // got it!  :-)
                output(t[i].maxEndIndex, t[c].minBeginIndex);
                return;
            }
        }
    }

    cout << "NIE\n";
}

int main() {
    init();
    czytaj();
    mieszaj();

    return 0;
}
