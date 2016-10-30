#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>

const int MaxN   =  5000;   // maximal number of generators
const int MaxGen = 50000;   // maximal generator

long N;                     // number of generators
long K;                     // number of queries
long G;                     // gcd of the generators
char buffer[1+MaxGen];      // working buffer

std::vector<long> gen;      // generators
std::vector<long> qvec;     // queries
std::vector<long> qi;       // queries (sorted indexes)
std::vector<long> qans;     // answers

using namespace std;

long gcd(long a, long b) {
    if (a < b)
        return gcd(b,a);
    if (0 == b)
        return a;
    return gcd(b, a % b);
}

void print_answers() {
    for (long i=0; i<K; ++i) {
        cout << (qans[i] ? "TAK" : "NIE") << endl;
    }
}

void init_data() {
    memset(buffer, 0, sizeof(buffer));
}

void read_data() {
    long j;

    cin >> N;
    for (long i=0; i<N; ++i) {
        cin >> j;
        gen.push_back(j);
    }

    cin >> K;
    for (long i=0; i<K; ++i) {
        cin >> j;
        qvec.push_back(j);
    }
}

// for sorting queries
bool mycmp (long i, long j) {
    return (qvec[i] < qvec[j]);
}

void compute() {
    // 1. compute gcd
    for (long i=0; i<N; ++i)
        G = ((0==i) ? gen[i] : gcd(gen[i],G));
    for (long i=0; i<N; ++i)
        gen[i] /= G;

    // 2. sort the queries
    for (long i=0; i<K; ++i) {
        qi.push_back(i);
        qans.push_back(0);
    }
    sort (qi.begin(), qi.end(), mycmp);

    // 3. iterate over queries in sorted order
    const long blen = gen[N-1];     // length of the buffer
    buffer[0] = 1;                  // 0 is visited (can be generated)
    long cur  = 0;                  // current number
    long bcur = 0;                  // cur % blen

    for (long i=0; i<K; ++i) {
        long query = qvec[qi[i]];
        // cerr << "query=" << query << endl;
        if (query % G)
            continue;
        query /= G;
        const long x = query % blen;

        while ((! buffer[x]) && (cur < query)) {
            if (blen == ++bcur)
                bcur = 0;
            ++cur;

            for (int j=N-2; (0<=j) && (! buffer[bcur]); --j) {
                long z = blen + bcur - gen[j];
                if (blen <= z)
                    z -= blen;
                if (buffer[z]) {
                    buffer[bcur] = 1;
                    // cerr << "query=" << query << " cur=" << cur << endl;
                }
            }
        }

        if (buffer[x]) {
            // cerr << "    yes" << endl;
            qans[qi[i]] = 1;
        }
    }
}

int main() {
    init_data();
    read_data();
    compute();
    print_answers();
    return 0;
}
