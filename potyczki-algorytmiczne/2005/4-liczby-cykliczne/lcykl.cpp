#include <iostream>
#include <set>

typedef unsigned long long ullong;
const int MaxK = 18;
const ullong pow10[1+MaxK] = {                   1ULL,
                                                10ULL,
                                               100ULL,
                                              1000ULL,
                                             10000ULL,
                                            100000ULL,
                                           1000000ULL,
                                          10000000ULL,
                                         100000000ULL,
                                        1000000000ULL,
                                       10000000000ULL,
                                      100000000000ULL,
                                     1000000000000ULL,
                                    10000000000000ULL,
                                   100000000000000ULL,
                                  1000000000000000ULL,
                                 10000000000000000ULL,
                                100000000000000000ULL,
                               1000000000000000000ULL
};

using namespace std;

ullong gcd(const ullong a, const ullong b) {
    if (a < b)
        return gcd(b,a);
    if (0 == b)
        return a;
    return gcd(b, a%b);
}

// dg: number of digits in z (including the leading 0s)
ullong shift_right(const ullong z, const int dg) {
    return (z % 10) * pow10[dg-1] + (z / 10);
}

bool is_cyclic(const ullong query, const unsigned int dg, set<ullong>& family) {
    family.clear();

    if (0 == query)
        return true;

    ullong cur  = query;
    ullong fgd  = query;   // family gcd
    ullong mx   = query;   // family max
    ullong mn   = query;   // family min

    family.insert(query);

    for (unsigned int i=1; i<dg; ++i) {
        cur = shift_right(cur, dg);
        fgd = gcd(fgd, cur);
        if (cur < mn)
            mn = cur;
        if (cur > mx)
            mx = cur;
        if (fgd * dg < mx)
            return false;      // this family gcd is already too small
        family.insert(cur);
    }

    if ((mn <= fgd) && (dg == family.size())) {
        for (unsigned int i=2; i<=dg; ++i)
            if (family.end() == family.find(mn*i))
                return false;
        return true;
    }

    return false;
}

int main() {
    ullong N = 0;
    cin >> N;

    if (N <= 9) {
        cout << N << endl;
        return 0;
    }

    ullong result = pow10[MaxK];

    for (int i=0; i<=MaxK; ++i) {
        const unsigned int dg = i+1;
        cerr << i << " " << pow10[i] << endl;
        const ullong d = ((pow10[i]-1)/dg);
        const ullong r = ((pow10[i]-1)%dg);
        cerr << "  d=" << d << " r=" << r << endl;

        if (0 == r) {
            set<ullong> family;

            if (is_cyclic(d, dg-1, family)) {
                cerr << "CYCLE " << i << " (" << family.size() << "):";
                for (set<ullong>::const_iterator it=family.begin(); it != family.end(); ++it) {
                    cerr << " " << (*it);
                    if ((N <= (*it)) && ((*it) < result))
                        result = (*it);
                }
                cerr << endl;
            }
        }
    }

    cout << result << endl;
    return 0;
}
