#include <cstring>

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>

typedef long int bigint;

const bigint MaxN =    500000;
//const bigint MaxE = 2 * (MaxN - 1);
const bigint MaxP = 200000000;

struct TEdge {
    bigint k1;
    bigint k2;

    TEdge(const bigint& a, const bigint& b) : k1(a), k2(b) {
    }

    bool operator<(const TEdge& another) const {
        return ((k1 < another.k1) || ((k1 == another.k1) && (k2 < another.k2)));
    }
};

struct TCity {
    bigint parent;
};

bigint N = 0;
bigint P = 0;
bigint diam = 0;

std::vector<TEdge> edges;
bigint nodeStart[MaxN + 2];
TCity cities[MaxN + 2];

using namespace std;

void init() {
    memset(nodeStart, 0, sizeof(nodeStart));
    memset(cities, 0, sizeof(cities));
}

void czytaj() {
    cin >> N >> P;
    bigint a, b;

    for (bigint i = 1; i < N; ++i) {
        cin >> a >> b;
        edges.push_back(TEdge(a,b));
        edges.push_back(TEdge(b,a));
    }
}

void bfs() {
    queue<bigint> q;
    q.push(1);

    while (! q.empty()) {
        const bigint& cur(q.front());
        const bigint& par(cities[cur].parent);
        bigint k = nodeStart[cur];

        for (bigint i = k; cur == edges[i].k1; ++i) {
            if (par != edges[i].k2) {
                cities[edges[i].k2].parent = cur;
                q.push(edges[i].k2);
            }
        }

        q.pop();
    }
}

bigint longestPath(const bigint k) {
    bigint max1 = -1;
    bigint max2 = -1;

    for (bigint i = nodeStart[k]; k == edges[i].k1; ++i) {
        if (cities[k].parent != edges[i].k2) {
            bigint p = longestPath(edges[i].k2);
            if (max1 <= p) {
                max2 = max1;
                max1 = p;
            } else if (p > max2) {
                max2 = p;
            }
        }
    }

    diam = max(diam, 2 + max1 + max2);
    return 1 + max1;
}

void oblicz() {
    std::sort(edges.begin(), edges.end());

    const bigint sz = edges.size();
    for (bigint i = 1; i < sz; ++i) {
        if (edges[i-1].k1 != edges[i].k1) {
            nodeStart[edges[i].k1] = i;
        }
    }

    bfs();
    longestPath(1);

    bigint res = 1;
    if (P <= diam) {
        res = 1 + P;
    } else {
        res = 1 + diam + (P - diam) / 2;
    }
    if (res > N)
        res = N;
    cout << res << endl;
}

int main() {
    init();
    czytaj();
    oblicz();

    return 0;
}
