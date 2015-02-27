#include <iostream>
#include <queue>
#include <cstring>

const int MaxN    = 30000;
const int MaxPow2 =    15;        // 2^15 is enough
const int MaxM    =  5000;

const unsigned int Pow2[1 + MaxPow2] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000};

typedef struct {
    int a;
    int b;
} TEdge;

typedef struct {
    unsigned int rdist;                  // distance to root
    int ancestors[1 + MaxPow2];
} TNode;

TEdge edges[2 * MaxN + 3];
TNode nodes[MaxN + 3];
int N = 0;
int e = 0;

using namespace std;

void init() {
    memset(edges, 0, sizeof(edges));
    memset(nodes, 0, sizeof(nodes));
}

inline void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

inline void swapEdges(int a, int b) {
    if (a != b) {
        int tmpa = edges[a].a;
        int tmpb = edges[a].b;
        edges[a].a = edges[b].a;
        edges[a].b = edges[b].b;
        edges[b].a = tmpa;
        edges[b].b = tmpb;
    }
}

void qsort(int left, int right) {
    if (left < right) {
        swapEdges(left, (left + right) / 2);

        int m = edges[left].a;
        int k = left;

        for (int i = left + 1; i <= right; ++i) {
            if (edges[i].a <= m)
                swapEdges(i, ++k);
        }

        swapEdges(left, k);
        qsort(left, k - 1);
        qsort(k + 1, right);
    }
}

int findFirstEdge(int left, int right, int ni) {
    if (left < right) {
        int h  = (left + right) / 2;
        int ha = edges[h].a;

        if (ha < ni)
            return findFirstEdge(h + 1, right, ni);
        else
        if (ha > ni)
            return findFirstEdge(left, h - 1, ni);
        else {
            if ((1 <= h) && (edges[h-1].a == ni))
                return findFirstEdge(left, h - 1, ni);
            else
                return h;
        }
    } else
        return left;        // may be incorrect
}

void buildTree() {
    std::queue<int> nfifo;
    nfifo.push(1);

    nodes[1].ancestors[0] = 0;
    nodes[1].rdist        = 0;

    while (! (nfifo.empty())) {
        const int ni = nfifo.front();
        nfifo.pop();
        const int ei = findFirstEdge(0, e-1, ni);

        for (int i = ei; (i < e) && (ni == edges[i].a); ++i) {
            const int nx = edges[i].b;

            if ((1 != nx) && (0 == nodes[nx].ancestors[0])) {
                nodes[nx].ancestors[0] = ni;
                nodes[nx].rdist        = 1 + nodes[ni].rdist;
                nfifo.push(nx);
            }
        }
    }
}

inline void initAncestorPtr2(int n, int dist2) {
    if (nodes[n].ancestors[dist2])
        return;
    if (nodes[n].rdist < Pow2[dist2])
        return;

    initAncestorPtr2(n, dist2 - 1);
    int h = nodes[n].ancestors[dist2 - 1];
    initAncestorPtr2(h, dist2 - 1);
    nodes[n].ancestors[dist2] = nodes[h].ancestors[dist2 - 1];
    // cerr << "Node " << n << ": ancestors[" << dist2 << "] := " << nodes[h].ancestors[dist2 - 1] << std::endl;
}

int findAncestor(int n, unsigned int dist) {
    // cerr << "  findAncestor (" << n << ", " << dist << ")" << std::endl;

    if (0 == dist)
        return n;
    if (1 == dist)
        return nodes[n].ancestors[0];

    int i = 0;
    for (; (i < MaxPow2) && (Pow2[i+1] <= dist); ++i);
    initAncestorPtr2(n, i);
    return findAncestor(nodes[n].ancestors[i], dist - Pow2[i]);
}

int lowestCommonAncestor(int n1, int n2, int mxp2) {
    if (nodes[n1].rdist > nodes[n2].rdist)
        swap(n1, n2);
    if (nodes[n1].rdist < nodes[n2].rdist) {
        int n2a = findAncestor(n2, nodes[n2].rdist - nodes[n1].rdist);
        // cerr << "  findAncestor (" << n2 << ", " << nodes[n2].rdist - nodes[n1].rdist << ") returned " << n2a << std::endl;
        return lowestCommonAncestor(n1, n2a, mxp2);
    }

    for (int i = mxp2; (0 <= i); --i) {
        initAncestorPtr2(n1, i);
        initAncestorPtr2(n2, i);

        if (nodes[n1].ancestors[i] != nodes[n2].ancestors[i])
            return lowestCommonAncestor(nodes[n1].ancestors[i], nodes[n2].ancestors[i], i);
    }

    return (n1 == n2) ? n1 : (nodes[n1].ancestors[0]);
}

int nodesDistance(int n1, int n2) {
    int lca = lowestCommonAncestor(n1, n2, MaxPow2);
    // cerr << "lca (" << n1 << "," << n2 << ") = " << lca << std::endl;
    return nodes[n1].rdist + nodes[n2].rdist - (2 * nodes[lca].rdist);
}

//void printTree() {
//    for (int i = 0; i <= N; ++i) {
//        cerr << "Node " << i << ": parent=" << nodes[i].ancestors[0] << " rdist=" << nodes[i].rdist << std::endl;
//    }
//}

int main() {
    init();

    int a, b;
    cin >> N;

    for (int i = 1; i < N; ++i) {
        cin >> a >> b;
        // if (a > b) swap(a,b);
        edges[e]  .a = a;
        edges[e++].b = b;
        edges[e]  .a = b;
        edges[e++].b = a;
    }

    qsort(0, e - 1);
    buildTree();
    // printTree();

    // komiwojazer path
    int M = 0;
    int prevCity = 0, city = 0;
    long totalDistance = 0;
    cin >> M;
    if (1 <= M)
        cin >> prevCity;

    for (int i = 1; i < M; ++i) {
        cin >> city;
        int ndist = nodesDistance(prevCity, city);
        // cerr << "distance between " << prevCity << " and " << city << " is " << ndist << std::endl;
        totalDistance += ndist;
        prevCity = city;
    }

    cout << totalDistance << std::endl;
}

