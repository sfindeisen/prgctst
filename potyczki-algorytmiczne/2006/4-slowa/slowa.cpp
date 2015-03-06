#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cstring>

using namespace std;

const int AlphaSZ =   26;
const int MaxN    = 1000;
const int MaxK    = MaxN;

struct TBlok {
    char c;
    int len;

    TBlok() : c(0), len(0) {
    }
};

struct TFunItem {
    int left;
    int right;
    int value;

    TFunItem() : left(0), right(0), value(0) {
    }

    TFunItem(int l, int r, int v) : left(l), right(r), value(v) {
        // cerr << "TFunItem: [" << l << ".." << r << "] -> " << v << endl;
    }

    inline int width() {
        return (1+right-left);
    }
};

// value(x) = vo + v(x-ho)
struct TFun {
    list<TFunItem> m;             // liczba niezgodnosci -> liczba blokow
    int ho;                       // horizontal offset
    int vo;                       // vertical offset

    TFun() : ho(0), vo(0) {
        m.push_back(TFunItem(0,MaxK,0));
    }

    void minimize(const TFun& another, bool createMissing) {
        if (m.empty()) {
            if (createMissing) {
                m  = another.m;
                ho = another.ho;
                vo = another.vo;
            }
            return;
        }

        const list<TFunItem>& a = another.m;
        list<TFunItem>::iterator it1 = m.begin();
        list<TFunItem>::const_iterator it2 = a.begin();

        if (createMissing) {
            // create missing portion of the domain in this
            const int start1 = ho + it1->left;

            for (; (it2 != a.end()) && (another.ho + (it2->right) < start1); ++it2) {
                TFunItem item;
                item.left  = another.ho + it2->left  - ho;
                item.right = another.ho + it2->right - ho;
                item.value = another.vo + it2->value - vo;
                m.insert(it1, item);
            }

            if ((it2 != a.end()) && ((another.ho + (it2->left)) < start1)) {
                if ((another.vo + (it2->value)) == (vo + (it1->value))) {
                    it1->left = another.ho + it2->left - ho;
                } else {
                    TFunItem item;
                    item.left  = another.ho + it2->left - ho;
                    item.right = start1 - 1 - ho;
                    item.value = another.vo + it2->value - vo;
                    m.insert(it1, item);
                }
            }
        }

        while ((it1 != m.end()) && (it2 != a.end())) {
            for (; (it1 != m.end()) && ((ho + it1->right) < (another.ho + it2->left)); ++it1);

            if (it1 == m.end())
                break;

            const int lv = another.vo + it2->value;
            if (vo + (it1->value) <= lv) {
                ++it2;
                continue;
            }

            if ((another.ho + (it2->right)) < (ho + (it1->right))) {
                if ((another.ho + it2->right) < (ho + it1->left)) {
                    // no intersection
                    ++it2;
                } else if ((ho + it1->left) < (another.ho + it2->left)) {
                    // it1 contains it2
                    TFunItem item(*it1);
                    it1->left  = another.ho + it2->left - ho;
                    item.right = another.ho + it2->left - ho - 1;
                    item.value = lv - vo;
                    m.insert(it1++, item);
                } else {
                    TFunItem item(*it1);
                    it1->left  = another.ho + it2->right - ho + 1;
                    item.right = another.ho + it2->right - ho;
                    item.value = lv - vo;
                    m.insert(it1++, item);
                }
            } else {
                if ((ho + (it1->right)) < (another.ho + (it2->left))) {
                    // no intersection
                    ++it1;
                } else if ((ho + (it1->left)) < (another.ho + (it2->left))) {
                    TFunItem item(*it1);
                    it1->value  = lv - vo;
                    it1->left   = another.ho + it2->left - ho;
                    item.right  = another.ho + it2->left - ho - 1;
                    m.insert(it1++, item);
                } else {
                    // it2 contains it1
                    it1->value = lv - vo;
                    ++it1;
                }
            }
        }
    }

    void cuttail(int cutoff) {
        list<TFunItem>::reverse_iterator it = m.rbegin();
        for (; ((it != m.rend()) && (cutoff < (ho + it->left))); ++it);
        m.erase(it.base(), m.end());
        if ((it != m.rend()) && (cutoff < (ho + it->right))) {
            it->right = cutoff - ho;
        }
    }

    /* void print() const {
        for (list<TFunItem>::const_iterator it = m.begin(); it != m.end(); ++it) {
            cerr << "  " << (ho + (it->left)) << " .. " << (ho + (it->right)) << " -> " << (vo + (it->value)) << endl;
        }
    } */
};

int k, n;
string slowo;
vector<TBlok> bloki;
TFun rozw[AlphaSZ];
TFun minr;

void kompresuj() {
    for (int i = 0; i < n;) {
        int j = 0;
        for (j = i+1; (j < n) && (slowo[i] == slowo[j]); ++j);

        TBlok blok;
        blok.c   = slowo[i];
        blok.len = j-i;

        bloki.push_back(blok);
        i = j;
    }
}

void oblicz() {
    for (int i = 0; i < bloki.size(); ++i) {
        const TBlok& blok = bloki[i];
        const int ci = blok.c - 'A';
        const int BL = blok.len;
        cerr << "==== blok: " << BL << " * " << blok.c << " (ci=" << ci << ")" << endl;

        // biezaca literka
        TFun& curr = rozw[ci];

        // cerr << "minr (in):" << endl;
        // minr.print();
        // cerr << "curr (in):" << endl;
        // curr.print();

        if (0 == i) {
            curr.vo = 1;
        } else {
            minr.vo += 1;
            curr.minimize(minr, true);
            minr.vo -= 1;
        }

        // cerr << "curr (out):" << endl;
        // curr.print();

        // pozostale literki
        for (int j = 0; j < AlphaSZ; ++j) {
            if (j != ci) {
                if (0 == i)
                    rozw[j].vo = 1;

                rozw[j].ho += BL;
                rozw[j].cuttail(k);
            }

            // cerr << "literka " << char('A' + j) << ": " << endl;
            // rozw[j].print();
        }

        // update min
        if (0 == i) {
            minr.vo = 1;
        } else {
            minr.ho += BL;
            minr.cuttail(k);
            // cerr << "minr (cuttail):" << endl;
            // minr.print();
            minr.minimize(curr, true);
        }

        // cerr << "minr (out):" << endl;
        // minr.print();
    }
}

void wypisz() {
    int t = n;
    const int ho = minr.ho;
    const int vo = minr.vo;

    for (list<TFunItem>::const_iterator it = minr.m.begin(); it != minr.m.end(); ++it) {
        if (k < (ho + (it->left)))
            break;
        t = min(t, vo + (it->value));
    }

    cout << t << endl;
}

int main() {
    cin >> n >> k;
    cin >> slowo;

    if (n != slowo.length()) {
        cerr << "n=" << n << " slowo.len=" << slowo.length() << " slowo=" << slowo << endl;
        return 0;
    }

    kompresuj();
    oblicz();
    wypisz();
}
