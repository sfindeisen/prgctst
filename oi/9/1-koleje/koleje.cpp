#include <algorithm>
#include <iostream>
#include <vector>

const long MaxN =   60000L;
const long lsz  = 0x20000L;

struct tnode {
    long wadd;
    long peak;      // max peak in this range (absolute value)

    tnode() : wadd(0), peak(0) { }
    tnode(const long w, const long p) : wadd(w), peak(p) { }

    void adj_peak(const long hl, const long hr) {
        peak = std::max(hl, hr);
    }
};

/**
 * For node 1<=k:
 *    left child: 2*k
 *   right child: 2*k+1
 */
std::vector<tnode> linia;
std::vector<long> pow2;

long N=0;
long M=0;
long Z=0;

using namespace std;

/**
 * ni     : node index
 * p2     : pow2 index corresponding to ni (extended range length)
 * offset : ni's range is [offset..offset+p2-1]
 * sa     : 0-based stacja poczatkowa (inclusive)
 * sb     : 0-based stacja koncowa    (inclusive)
 * x      : what to add
 */
void dodaj(
    const size_t ni,
    const size_t p2,
    const long offset,
    const long sa,
    const long sb,
    const long x
) {
    const long fi = offset;                             // from inclusive
    const long ti = offset + pow2[p2] - 1;              // to inclusive

    // cerr << "dodaj [" << sa << ".." << sb << "] + " << x << ", ni=" << ni << " p2=" << p2 << " offset=" << offset << endl;

    if ((0 == p2) || ((fi==sa) && (ti==sb))) {
        linia[ni].wadd += x;
        linia[ni].peak += x;
        
        // cerr << "dodaj [" << sa << ".." << sb << "] + " << x << " => [" << fi << ".." << ti << "] w=" << (linia[ni].wadd) << " p=" << (linia[ni].peak) << endl;
    } else {
        const long ro = offset + pow2[p2-1];            // right tree offset

        if (sb < ro) {
            dodaj(2*ni, p2-1, offset, sa, sb, x);       // left tree
        } else if (ro <= sa) {
            dodaj(2*ni+1, p2-1, ro, sa, sb, x);         // right tree
        } else {
            dodaj(2*ni,   p2-1, offset, sa, ro-1, x);   // left tree
            dodaj(2*ni+1, p2-1, ro, ro, sb, x);         // right tree
        }

        linia[ni].adj_peak(
            linia[2*ni  ].peak,
            linia[2*ni+1].peak);
        linia[ni].peak += linia[ni].wadd;
    }
}

void init() {
    long p2 = 1;
    for (; (p2 < N); p2 *= 2)
        pow2.push_back(p2);
    pow2.push_back(p2);
    pow2.push_back(p2 *= 2);

    //cerr << "pow2: ";
    //for (vector<long>::const_iterator it = pow2.begin(); it < pow2.end(); ++it)
    //    cerr << " " << (*it);
    //cerr << endl;

    for (long i=0; i<=p2; ++i)
        linia.push_back(tnode());
}

int main() {
    cin >> N >> M >> Z;

    // cerr << "N=" << N << endl;
    // cerr << "M=" << M << endl;

    init();

    for (long i=0; i<Z; ++i) {
        long a = 0;
        long b = 0;
        long s = 0;

        cin >> a >> b >> s;
        // cerr << "query: [" << (a-1) << ".." << (b-2) << "] + " << s << endl;
        dodaj(1, pow2.size()-1, 0, a-1, b-2, s);

        if (linia[1].peak <= M) {
            cout << "T" << endl;
        } else {
            dodaj(1, pow2.size()-1, 0, a-1, b-2, -s);   // revert
            cout << "N" << endl;
        }

        // for (vector<tnode>::const_iterator it = linia.begin(); it < linia.end(); ++it) {
        //     cerr << "  w=" << (it->wadd) << " p=" << (it->peak) << endl;
        // }
    }

    return 0;
}
