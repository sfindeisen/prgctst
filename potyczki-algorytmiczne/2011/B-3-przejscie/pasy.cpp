#include <iostream>

#include <set>

typedef long int bigint;

const bigint MaxN = 500000;

using namespace std;

bigint N, Step, Foot;
bigint Space;           // Step - Foot
short ok = 1;

struct TWhitePas {
    bigint first;
    bigint last;

    TWhitePas(const bigint& f, const bigint& l) : first(f), last(l) {
    }

    inline bigint width() const {
        return (1 + last - first);
    }
};

typedef bool(*TPasyCmpFun)(const TWhitePas&,const TWhitePas&);

bool pasyCmp(const TWhitePas& lhs, const TWhitePas& rhs) {
    return ((lhs.first) < (rhs.first));
}

std::set<TWhitePas,TPasyCmpFun> zebra(pasyCmp);
bigint testCount = 0;

void initPhase() {
    N = Step = Foot = 0;
    zebra.clear();
    ok = 1;
}

// pFirst <= pLast
void mergeWh(const bigint& pFirst, const bigint& pLast) {
    set<TWhitePas,TPasyCmpFun>::iterator lo = zebra.lower_bound(TWhitePas(pFirst - Foot, 0));
    bigint newFirst = pFirst;
    bigint newLast  = pLast;

    if (zebra.end() != lo)
        newFirst = min(newFirst, lo->first);

    if (zebra.begin() != lo) {
        --lo;

        if (newFirst <= (lo->last + Foot))
            newFirst = lo->first;
        else
            ++lo;
    }

    set<TWhitePas,TPasyCmpFun>::iterator hi = zebra.upper_bound(TWhitePas(pLast + Foot, 0));

    if (zebra.begin() != hi) {
        --hi;
        if (newLast < (hi->last))
            newLast = (hi->last);
        ++hi;
    }

    zebra.erase(lo, hi);

    TWhitePas pas(newFirst, newLast);
    if (Space < pas.width())
        ok = 0;
    else
        zebra.insert(pas);
}

void readMerge() {
    cin >> Foot >> Step >> N;
    //cerr << "Foot: " << Foot << " Step: " << Step << " N: " << N << endl;
    Space = Step - Foot;

    bigint d = 0;
    bigint k = 0;
    short wh = 1;

    for (bigint i = 0; i < N; ++i, wh = 1 - wh) {
        cin >> k;

        if (wh) {
            if (Space < k)
                ok = 0;
            if (ok) {
                bigint pasEnd = (d + k) % Step;
                if (0 == pasEnd)
                    pasEnd = Step;

                if (d < pasEnd) {
                    mergeWh(d + 1, pasEnd);
                } else {
                    mergeWh(d + 1, Step);
                    if (ok)
                        mergeWh(1, pasEnd);
                }
            }
        }

        d += k;
        d %= Step;
    }
}

void oblicz() {
    bigint off = 0;
    bigint ww  = 0;

    for (set<TWhitePas,TPasyCmpFun>::iterator it = zebra.begin(); it != zebra.end(); ++it) {
        const TWhitePas& pas(*it);
        const bigint bw = pas.first - off - 1;

        if (Foot <= bw) {
            return;
        } else {
            ww += (bw + pas.width());
        }

        if (Space < ww) {
            ok = 0;
            return;
        }

        off = pas.last;
    }
}

void printResult() {
    cout << (ok ? "TAK" : "NIE") << endl;
}

int main() {
    cin >> testCount;

    for (bigint i = 0; i < testCount; ++i) {
        initPhase();
        readMerge();
        if (ok)
            oblicz();
        printResult();
    }

    return 0;
}
