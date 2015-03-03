#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <cstdio>
#include <iomanip>

using namespace std;

//#define DEBUG

const short STATE_VISITED = 0x01;
const short STATE_WSPAK   = 0x02;

struct TNode {
    short state;
    int nr;              // dfs number
    int ns;              // dfs subtree size (including the node)
    int k;               // dfs_forward: dsf parent (1-based); dfs_backward: scc number (root index)
    unsigned int p;      // dfs_*: ein/eou index; final: scc size (unique nodes only) (including the node)
    vector<int> eou;     // indeksy do tablicy nodes
    vector<int> ein;

    inline TNode() : state(0), nr(0), ns(0), k(0), p(0) {
    }

    inline bool visited() const {
        return (STATE_VISITED & state);
    }

    inline bool is_wspak() const {
        return (STATE_WSPAK & state);
    }
};

long n  = 0;
long n2 = 0;
long m  = 0;
//long MaxN =  10000;
//long MaxM = 100000;

vector<TNode> nodes;    // n*wspak + n*normal
stack<int> s;           // indeksy do tablicy nodes
long counter = 0;
vector<int> nodelist;   // indeksy do tablicy nodes
set<int> wns;

#ifdef DEBUG
void debug() {
    cerr << "counter=" << counter << endl;
    for (long i = 0; i < n2; ++i) {
        const TNode& node = nodes[i];
        cerr << setw(3) << i << ": s=" << node.state << " nr=" << setw(3) << node.nr << " ns=" << setw(3) << node.ns << " k=" << setw(3) << node.k << " p=" << setw(3) << node.p << endl;
    }
}
#endif

void add_edge(int i1, int i2) {
    nodes[i1].eou.push_back(i2);
    nodes[i2].ein.push_back(i1);
}

void czytaj() {
    cin >> n >> m;
    // cerr << "n=" << n << " m=" << m << endl;
    n2 = 2*n;

    for (long i = 0; i < n; ++i) {
        TNode d;
        d.state |= STATE_WSPAK;
        nodes.push_back(d);
        nodelist.push_back(i);
    }

    for (long i = 0; i < n; ++i) {
        TNode d;
        nodes.push_back(d);
        nodelist.push_back(i + n);
    }

    for (long j = 0; j < m; ++j) {
        long  a;
        long  b;
        short s;

        cin >> a >> b >> s;

        if (1 == s) {
            add_edge(n+a-1,b-1);
            add_edge(b-1,n+a-1);
        } else {
            add_edge(n+a-1,n+b-1);
            add_edge(b-1,a-1);
        }
    }
}

void dfs_forward() {
    while (! s.empty()) {
        const int j = s.top();
        TNode& node = nodes[j];

        if (! node.visited()) {
#ifdef DEBUG
            cerr << "FWD: " << setw(3) << j << "  parent: " << setw(3) << (node.k-1) << endl;
#endif
            node.state |= STATE_VISITED;
            node.nr = counter++;
            node.ns = 1;
        }

        bool godeep = false;
        while (node.p < node.eou.size()) {
            const int k = node.eou[node.p++];
#ifdef DEBUG
            cerr << "    k=" << k << endl;
#endif

            if (! nodes[k].visited()) {
#ifdef DEBUG
                cerr << "    go deep: " << k << endl;
#endif
                nodes[k].k = 1+j;
                s.push(k);
                godeep = true;
                break;
            }
        }

        if (! godeep) {
            if (node.k)
                nodes[node.k-1].ns += node.ns;
            s.pop();
        }
    }
}

void dfs_backward(int r) {
    const TNode& root = nodes[r];

    while (! s.empty()) {
        const int j = s.top();
        TNode& node = nodes[j];

        if (! node.visited()) {
#ifdef DEBUG
            cerr << "BKW: " << setw(3) << j << endl;
#endif
            node.state |= STATE_VISITED;
            node.k = r;
        }

        bool godeep = false;
        while (node.p < node.ein.size()) {
            const int k = node.ein[node.p++];
#ifdef DEBUG
            cerr << "    k=" << k << endl;
#endif
            const TNode& node2 = nodes[k];

            if ((! node2.visited()) && (root.nr <= node2.nr) && (node2.nr <= (root.nr + root.ns - 1))) {
#ifdef DEBUG
                cerr << "    go deep: " << k << endl;
#endif
                s.push(k);
                godeep = true;
                break;
            }
        }

        if (! godeep) {
            s.pop();
        }
    }
}

bool cscc (int i, int j) {
    return (nodes[i].k < nodes[j].k);
}

bool cnr (int i, int j) {
    return (nodes[i].nr < nodes[j].nr);
}

void oblicz() {
    // go forward
    counter = 1;
    for (long i = 0; i < n2; ++i) {
        if (! nodes[i].visited()) {
#ifdef DEBUG
            cerr << "FORWARD root=" << setw(3) << i << " nr=" << setw(3) << nodes[i].nr << " ns=" << setw(3) << nodes[i].ns << endl;
#endif
            s.push(i);
            dfs_forward();
        }
    }

#ifdef DEBUG
    debug();
#endif

    // sort according to nr
    sort (nodelist.begin(), nodelist.end(), cnr);
 
    // clean
    for (long i = 0; i < n2; ++i) {
        nodes[i].state &= (~STATE_VISITED);
        nodes[i].k = 0;
        nodes[i].p = 0;
    }

    // go backward
    for (long i = 0; i < n2; ++i) {
        const int k = nodelist[i];

        if (! nodes[k].visited()) {
#ifdef DEBUG
            cerr << "BACKWARD root=" << setw(3) << k << " nr=" << setw(3) << nodes[k].nr << " ns=" << setw(3) << nodes[k].ns << endl;
#endif
            s.push(k);
            dfs_backward(k);
        }
    }

#ifdef DEBUG
    debug();
#endif

    // oblicz wynik
    sort (nodelist.begin(), nodelist.end(), cscc);
    int k = nodes[nodelist[0]].k;

    for (long i = 0; i < n2; ++i) {
        const int wi = nodelist[i];
        const TNode& node = nodes[wi];

#ifdef DEBUG
        cerr << "  k: " << setw(3) << k << "  wi: " << setw(3) << wi << "  scc: " << setw(2) << node.k << endl;
#endif

        if (k == node.k) {
            if (wi < n) {
                wns.insert(wi);
            } else {
                wns.insert(wi - n);
            }
        } else {
            for (long j = i-1; 0 <= j; --j) {
                TNode& node2 = nodes[nodelist[j]];
                if (k == node2.k)
                    node2.p = wns.size();
                else
                    break;
            }

            k = node.k;
            wns.clear();
            --i;
        }
    }

    for (long j = n2-1; 0 <= j; --j) {
        TNode& node2 = nodes[nodelist[j]];
        if (k == node2.k)
            node2.p = wns.size();
        else
            break;
    }

#ifdef DEBUG
    debug();
#endif
}

void wypisz() {
    for (long i = 0; i < n; ++i) {
        printf("%d\n", nodes[i].p - 1);
    }
}

int main() {
    czytaj();
    oblicz();
    wypisz();
    return 0;
}
