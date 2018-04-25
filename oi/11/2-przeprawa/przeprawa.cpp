#include <iostream>

const unsigned long pow2[] = { 0x1,     0x2,     0x4,     0x8,
                               0x10,    0x20,    0x40,    0x80,
                               0x100,   0x200,   0x400,   0x800,
                               0x1000,  0x2000,  0x4000,  0x8000,
                               0x10000, 0x20000, 0x40000, 0x80000 };

const int MaxN      = 16;
const int MaxT      = 50;
const int MaxTTotal = MaxT * MaxN;

int N = 0;
int W = 0;

typedef unsigned long tbitmap;       // sluzy do okreslania podzbiorow wszystkich bajtolazow
tbitmap mask_all = 0;                // zbior wszystkich bajtolazow

typedef struct {
    int waga;
    int czas;
} TBajtolaz;

TBajtolaz bd[MaxN + 1];   // podstawowe dane o bajtolazach
int t[0x10000 + 1];       // zbior bajtolazow --> minimalny czas przeprawy

using namespace std;

void init1() {
    memset(bd, 0, sizeof(bd));
    memset(t,  0, sizeof(t));
}

void czytaj() {
    // cout << "sizeof(bd)=" << sizeof(bd) << "\n";
    cin >> W >> N;
    // cout << "W=" << W << ", N=" << N << "\n";

    for (int i = 0; i < N; i++) {
        cin >> bd[i].czas >> bd[i].waga;
        // cout << "  bajtolaz " << (i+1) << ": czas=" << bd[i].czas << ", waga=" << bd[i].waga << '\n';
    }
}

void init2() {
    mask_all = pow2[N] - 1;
    // cout << "mask_all=" << hex << mask_all << "\n";
    // cout << dec;
}

inline int max_czas(tbitmap grono) {
    int result = 0;
    for (int i = 0; i < N; i++) {
        if (grono & pow2[i]) {
            if (result < bd[i].czas)
                result = bd[i].czas;
        }
    }
    return result;
}

inline int suma_wag(tbitmap grono) {
    int result = 0;
    for (int i = 0; i < N; i++) {
        if (grono & pow2[i])
            result += bd[i].waga;
    }
    // cout << hex << grono << ": suma_wag = " << dec << result << '\n';
    return result;
}

int oblicz(tbitmap grono);

inline void gen_1_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    if (x) {
        tbest = min(tbest, oblicz(x) + oblicz(grono & (~x)));
    }

    tbitmap g1 = (x | pow2[jed[0]]);
    if (g1 != grono) {
        tbest = min(tbest, oblicz(g1) + oblicz(grono & (~g1)));
    }
};

inline void gen_2_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_1_pref(grono, jed, tbest, x);
    gen_1_pref(grono, jed, tbest, x | pow2[jed[1]]);
}

inline void gen_3_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_2_pref(grono, jed, tbest, x);
    gen_2_pref(grono, jed, tbest, x | pow2[jed[2]]);
}

inline void gen_4_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_3_pref(grono, jed, tbest, x);
    gen_3_pref(grono, jed, tbest, x | pow2[jed[3]]);
}

inline void gen_5_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_4_pref(grono, jed, tbest, x);
    gen_4_pref(grono, jed, tbest, x | pow2[jed[4]]);
}

inline void gen_6_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_5_pref(grono, jed, tbest, x);
    gen_5_pref(grono, jed, tbest, x | pow2[jed[5]]);
}

inline void gen_7_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_6_pref(grono, jed, tbest, x);
    gen_6_pref(grono, jed, tbest, x | pow2[jed[6]]);
}

inline void gen_8_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_7_pref(grono, jed, tbest, x);
    gen_7_pref(grono, jed, tbest, x | pow2[jed[7]]);
}

inline void gen_9_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_8_pref(grono, jed, tbest, x);
    gen_8_pref(grono, jed, tbest, x | pow2[jed[8]]);
}

inline void gen_10_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_9_pref(grono, jed, tbest, x);
    gen_9_pref(grono, jed, tbest, x | pow2[jed[9]]);
}

inline void gen_11_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_10_pref(grono, jed, tbest, x);
    gen_10_pref(grono, jed, tbest, x | pow2[jed[10]]);
}

inline void gen_12_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_11_pref(grono, jed, tbest, x);
    gen_11_pref(grono, jed, tbest, x | pow2[jed[11]]);
}

inline void gen_13_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_12_pref(grono, jed, tbest, x);
    gen_12_pref(grono, jed, tbest, x | pow2[jed[12]]);
}

inline void gen_14_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_13_pref(grono, jed, tbest, x);
    gen_13_pref(grono, jed, tbest, x | pow2[jed[13]]);
}

inline void gen_15_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_14_pref(grono, jed, tbest, x);
    gen_14_pref(grono, jed, tbest, x | pow2[jed[14]]);
}

