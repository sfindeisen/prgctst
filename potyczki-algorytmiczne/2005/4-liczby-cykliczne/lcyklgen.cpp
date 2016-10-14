#include <iostream>
#include <set>  // C++11's unordered_set is unavailable as of this writing

typedef unsigned long long ullong;
const int MaxK = 18;
const ullong pow10[1+MaxK] = { 1ULL,
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

// returns the number of digits
unsigned int digits(const ullong z) {
    int a = 0;
    int b = MaxK;
    int c = (a+b)/2;

    for (; (a<b); c=(a+b)/2) {
        if (z < pow10[c]) {
            b = c;
        } else if (a==c) {
            c = ((z == pow10[b]) ? b : a);
            break;
        } else {
            a = c;
        }
    }

    return 1+c;
}

// dg: number of digits in z (including the leading 0s)
ullong shift_right(const ullong z, const int dg) {
    return (z % 10) * pow10[dg-1] + (z / 10);
}
//ullong shift_right(const ullong z) {
//    return shift_right(z, digits(z));
//}

// we assume no leading 0s: works for the largest number in the cyclic family
bool is_cyclic(const ullong z, set<ullong>& akk) {
    akk.clear();

    if (0 == z)
        return true;

    const unsigned int dg = digits(z);
    ullong x = z;
    ullong d = z;   // gcd
    ullong m = z;   // min
    akk.insert(z);

    for (unsigned int i=1; i<dg; ++i) {
        x = shift_right(x, dg);
        d = gcd(d, x);
        if (x < m)
            m = x;
        if (d*dg < z) {
            // gcd is too small
            return false;
        }
        akk.insert(x);

        // if (857142==z) {
        //     cout << "z=" << z << " x=" << x << " d=" << d << " m=" << m << endl;
        // }
    }

    if ((m <= d) && (dg == akk.size())) {
        for (unsigned int i=2; i<=dg; ++i)
            if (akk.end() == akk.find(m*i))
                return false;
        return true;
    }

    return false;

/*
    const ullong zmax = dg * z;
    const int zmaxdg = digits(zmax);

    if (dg < zmaxdg)
        return false;
    if (! eq_cyclic(z, zmax))
        return false;
    for (int i=2; i<dg; ++i) {
        if (! eq_cyclic(z, i*z))
            return false;
    }
*/
}

int main() {
    for (int i=0; i<=MaxK; ++i) {
        cout << i << " " << pow10[i] << endl;
        cout << digits(pow10[i]) << endl;
    }

    set<ullong> family;

    ullong xfrom;
    cin >> xfrom;
    cout << "Starting from: " << xfrom << endl;

    for (ullong i=xfrom; i<=pow10[MaxK]; ++i) {
        if (is_cyclic(i, family)) {
            bool is_max = ((! family.empty()) && (i == *family.rbegin()));
            cout << "CYCLE " << i << " (";
            if (is_max)
                cout << family.size();
            else
                cout << "-";
            cout << "):";
            for (set<ullong>::const_iterator it=family.begin(); it != family.end(); ++it) {
                cout << " " << (*it);
            }
            cout << endl;
        } else if (0 == (i % 10000000)) {
            cout << "i=" << i << endl;
        }
    }

/*

    shift_right(5);
    shift_right(15);
    shift_right(51);
    shift_right(451);
    shift_right(230);
    shift_right(23);
    shift_right(23, 1);
    shift_right(23, 2);
    shift_right(23, 3);
    shift_right(23, 4);
    shift_right(23, 5);
    shift_right(4507);
*/

    return 0;
}
