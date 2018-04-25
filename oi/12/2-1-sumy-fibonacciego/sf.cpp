#include <iostream>
#include <vector>

typedef int Int;
typedef long long Long;
typedef std::vector<Int> VInt;

#define ssize(c) ((Long)((c).size()))

using namespace std;

void main_start() {
    ios_base::sync_with_stdio(0);
}

VInt f1, f2, fsum;

VInt wczytaj() {
    int i=0, k=0;
    cin >> i;
    VInt v;

    for (int j=0; j<i; ++j) {
        cin >> k;
        v.push_back(k);
    }

    return v;
}

void wypisz(const VInt& fibo) {
    int k = fibo.size()-1;
    for (; (0 <= k) && (0 == fibo[k]); --k);
    if (k < 0)
        cout << "1 0";
    else {
        cout << (k+1);
        for (int i=0; i<=k; ++i)
            cout << " " << fibo[i];
    }
    cout << endl;
}

// eliminacja 2
// kazda 2 jest otoczona z obydwu stron 0 (poza byc moze najmniej znaczaca)
void elim2(VInt& fibo) {
    for (int i=0; i < ssize(fibo)-1; ++i) {
        if (2 == fibo[i]) {
            if (0 == i) {
                // 0 2 $
                fibo[1] = 1;
                fibo[0] = 0;
            } else {
                // 0 2 ..
                fibo[i+1] += 1;
                fibo[i]    = 1;
                fibo[i-1] -= 1;
            }
        }
    }
}

// eliminacja -1
// nie ma dwoch -1 obok siebie
// na lewo od kazdej -1 stoi +1
void elim1(VInt& fibo) {
    for (int i=ssize(fibo)-2; 0<=i; --i) {
        if (-1 == fibo[i]) {
            if (0 == fibo[i+1]) {
                // 1 0 -1
                fibo[i+2] = 0;
                fibo[i+1] = 1;
                fibo[i]   = 0;
            } else {
                // 1 -1
                if (0 == i) {
                    // 1 -1 $
                    fibo[1] = 0;
                    fibo[0] = 1;
                } else {
                    // 1 -1 ..
                    if (1 == fibo[i-1]) {
                        // 1 -1 1
                        if (1 == i) {
                            // 1 -1 1 $
                            fibo[2] = 0;
                            fibo[1] = 1;
                            fibo[0] = 0;
                        } else {
                            // 1 -1 1 ..
                            fibo[i+1]  = 0;
                            fibo[i]    = 1;
                            fibo[i-1]  = 1;
                            fibo[i-2] -= 1;
                        }
                    } else {
                        // 1 -1 0
                        fibo[i+1] = 0;
                        fibo[i]   = 0;
                        fibo[i-1] = 1;
                    }
                }
            }
        }
    }
}

void zwin(VInt& fibo) {
    const int z = fibo.size() - 1;

    for (int i=z; 1<=i; --i) {
        for (int j=i; ((j<z) && (1==fibo[j]) && (1==fibo[j-1])); j+=2) {
            fibo[j+1] = 1;
            fibo[j]   = 0;
            fibo[j-1] = 0;
        }
    }
}

int main() {
    main_start();

    f1 = wczytaj();
    f2 = wczytaj();

    const int sz = max(ssize(f1), ssize(f2));
    fsum.assign(sz+5, 0);
    for (int i=0; i<sz; ++i)
        fsum[i] = ((i<ssize(f1)) ? f1[i] : 0) + ((i<ssize(f2)) ? f2[i] : 0);
    elim2(fsum);
    elim1(fsum);
    zwin(fsum);

    wypisz(fsum);
    return 0;
}
