#include <algorithm>
#include <cstdio>
#include <set>
#include <queue>
#include <vector>
#include <iostream>

const long MaxN = 50000;
const long MaxM = 50000;

long n=0;
long m=0;
long k=0;
long root=0;

struct TNode {
    long parent;        // parent
    long dist;          // distance to root
    long depth;         // distance to the deepest zuczek in bfs subtree (-1 if no zuczek)
    std::set<long> adj;
    char zuczek;

    TNode() : parent(0), dist(0), depth(-1), zuczek(0) {
    }

    inline void add_adj(const long k) {
        adj.insert(k);
    }
};

std::vector<TNode> nodes;
std::vector<long> zuczki;
char gotz1 = 0;
long res   = 100 + MaxN;

void calc_dist() {
    std::queue<long> q;
    q.push(root);

    while (! q.empty()) {
        const long k = q.front();
        q.pop();
        const TNode& knode(nodes[k]);

        for (std::set<long>::const_iterator it = knode.adj.begin(); it != knode.adj.end(); ++it) {
            TNode& n2(nodes[*it]);

            if ((root != (*it)) && (0 == n2.parent)) {
                n2.parent = k;
                n2.dist   = 1 + knode.dist;

                if ((n2.zuczek) && (n2.dist % 2)) {
                    gotz1 = 1;
                    return;
                }

                // std::cerr << "node " << (*it) << " has parent: " << (n2.parent) << " dist: " << n2.dist << std::endl;
                q.push(*it);
            }
        }
    }
}

void calc_depth(long k) {
    TNode& knode(nodes[k]);
    long ld = 0;
    char gz = 0;

    // std::cerr << "calc_depth: k=" << k << std::endl;

    for (std::set<long>::const_iterator it = knode.adj.begin(); it != knode.adj.end(); ++it) {
        // std::cerr << "  it: " << (*it) << std::endl;
        if (*it != knode.parent) {
            calc_depth(*it);
            ld = std::max(ld, nodes[*it].depth);
            if (nodes[*it].zuczek)
                gz = 1;
        }
    }

    if (gz || (1 <= ld)) {
        knode.depth = 1+ld;
    } else if (knode.zuczek) {
        knode.depth = 0;
    }

    // std::cerr << "node " << k << " gz: " << int(gz) << " ld: " << ld << " depth: " << (knode.depth) << std::endl;
}

int move_root() {
    long r     = root;
    long pplen = 0;

    while (1) {
        // std::cerr << "new root: " << r << std::endl;

        const TNode& rnode(nodes[r]);
        long d = -1;
        long c = -1;    // 2nd depth
        long p = -1;

        // select a child with the deepest zuczek
        for (std::set<long>::const_iterator it = rnode.adj.begin(); it != rnode.adj.end(); ++it) {
            if ((*it != rnode.parent) && (c < nodes[*it].depth)) {
                if (d < nodes[*it].depth) {
                    c = d;
                    d = nodes[*it].depth;
                    p = *it;
                } else {
                    c = nodes[*it].depth;
                }
            }
        }

        // init
        if (root==r)
            res = std::min(res, 1+d);

        // std::cerr << "r=" << r << " pplen=" << pplen << " c=" << c << " d=" << d << " p=" << p << std::endl;

        // descend
        if (((1+pplen) <= d) && ((c < 0) || ((2+c) <= d))) {
            pplen = ((0 <= c) ? std::max(1+pplen,2+c) : (1+pplen));
            res = std::min(res, std::max(pplen,d));
            // std::cerr << "res= " << res << std::endl;
            r = p;
        } else
            break;
    }
 
    return res;
}

int oblicz() {
    if (k <= 1)
        return 0;

    calc_dist();

    if (gotz1)
        return -1;

    calc_depth(root);
    return move_root();
}

void wczytaj() {
    scanf("%ld %ld", &n, &m);

    for (long i=0; i<=n; ++i)
        nodes.push_back(TNode());

    for (long i=0; i<m; ++i) {
        long a=0;
        long b=0;

        scanf("%ld %ld", &a, &b);

        nodes[a].add_adj(b);
        nodes[b].add_adj(a);
    }

    scanf("%ld", &k);

    for (long i=0; i<k; ++i) {
        long a=0;
        scanf("%ld", &a);
        nodes[a].zuczek = 1;
        zuczki.push_back(a);

        if (0==i)
            root = a;
    }
}

int main() {
    wczytaj();
    long res = oblicz();

    if (res < 0)
        printf("NIE\n");
    else
        printf("%ld\n", res);

    return 0;
}
