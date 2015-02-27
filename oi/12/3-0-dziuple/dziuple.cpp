/**
 *   2007-12-07
 *   STF
 */

#include <iostream>
// #include <ctime>

typedef struct {
    long index_znajomi;
    long znajomi_count;
    long spojna_skladowa;   // ktora to spojna skladowa
    long z1;                // znajomy z co najmniej 2 krawedziami
    long z2;                // znajomy z co najmniej 2 krawedziami
    char ktore_drzewo;
} TPtaszek;

typedef struct {
    long ppoj;    // na ile sposobow mozna przestawiac "liscie" (ptaszki ktore maja tylko 1 znajomego) (modulo modulator)
    long d1count;   // liczba ptaszkow na 1 drzewie
    long d2count;   // liczba ptaszkow na 2 drzewie
} TSpojnaSkladowa;

const char Drzewo_1 = 1;
const char Drzewo_2 = 2;

const long MaxLPtaszkow           =  1000000;
const long MaxLSpojnychSkladowych = MaxLPtaszkow;
const long MaxLParZnajomych       = 10000000;
const long MaxModulator           =  2000000;

long lptaszkow      = 0;
long lpar_znajomych = 0;
long modulator      = 0;

TPtaszek ptaszki[MaxLPtaszkow];

long * znajomi = NULL;
long znajomiLen = 0;

TSpojnaSkladowa spojneSkladowe[MaxLSpojnychSkladowych];
long lspojnych_skladowych = 0;

const long SilniaLen = MaxLPtaszkow + 1;
long silniaTab[SilniaLen];

// uzywane gdzieniegdzie dla ustawienia/sprawdzenia ze wynik to 0
bool zeroFlag = false;

using namespace std;

// ===========================
// helpery i debugging
// ===========================

/*
inline void debugMessage(const char * msg) {
    cout << "DEBUG (" << time(NULL) << "): " << msg << '\n';
    cout.flush();
}

void pokaz_tablice(const char * title, const long* t, long len) {
    if (title)
        cout << title;
    cout << "(len=" << len << ") ";
    for (long i = 0; i < len; i++) {
        cout << t[i];
        if (i < len - 1)
            cout << ", ";
    }
    cout << '\n';
}

void pokaz_spojna_skladowa(long i) {
    const long d1count = spojneSkladowe[i - 1].d1count;
    const long d2count = spojneSkladowe[i - 1].d2count;
    const long count   = d1count + d2count;
    const long ppoj    = spojneSkladowe[i - 1].ppoj;
    long k = 0;

    cout << "SS=" << i << ", ppoj=" << ppoj << ", d1count=" << d1count << ", d2count=" << d2count << ", ptaki: ";

    for (long j = 0; j < lptaszkow; j++) {
        if (ptaszki[j].spojna_skladowa == i) {
            cout << j;
            if ((++k < count) || (0 == count))
                cout << ", ";
        }
    }
    cout << '\n';
}

void pokaz_spojna_skladowa() {
    cout << "Liczba spojnych skladowych: " << lspojnych_skladowych << '\n';
    for (long i = 1; i <= lspojnych_skladowych; i++)
        pokaz_spojna_skladowa(i);
}

void pokaz_ptaszka(long i) {
    cout << "P=" << i << ", SS=" << ptaszki[i].spojna_skladowa << ", D=" << (int) (ptaszki[i].ktore_drzewo) << ", z1=" << ptaszki[i].z1 << ", z2=" << ptaszki[i].z2 << '\n';
    cout << "   " << ptaszki[i].znajomi_count << " znajomych (start=" << ptaszki[i].index_znajomi << "): ";
    for (long j = 0; j < ptaszki[i].znajomi_count; j++) {
        long z = znajomi[ptaszki[i].index_znajomi + j];
        cout << z;
        if (j < ptaszki[i].znajomi_count - 1)
            cout << ", ";
    }
    cout << '\n';
}

void pokaz_ptaszki() {
    for (long i = 0; i < lptaszkow; i++) {
        pokaz_ptaszka(i);
    }
}

void pokaz_znajomych_raw() {
    pokaz_tablice("   znajomi (raw): ", znajomi, 2 * lpar_znajomych);
}

void pokaz_znajomosci() {
    cout << lpar_znajomych << " par znajomych: ";
    for (long i = 0; i < lpar_znajomych; i++) {
        cout << znajomi[i] << ':' << znajomi[i + lpar_znajomych];
        if (i < lpar_znajomych - 1)
            cout << ", ";
    }
    cout << '\n';
    pokaz_znajomych_raw();
}

void pokaz() {
    pokaz_ptaszki();
    pokaz_spojna_skladowa();
}
*/