inline void gen_16_pref(const tbitmap& grono, int jed[], int& tbest, tbitmap x) {
    gen_15_pref(grono, jed, tbest, x);
    gen_15_pref(grono, jed, tbest, x | pow2[jed[15]]);
}

inline void gen_0(int& tbest) {
     tbest = 0;
};

inline void gen_1(int jed[], int& tbest) {
     tbest = bd[jed[0]].czas;
};

inline void gen_2(const tbitmap& grono, int jed[], int& tbest) {
    gen_1_pref(grono, jed, tbest, 0);
}

inline void gen_3(const tbitmap& grono, int jed[], int& tbest) {
    gen_2_pref(grono, jed, tbest, 0);
}

inline void gen_4(const tbitmap& grono, int jed[], int& tbest) {
    gen_3_pref(grono, jed, tbest, 0);
}

inline void gen_5(const tbitmap& grono, int jed[], int& tbest) {
    gen_4_pref(grono, jed, tbest, 0);
}

inline void gen_6(const tbitmap& grono, int jed[], int& tbest) {
    gen_5_pref(grono, jed, tbest, 0);
}

inline void gen_7(const tbitmap& grono, int jed[], int& tbest) {
    gen_6_pref(grono, jed, tbest, 0);
}

inline void gen_8(const tbitmap& grono, int jed[], int& tbest) {
    gen_7_pref(grono, jed, tbest, 0);
}

inline void gen_9(const tbitmap& grono, int jed[], int& tbest) {
    gen_8_pref(grono, jed, tbest, 0);
}

inline void gen_10(const tbitmap& grono, int jed[], int& tbest) {
    gen_9_pref(grono, jed, tbest, 0);
}

inline void gen_11(const tbitmap& grono, int jed[], int& tbest) {
    gen_10_pref(grono, jed, tbest, 0);
}

inline void gen_12(const tbitmap& grono, int jed[], int& tbest) {
    gen_11_pref(grono, jed, tbest, 0);
}

inline void gen_13(const tbitmap& grono, int jed[], int& tbest) {
    gen_12_pref(grono, jed, tbest, 0);
}

inline void gen_14(const tbitmap& grono, int jed[], int& tbest) {
    gen_13_pref(grono, jed, tbest, 0);
}

inline void gen_15(const tbitmap& grono, int jed[], int& tbest) {
    gen_14_pref(grono, jed, tbest, 0);
}

inline void gen_16(const tbitmap& grono, int jed[], int& tbest) {
    gen_15_pref(grono, jed, tbest, 0);
}

int oblicz(tbitmap grono) {
    // cout << hex << grono << " start\n";

    if ((0 == t[grono]) && grono) {
        if (suma_wag(grono) <= W) {
            t[grono] = max_czas(grono);
            // cout << hex << grono << ": tbest (fast) = " << dec << t[grono] << '\n';
        } else {
            // wygeneruj wszystkie podzbiory

            int tbest = MaxTTotal + 1;
            int jed[MaxN + 1];
            int jsz = 0;

            for (int i = 0; i < N; i++) {
                if (grono & pow2[i]) {
                    jed[jsz++] = i;
                }
            }

            // cout << hex << grono << ": jsz=" << dec << jsz << '\n';

            switch (jsz) {
                case 0:
                    gen_0(tbest);
                    break;
                case 1:
                    gen_1(jed, tbest);
                    break;
                case 2:
                    gen_2(grono, jed, tbest);
                    break;
                case 3:
                    gen_3(grono, jed, tbest);
                    break;
                case 4:
                    gen_4(grono, jed, tbest);
                    break;
                case 5:
                    gen_5(grono, jed, tbest);
                    break;
                case 6:
                    gen_6(grono, jed, tbest);
                    break;
                case 7:
                    gen_7(grono, jed, tbest);
                    break;
                case 8:
                    gen_8(grono, jed, tbest);
                    break;
                case 9:
                    gen_9(grono, jed, tbest);
                    break;
                case 10:
                    gen_10(grono, jed, tbest);
                    break;
                case 11:
                    gen_11(grono, jed, tbest);
                    break;
                case 12:
                    gen_12(grono, jed, tbest);
                    break;
                case 13:
                    gen_13(grono, jed, tbest);
                    break;
                case 14:
                    gen_14(grono, jed, tbest);
                    break;
                case 15:
                    gen_15(grono, jed, tbest);
                    break;
                case 16:
                    gen_16(grono, jed, tbest);
                    break;
            }

            t[grono] = tbest;
            // cout << hex << grono << ": tbest=" << dec << tbest << '\n';
        }
    }

    return t[grono];
}

//void oblicz() {
//    oblicz_grono(mask_all);
//}

void wypisz() {
    // cout << "Oto wynik: " << t[mask_all] << '\n';
    cout << t[mask_all] << '\n';
}

int main() {
    init1();
    czytaj();
    init2();

    oblicz(mask_all);
    wypisz();

    return 0;
}

