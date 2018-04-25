#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <set>

const long MaxN = 2000000;
      long    N = 0;

char tkst[MaxN + 2];   // text
long tind[MaxN + 2];   // start index
long tlen[MaxN + 2];   // length
long parr[MaxN + 2];   // KMP's P array

std::map<std::string,long> klabsz;  // licznosci poszczegolnych klas abstrakcji
unsigned long long finalResult = 0;

using namespace std;

/** ii is word number */
void tablicaP(const long ii) {
    const long offset = tind[ii];
    const long len    = tlen[ii];

    if (0 == len)
        return;

          long k      = 0;
    parr[offset]      = 0;    // prefix of length 1: P is 0
    //if (ii <= 20)
    //    cerr << "tablicaP: ii=" << ii << ", offset=" << offset << ", len=" << len << std::endl;

    for (long i = 1; i < len; ++i) {
        const char c = tkst[offset + i];
        while (k && (tkst[offset + k] != c))
            k = parr[offset + k - 1];
        if (tkst[offset + k] == c)
            ++k;
        parr[offset + i] = k;

        //if (ii <= 20)
        //    cerr << "    k= " << k << std::endl;
    }
}

void tabliceP() {
    for (long i = 0; i < N; ++i) {
        tablicaP(i);
    }
}

void init() {
    memset(tkst, 0, sizeof(tkst));
    memset(tind, 0, sizeof(tind));
    memset(tlen, 0, sizeof(tlen));
}

void readData() {
    std::string s;
    long k, ii=0;
    cin >> N;

    for (long i = 0; i < N; ++i) {
        cin >> k;
        cin >> s;
        //if (i <= 20)
        //    cerr << "got: " << i << ": k=" << k << " and s=" << s << "#" << std::endl;
        tind[i] = ii;
        tlen[i] = k;
        strncpy(tkst + ii, s.c_str(), k);
        ii += k;
    }
}

void klastruj() {
    std::string atom;

    for (long ii = 0; ii < N; ++ii) {
        atom.clear();

        const long offset  = tind[ii];
        const long len     = tlen[ii];
        const long prfsfln = parr[offset + len - 1];
              long atomLen = len - prfsfln;

        if (((2 * prfsfln) < len) || (len % atomLen)) {
            // cerr << " klastruj: " << ii << ": len=" << len << ", prfsfln=" << prfsfln << " short" << std::endl;
            atomLen = len;
        }

        atom.append(tkst + offset, atomLen);
        std::map<std::string,long>::iterator it = klabsz.find(atom);
        if (klabsz.end() == it) {
            klabsz.insert(pair<std::string,long>(atom,1));
        } else {
            it->second += 1;
        }
    }
}

void oblicz() {
    for (std::map<std::string,long>::const_iterator it = klabsz.begin(); klabsz.end() != it; ++it) {
        const long kasz = (it->second);
        //cerr << "  oblicz: " << (it->first) << " -> " << kasz << std::endl;
        finalResult += (kasz * kasz);
    }
}

int main() {
    init();
    readData();
    tabliceP();
    klastruj();
    oblicz();

    cout << finalResult << std::endl;
    return 0;
}