/*
long gcd(long a, long b) {
    if (0 == a)
        return b;
    return gcd(b % a, a);
}
*/

inline char anotherTree(char d) {
    return 3 - d;
}

// k! mod modulator
inline long silnia(long k) {
    if (silniaTab[k] < 0) {
        long i = 0;
        for (i = k - 1; (0 <= i) && (silniaTab[i] < 0); i--);
        // i nie wyszlo poza zakres, bo byla dobra inicjalizacja
        for (long long j = i + 1; j <= k; j++) {
            silniaTab[j] = (j * ((long long) silniaTab[j - 1])) % modulator;
        }
    }

    // cout << "silnia(" << k << ") mod " << modulator << " = " << silniaTab[k] << '\n';
    return silniaTab[k];
}

// ======================================
// Rozdzielanie ptaszkow na drzewa
// ======================================

void rozdzielaj_na_drzewa(long parent, long current) {
    while (0 <= current) {
        // cout << "Rozdzielam na drzewa: parent=" << parent << ", current=" << current << '\n';

        char parentTree = ptaszki[parent].ktore_drzewo;
        if (parentTree == ptaszki[current].ktore_drzewo) {
            // cout << "Problem z drzewem: parent=" << parent << ", current=" << current << '\n';
            zeroFlag = true;
            return;
        }

        if (0 != ptaszki[current].spojna_skladowa) {
            // cout << "Cykl: parent=" << parent << ", current=" << current << '\n';
            // cykl!
            zeroFlag = true;
            return;
        }
        const long ss = ptaszki[parent].spojna_skladowa;
        ptaszki[current].spojna_skladowa = ss;

        char currentTree = anotherTree(parentTree);
        ptaszki[current].ktore_drzewo = currentTree;

        // ustawiamy drzewa w sasiadach
        for (long i = 0; i < ptaszki[current].znajomi_count; i++) {
            long z = znajomi[ptaszki[current].index_znajomi + i];
            if (currentTree == ptaszki[z].ktore_drzewo) {
                // cout << "Problem z drzewem: parent=" << parent << ", current=" << current << ", z=" << z << '\n';
                zeroFlag = true;
                return;
            }
            ptaszki[z].ktore_drzewo = parentTree;

            if (1 == ptaszki[z].znajomi_count)
                ptaszki[z].spojna_skladowa = ss;
        }

        //pokaz_ptaszka(current);

        // jedziemy dalej po jedynym 2-krawedziowym sasiedzie roznym od parenta
        if ((0 <= ptaszki[current].z1) && (parent != ptaszki[current].z1)) {
            parent = current;
            current = ptaszki[current].z1;
        } else if ((0 <= ptaszki[current].z2) && (parent != ptaszki[current].z2)) {
            parent = current;
            current = ptaszki[current].z2;
        } else {
            parent = current;
            current = -1;
        }
    }
}

// rozdziela na drzewa pojedyncza spojna skladowa; current to jej "ojciec"
// (pierwszy element)
void rozdzielaj_na_drzewa(long current) {
    // cout << "Rozdzielam na drzewa: current=" << current << '\n';

    const long ss = ptaszki[current].spojna_skladowa;
    const long z1 = ptaszki[current].z1;
    const long z2 = ptaszki[current].z2;

    char currentTree = Drzewo_1;    // arbitralnie
    char neighbrTree = anotherTree(currentTree);
    ptaszki[current].ktore_drzewo = currentTree;

    // ustaw drzewa i spojne skladowe w sasiadach
    for (long i = 0; i < ptaszki[current].znajomi_count; i++) {
        long z = znajomi[ptaszki[current].index_znajomi + i];
        ptaszki[z].ktore_drzewo = neighbrTree;
        ptaszki[z].spojna_skladowa = ss;
    }

    if (0 <= z1)
        ptaszki[z1].spojna_skladowa = 0;
    if (0 <= z2)
        ptaszki[z2].spojna_skladowa = 0;

    if (0 <= z1)
        rozdzielaj_na_drzewa(current, z1);
    if ((! zeroFlag) && (0 <= z2))
        rozdzielaj_na_drzewa(current, z2);
}

