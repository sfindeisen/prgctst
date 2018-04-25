#include <cstdio>
#include <functional>
#include <queue>
#include <map>
#include <vector>

class mycmp {
    bool reverse;

    public:
        mycmp(const bool& revparam=false) : reverse(revparam) {
        }

        bool operator() (const int& lhs, const int&rhs) const {
            return (reverse ? (lhs>rhs) : (lhs<rhs));
        }
};

typedef std::priority_queue<long, std::vector<long>, mycmp> PQUEUE;

using namespace std;

long fetch_next(PQUEUE& que, map<long,long>& mp) {
    while (1) {
        const long k = que.top();
        que.pop();

        // update the map
        map<long,long>::iterator it = mp.find(k);
        if ((mp.end() == it) || (0 == it->second))
            return k;
        --(it->second);
    }
}

void incr(map<long,long>& mp, const long key) {
    map<long,long>::iterator it = mp.find(key);
    if (mp.end() == it) {
        mp[key] = 1;
    } else {
        ++(it->second);
    }
}

int main() {
    int N = 0;
    scanf("%d", &N);

    PQUEUE loq(mycmp(true));
    PQUEUE hiq;
    map<long,long> lov;
    map<long,long> hiv;
    long smax = 0;
    long smin = 0;

    for (int i=0; i<N; ++i) {
        long k = 0;
        scanf("%ld", &k);

        for (long j=0; j<k; ++j) {
            long x = 0;
            scanf("%ld", &x);
            loq.push(x);
            hiq.push(x);
        }

        const long hi = fetch_next(hiq, lov);
        const long lo = fetch_next(loq, hiv);
        smax += hi;
        smin += lo;
        incr(hiv,hi);
        incr(lov,lo);
    }

    printf("%ld\n", smax-smin);
    return 0;
}
