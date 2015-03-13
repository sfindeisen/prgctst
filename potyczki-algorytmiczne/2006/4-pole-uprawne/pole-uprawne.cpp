#include <algorithm>
//#include <iostream>
#include <cstdio>
#include <queue>
#include <functional>
#include <vector>

using namespace std;

struct TOdcinek {
    long l;
    long r;

    TOdcinek() : l(0), r(0) {
    }

    TOdcinek(long a, long b) : l(a), r(b) {
    }

    inline long width() const {
        return (r+1-l);
    }
};

const long MaxN = 5000000;
long a, b, c, d;
long long res = 0;
TOdcinek plansza[MaxN];

struct cmpl {
    bool operator()(const long &a, const long &b) const {
        return (plansza[a].l < plansza[b].l);
    }
};

struct cmpr {
    bool operator()(const long &a, const long &b) const {
        return (plansza[b].r < plansza[a].r);
    }
};

priority_queue<long, vector<long>, cmpl> ql;
priority_queue<long, vector<long>, cmpr> qr;

void init() {
    priority_queue<long, vector<long>, cmpl> qle;
    priority_queue<long, vector<long>, cmpr> qre;

    swap(qr, qre);
    swap(ql, qle);
    
    //while (! ql.empty())
    //    ql.pop();
    //while (! qr.empty())
    //    qr.pop();
}

void oblicz() {
    //priority_queue<long, vector<long>, greater<long> > qle;
    //swap(ql, qle);
    //priority_queue<long, vector<long>, less<long> > qre;
    //swap(qr, qre);

    if ((a < c) || (b < d))
        return;

    init();
    long first=0;

    for (long i = 0; i < b; ++i) {
        const TOdcinek& odc = plansza[i];
        // cerr << "i=" << i << " odc: " << odc.l << ".." << odc.r << endl;

        if ((odc.width()) < c) {
            init();
            first = 1+i;
            continue;
        }

        ql.push(i);
        qr.push(i);

        while ((first + d - 1 <= i) && (! ql.empty()) && (! qr.empty())) {
            const long li = ql.top();
            const long ri = qr.top();

            // cerr << "    li: " << li << " ri: " << ri << endl;

            if (li < first) {
                ql.pop();
                continue;
            }

            if (ri < first) {
                qr.pop();
                continue;
            }

            if (plansza[ri].r < (plansza[li].l + c - 1)) {
                // too narrow!
                first = 1 + min(li,ri);
                // cerr << "    too narrow! first=" << first << endl;
            } else {
                // match!
                const long w = (plansza[ri].r - plansza[li].l - c + 2);
                res += w;
                ++first;
                // cerr << " ## match! w=" << w << endl;
            }
        }
    }
}

void wczytaj() {
    scanf("%lu %lu %lu %lu", &a, &b, &c, &d);
    // cerr << "a=" << a << " b=" << b << " c=" << c << " d=" << d << endl;
    long x,y;

    for (long i = 0; i < b; ++i) {
        scanf("%lu %lu", &x, &y);
        // cerr << "x=" << x << " y=" << y << endl;

        plansza[i].l = x-1;
        plansza[i].r = x+y-2;

        // cerr << "i: " << i << " odc: " << plansza[i].l << ".." << plansza[i].r << endl;
    }
}

int main() {
    wczytaj();
    oblicz();

    if (c != d) {
        swap(c,d);
        oblicz();
    }

    printf("%lld\n", res);
    return 0;
}