// rozdziela na drzewa wszystkie spojne skladowe po kolei
void rozdzielaj_na_drzewa() {
    long ss = 0;

    for (long i = 0; i < lptaszkow; i++) {
        if ((0 == ptaszki[i].spojna_skladowa) && (2 <= ptaszki[i].znajomi_count)) {
            // boczne ptaszki nie moga byc korzeniami spojnych skladowych tutaj
            ptaszki[i].spojna_skladowa = ++ss;
            rozdzielaj_na_drzewa(i);
            if (zeroFlag)
                return;
        }
    }

    for (long i = 0; i < lptaszkow; i++) {
        if (0 == ptaszki[i].spojna_skladowa) {
            // w tej spojnej skladowej sa same boczne ptaszki - jedz
            ptaszki[i].spojna_skladowa = ++ss;
            rozdzielaj_na_drzewa(i);
            if (zeroFlag)
                return;
        }
    }

    lspojnych_skladowych = ss;
}

// ======================================
// Kilka prostych sprawdzen
// ======================================

// Znajdowanie znajomych z co najmniej 2-ma znajomymi
void sprawdz_liczebnosc() {
    for (long i = 0; i < lptaszkow; i++) {
        ptaszki[i].z1 = -1;
        ptaszki[i].z2 = -1;

        for (long j = 0; j < ptaszki[i].znajomi_count; j++) {
            long z = znajomi[j + ptaszki[i].index_znajomi];
            if (2 <= ptaszki[z].znajomi_count) {
                if (-1 == ptaszki[i].z1)
                    ptaszki[i].z1 = z;
                else if (-1 == ptaszki[i].z2)
                    ptaszki[i].z2 = z;
                else {
                    // to juz 3-ci
                    zeroFlag = true;
                    return;
                }
            }
        }
    }
}

// ===============================================
// Ogladanie spojnych skladowych grafu znajomosci
// ===============================================

// calkowita liczba kombinacji generowanych przez dana spojna skladowa
// (modulo modulator)
// To nie jest keszowane bo bedzie wolane raz.
long kombi_spojna_skladowa(long ss) {
    // zamiana drzew, przestawienia jednokrawedziowych ptaszkow
    long res = 2 * spojneSkladowe[ss].ppoj;
    // obrocenie do gory nogami
    if ((2 <= spojneSkladowe[ss].d1count) && (2 <= spojneSkladowe[ss].d2count))
        res *= 2;

    res %= modulator;
    // cout << "[K] kombi: ss=" << (ss+1) << ", res=" << res << '\n';
    return res;
}

// Liczy kombinacje w obrebie poszczegolnych spojnych skladowych
void licz_spojne_skladowe() {
    for (long i = 0; i < lspojnych_skladowych; i++) {
        spojneSkladowe[i].ppoj = 1;
    }

    for (long i = 0; i < lptaszkow; i++) {
        const long ss = ptaszki[i].spojna_skladowa;
        if (Drzewo_1 == ptaszki[i].ktore_drzewo)
            spojneSkladowe[ss - 1].d1count++;
        else if (Drzewo_2 == ptaszki[i].ktore_drzewo)
            spojneSkladowe[ss - 1].d2count++;

        // liczba pojedynczych znajomych i ich kombinacje
        long pojzc = ptaszki[i].znajomi_count;
        if (0 <= ptaszki[i].z1)
            pojzc--;
        if (0 <= ptaszki[i].z2)
            pojzc--;

        spojneSkladowe[ss - 1].ppoj = ((long long) spojneSkladowe[ss - 1].ppoj * (long long) silnia(pojzc)) % modulator;

        // cout << "[K] P=" << i << ", SS=" << ss << ", pojzc=" << pojzc << ", ppoj=" << spojneSkladowe[ss - 1].ppoj << '\n';
    }
}

// ======================================
// Liczenie wyniku
// ======================================

long licz() {
    // debugMessage("Sprawdzam liczebnosc...");

    sprawdz_liczebnosc();
    if (zeroFlag)
        return 0;

    // pokaz_ptaszki();
    // debugMessage("Rozdzielam na drzewa....");

    rozdzielaj_na_drzewa();
    if (zeroFlag)
        return 0;

    // pokaz_ptaszki();
    // pokaz_spojna_skladowa();

    // debugMessage("Licze spojne skladowe...");

    licz_spojne_skladowe();

    // pokaz_ptaszki();
    // pokaz_spojna_skladowa();

    // debugMessage("Licze wolne ptaszki...");

         long wolnePtaszki = 0;  // liczba "wolnych ptaszkow" (bez sasiadow)
    long long result       = 1;

    for (long i = 0; i < lspojnych_skladowych; i++) {
        if (2 <= spojneSkladowe[i].d1count + spojneSkladowe[i].d2count) {
            result *= kombi_spojna_skladowa(i);
            result %= modulator;
        }
        else
            wolnePtaszki++;
    }
    // cout << "wolnePtaszki=" << wolnePtaszki << ", res=" << result << '\n';

    if (0 == result)
        return 0;

    // Liczba spojnych skladowych z co najmniej 2 ptaszkami.
    // Mozemy je poprzestawiac na k! silnia sposobow.
    const long ssNormal = lspojnych_skladowych - wolnePtaszki;
    result *= silnia(ssNormal);
    result %= modulator;
    // cout << "ssNormal=" << ssNormal << ", res=" << result << '\n';

    if (0 == result)
        return 0;

    // wolne ptaszki moga wskakiwac gdziekolwiek
    const long lZajetychPtaszkow = lptaszkow - wolnePtaszki;
    for (long i = 0; i < wolnePtaszki; i++) {
        result *= (lZajetychPtaszkow + 2 + i);
        result %= modulator;
    }

    // cout << "lZajetychPtaszkow=" << lZajetychPtaszkow << ", res=" << result << '\n';
    return result;
}

// ======================================
// Budowanie struktur danych (znajomi)
// ======================================

void zamien_znajomych(long i, long j) {
    long i2 = lpar_znajomych + i;
    long j2 = lpar_znajomych + j;

    long k = znajomi[i];
    znajomi[i] = znajomi[j];
    znajomi[j] = k;

    k = znajomi[i2];
    znajomi[i2] = znajomi[j2];
    znajomi[j2] = k;
}

inline bool znajomi_le(long i, long j) {
    return (znajomi[i] <= znajomi[j]);
}

void sortuj_znajomych(long left, long right) {
    // cout << "sortuj znajomych: left=" << left << ", right=" << right << '\n';

    if (left >= right)
        return;

    const long center = (left + right) / 2;
    long el_to_swap = center;
    // const long range = right - left + 1;
    // long el_to_swap = left + (rand() % range);

    // mediana z trzech
/*
    TO ZA DLUGO DZIALA!
        if (znajomi_le(left, right)) {
            if (znajomi_le(right, center))
                el_to_swap = right;
            else if (znajomi_le(center, left))
                el_to_swap = left;
        } else {
            if (znajomi_le(left, center))
                el_to_swap = left;
            else if (znajomi_le(center, right))
                el_to_swap = right;
        }
*/

    zamien_znajomych(el_to_swap, left);

    long rb = right;
    for (long i = left + 1; i <= rb; ) {
        if (znajomi_le(i, left))
            i++;
        else
            zamien_znajomych(rb--, i);
    }

    zamien_znajomych(left, rb);
    sortuj_znajomych(left, rb - 1);
    sortuj_znajomych(rb + 1, right);
}

void kopiuj_znajomych(long destOrig, long srcOrig, long countOrig) {
    long dest  = destOrig;
    long src   = srcOrig;
    long count = countOrig;

    if (dest < src) {
        if ((dest + count - 1) >= src) {
            long k = dest + count - src;
            src += k;
            count -= k;
        }
    } else if (dest > src) {
        if ((src + count - 1) >= dest) {
            long k = src + count - dest;
            dest += k;
            count -= k;
        }
    } else
        count = 0;

    if (1 <= count)
        memcpy(znajomi + dest, znajomi + src, count * sizeof(long));
}

inline void dodaj_znajomego(long a, long b) {
    // cout << "dodaj znajomego: " << a << " -> " << b << '\n';
    znajomi[ptaszki[a].index_znajomi + ptaszki[a].spojna_skladowa++] = b;
}

void sortuj_znajomosci() {
    sortuj_znajomych(0, lpar_znajomych - 1);
    // debugMessage("znajomi posortowani");

    // pokaz_znajomosci();

    // Jako biezaca liczbe znajomych bedziemy uzywac pola spojna_skladowa.
    // Na koniec ja wyzerujemy.

    long u[lptaszkow];  // pierwsze polowki (numery ptaszkow)
    long uc = 0;
    long v[lptaszkow];  // pierwsze polowki (gdzie sie zaczyna)

    memset(u, 0, sizeof(u));
    memset(v, 0, sizeof(v));

    // liczymy znajomych
    u[uc] = znajomi[0];
    v[uc] = lpar_znajomych;
    uc++;

    for (long i = 0; i < lpar_znajomych; i++) {
        ptaszki[znajomi[i]                 ].znajomi_count++;
        ptaszki[znajomi[i + lpar_znajomych]].znajomi_count++;

        // liczby znajomych dla pierwszych polowek
        ptaszki[znajomi[i]].spojna_skladowa++;

        if (u[uc - 1] != znajomi[i]) {
            u[uc] = znajomi[i];
            v[uc] = i + lpar_znajomych;
            uc++;
        }
    }

    // debugMessage("u,v,uc policzone");
    // cout << "uc=" << uc << '\n';
    // pokaz_tablice("u=", u, uc);
    // pokaz_tablice("v=", v, uc);

    // liczymy docelowe indeksy w tablicy znajomi
    long s = 0;
    for (long i = 0; i < lptaszkow; i++) {
        ptaszki[i].index_znajomi = s;
        s += ptaszki[i].znajomi_count;
    }

    // debugMessage("kopiujemy pierwsze polowki na ich docelowe miejsca...");

    // kopiujemy znajomych na ich docelowe miejsca (pierwsze polowki)
    for (long i = 0; i < uc; i++) {
        long p = u[i];
        long count = ptaszki[p].spojna_skladowa;
        long src   = v[i];
        long dest  = ptaszki[p].index_znajomi;

        // cout << "kopiuj: p=" << p << ", count=" << count << ", src=" << src << ", dest=" << dest << '\n';

        kopiuj_znajomych(dest, src, count);

        // teraz v[i] przechowuje liczbe wpisanych znajomych
        v[i] = ptaszki[p].spojna_skladowa;
    }

    // pokaz_tablice("(ile wpisanych) v=", v, uc);
    // pokaz_znajomych_raw();

    // debugMessage("wpisujemy drugie polowki na ich docelowe miejsca...");

    // wpisujemy znajomych na ich docelowe miejsca (drugie polowki)
    for (long i = 0; i < uc; i++) {
        long p = u[i];

        // jedziemy po drugich (dolnych) polowkach:
        for (long j = 0; j < v[i]; j++) {
            long n = znajomi[j + ptaszki[p].index_znajomi];
            dodaj_znajomego(n, p);
        }
    }

    // pokaz_znajomych_raw();

    // debugMessage("zerujemy spojne skladowe...");

    // zerujemy pola spojna_skladowa
    for (long i = 0; i < lptaszkow; i++) {
        ptaszki[i].spojna_skladowa = 0;
    }
}

// ===============================================
// Wczytywanie danych wejsciowych i main()
// ===============================================

void czytaj() {
    cin >> lptaszkow >> lpar_znajomych >> modulator;
    // cout << "lptaszkow=" << lptaszkow << ", lpar_znajomych=" << lpar_znajomych << ", modulator=" << modulator << '\n';

    // init znajomi
    znajomiLen = 2 * lpar_znajomych;
    znajomi = new long[znajomiLen];
    memset(znajomi, 0, znajomiLen * sizeof(long));

    long a, b;
    for (long i = 0; (i < lpar_znajomych); i++) {
        cin >> a >> b;

        if (a <= b) {
            znajomi[i] = a - 1;
            znajomi[i + lpar_znajomych] = b - 1;
        } else {
            znajomi[i] = b - 1;
            znajomi[i + lpar_znajomych] = a - 1;
        }
    }
}

void init() {
    // srand(time(NULL));

    memset(ptaszki,        0, sizeof(ptaszki));
    memset(spojneSkladowe, 0, sizeof(spojneSkladowe));

    memset(silniaTab,      0, sizeof(silniaTab));
    for (long i = 0; i < SilniaLen; i++)
        silniaTab[i] = -1;
    silniaTab[0] = 1;
    silniaTab[1] = 1;
}

int main() {
    // cout << "sizeof(long)=" << sizeof(long) << ", sizeof(long long)=" << sizeof(long long) << '\n';

    init();
    // debugMessage("dopiero czytam dane...");
    czytaj();
    // debugMessage("Dane wczytane, tworze struktury....");

    // pokaz_znajomosci();
    sortuj_znajomosci();

    // debugMessage("Dane zainicjalizowane, licze wynik...");
    // pokaz();
    // debugMessage("Teraz juz naprawde licze wynik...");

    long result = licz();
    // cout << "WYNIK: " << result << '\n';
    cout << result << '\n';

    return 0;
}
