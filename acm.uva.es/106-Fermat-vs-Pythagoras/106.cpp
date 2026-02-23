/**
 * Uwaga, z tego kodu zniknely odwolania do vector.at()
 * zeby sie kompilowalo w gcc 2.95.
 */

#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#include <cstring>

using namespace std;

template<class T> class RozneFajneObliczenia {
    protected:
        vector<T>* primes;
        void generatePrimes(T upperLimit);
        bool isPrime(T number, int left, int right);
        T sqrt(T number, T left, T right);
    public:
        RozneFajneObliczenia();
        virtual ~RozneFajneObliczenia();
        T sqrt(T number);
        bool isPrime(T number);
        void getPrimeFactors(T number, vector<T> &primeDivs, vector<T> &primeDivExps);
        T gcd(T a, T b);
        T euler(T number);
        void generateCoPrimes(T number, vector<T> &toAppendTo);
};

template<class T> RozneFajneObliczenia<T>::RozneFajneObliczenia() {
    primes = new vector<T>;
    primes->push_back(2);
    primes->push_back(3);
    primes->push_back(5);
}

template<class T> RozneFajneObliczenia<T>::~RozneFajneObliczenia() {
    delete primes;
}

template<class T> void RozneFajneObliczenia<T>::generatePrimes(T upperLimit) {
    for (T p = primes->back() + 2; p <= upperLimit; p += 2) {
        bool pp = true;
        T psqrt = (this->sqrt(p)) + 1;
        for (unsigned int i = 0; i < primes->size(); i++) {
            T ip = (*primes)[i];
            if (ip >= psqrt)
                break;
            if (0 == (p % ip)) {
                pp = false;
                break;
            }
        }
        if (pp) {
            // cout << "    addPrime: " << p << endl;
            primes->push_back(p);
        }
    }
}

/**
 * pierwiastek kwadratowy zaokraglony w dol
 */
template<class T> T RozneFajneObliczenia<T>::sqrt(T number, T left, T right) {
    if (left == right)
        return left;

    T m = (left + right + 1) / 2;
    T m2 = m * m;
    if (number == m2)
        return m;
    else if (number <= m2)
        return sqrt(number, left, m - 1);
    else
        return sqrt(number, m, right);
}

/**
 * pierwiastek kwadratowy zaokraglony w dol
 */
template<class T> T RozneFajneObliczenia<T>::sqrt(T number) {
    if (number <= 0)
        return 0;
    return sqrt(number, 1, number);
}

template<class T> bool RozneFajneObliczenia<T>::isPrime(T number, int left, int right) {
    int m = (left + right + 1) / 2;
    T mp = (*primes)[m];
    if (left == right)
        return (number == mp);

    if (number == mp)
        return true;
    else if (number <= mp)
        return isPrime(number, left, m - 1);
    else
        return isPrime(number, m, right);
}

template<class T> bool RozneFajneObliczenia<T>::isPrime(T number) {
    generatePrimes(number);
    return isPrime(number, 0, primes->size() - 1);
}

template<class T> T RozneFajneObliczenia<T>::gcd(T a, T b) {
    if (0 == a)
        return b;
    return gcd(b % a, a);
}

/**
 * Liczba liczb 0..number-1 wzglednie pierwszych z number.
 * euler(1) = 1
 * euler(p^k) = p^k - p^(k-1)
 * euler(m*n) = euler(m) * euler(n) if gcd(m,n) = 1
 */
template<class T> T RozneFajneObliczenia<T>::euler(T number) {
    if (number <= 0)
        return 0;
    if (number <= 2)
        return 1;

    generatePrimes(number);
    if (isPrime(number))
        return number - 1;

    T result = 1;
    for (unsigned int i = 0; (i < primes->size()) && (number != 1); i++) {
        T ip = (*primes)[i];
        T factor = 1;
        while (0 == (number % ip)) {
            factor *= ip;
            number /= ip;
        }
        if (1 != factor)
            result *= (factor - (factor/ip));
    }

    return result * euler(number);
}

template<class T> void RozneFajneObliczenia<T>::generateCoPrimes(T number, vector<T> &toAppendTo) {
    if (1 == number) {
        toAppendTo.push_back(0);
        return;
    }

    vector<T> primeDivs;
    vector<T> primeDivExps;
    getPrimeFactors(number, primeDivs, primeDivExps);

    for (T cp = 1; cp < number; cp++) {
        bool coprime = true;
        for (unsigned int j = 0; coprime && (j < primeDivs.size()); j++) {
            if (0 == (cp % primeDivs[j]))
                coprime = false;
        }
        if (coprime)
            toAppendTo.push_back(cp);
    }
}

template<class T> void RozneFajneObliczenia<T>::getPrimeFactors(T number, vector<T> &primeDivs, vector<T> &primeDivExps) {
    // cout << "getPrimeFactors: number=" << number << endl;

    if (number <= 1)
        return;

    generatePrimes(number);
    if (isPrime(number)) {
        primeDivs.push_back(number);
        primeDivExps.push_back(1);
    }

    for (unsigned int i = 0; (i < primes->size()) && (number != 1); i++) {
        T ip = (*primes)[i];
        T exp = 0;
        while (0 == (number % ip)) {
            exp += 1;
            number /= ip;
        }
        if (1 <= exp) {
            primeDivs.push_back(ip);
            primeDivExps.push_back(exp);
        }
    }
}

//
// PITAGORAS
//

template<class T> class TrojkaPitagorejska {
    protected:
        T a;
        T b;
        T c;
    public:
        T getA() { return a; }
        T getB() { return b; }
        T getC() { return c; }

        TrojkaPitagorejska(T ta, T tb, T tc) {
            this->a = ta;
            this->b = tb;
            this->c = tc;
        }

    bool operator() (const TrojkaPitagorejska<T>& lhs, const TrojkaPitagorejska<T>& rhs) const {
        return (lhs.c < rhs.c);
    }
};

template<class T> class Pitagoras {
    protected:
        RozneFajneObliczenia<T>* rfo;
        vector<T>* kwadratyLiczb;
        T doKwadratu(T number);
    public:
        Pitagoras();
        virtual ~Pitagoras();
        T liczbaPierwotnychTrojekPitagorejskich(T m);
        void generujPierwotneTrojki(T maxC, vector<TrojkaPitagorejska<T> > &toAppendTo);
        T liczbaPierwotnychTrojekPitagorejskichLimit(T maxC, T* usedNumbers);
};

template<class T> Pitagoras<T>::Pitagoras() {
    rfo = new RozneFajneObliczenia<T>;
    kwadratyLiczb = new vector<T>;
}

template<class T> Pitagoras<T>::~Pitagoras() {
    delete rfo;
    delete kwadratyLiczb;
}

template<class T> inline T Pitagoras<T>::doKwadratu(T number) {
    while (kwadratyLiczb->size() <= number) {
        T sz = kwadratyLiczb->size();
        kwadratyLiczb->push_back(sz * sz);
    }
    return (*kwadratyLiczb)[number];
}

/**
 *  For integers m > n :
 *
 *    a = m^2 - n^2
 *    b = 2 * m * n
 *    c = m^2 + n^2
 *
 *  gives a pithagorean triple.
 *
 *  Pithagorean triple is primitive <=> gcd(m,n) = 1 and at least one of (m,n) is even.
 *
 *  This function accepts m.
 */
template<class T> T Pitagoras<T>::liczbaPierwotnychTrojekPitagorejskich(T m) {
    T nCount = rfo->euler(m);
    if (0 != (m % 2))
        nCount /= 2;

    if (1 <= nCount)
        cout << "  m=" << m << " -> " << nCount << endl;

    return nCount;
}

template<class T> T Pitagoras<T>::liczbaPierwotnychTrojekPitagorejskichLimit(T maxC, T* usedNumbers) {
    T result = 0;

    T maxM = rfo->sqrt(maxC);
    vector<T> copvec;
    for (T m = 2; m <= maxM; m++) {
        bool m_is_even = (0 == (m % 2));
        T m2 = doKwadratu(m);
        copvec.clear();
        rfo->generateCoPrimes(m, copvec);
        // cout << "m=" << m << " --> " << copvec.size() << " co-primes" << endl;
        for (unsigned int j = 0; j < copvec.size(); j++) {
            T n = copvec[j];
            if (m_is_even || (0 == (n % 2))) {
                // pierwotna trojka
                T n2 = doKwadratu(n);
                T tc = m2 + n2;
                if (tc <= maxC) {
                    // w limicie
                    T ta = m2 - n2;
                    T tb = 2 * m * n;
                    if (tb < ta) {
                        T tswap = ta;
                        ta = tb;
                        tb = tswap;
                    }
                    //usedNumbers.insert(ta);
                    //usedNumbers.insert(tb);
                    //usedNumbers.insert(tc);
                    // cout << "  pierw. (" << m << ", " << n << "): " << ta << " " << tb << " " << tc << endl;
                    result++;

                    for (T tta=ta, ttb=tb, ttc=tc; ttc <= maxC; ttc += tc) {
                        usedNumbers[tta] = 1;
                        usedNumbers[ttb] = 1;
                        usedNumbers[ttc] = 1;
                        //usedNumbers.insert(tta);
                        //usedNumbers.insert(ttb);
                        //usedNumbers.insert(ttc);
                        tta += ta;
                        ttb += tb;
                    }
                }
            }
        }
    }
    return result;
}

template<class T> void Pitagoras<T>::generujPierwotneTrojki(T maxC, vector<TrojkaPitagorejska<T> > &toAppendTo) {
    T maxM = rfo->sqrt(maxC);
    vector<T> copvec;
    for (T m = 2; m <= maxM; m++) {
        bool m_is_even = (0 == (m % 2));
        T m2 = doKwadratu(m);
        copvec.clear();
        rfo->generateCoPrimes(m, copvec);
        // cout << "m=" << m << " --> " << copvec.size() << " co-primes" << endl;
        for (unsigned int j = 0; j < copvec.size(); j++) {
            T n = copvec[j];
            if (m_is_even || (0 == (n % 2))) {
                // pierwotna trojka
                T n2 = doKwadratu(n);
                T tc = m2 + n2;
                if (tc <= maxC) {
                    // w limicie
                    T ta = m2 - n2;
                    T tb = 2 * m * n;
                    if (tb < ta) {
                        T tswap = ta;
                        ta = tb;
                        tb = tswap;
                    }
                    //usedNumbers.insert(ta);
                    //usedNumbers.insert(tb);
                    //usedNumbers.insert(tc);
                    toAppendTo.push_back(TrojkaPitagorejska<T>(ta, tb, tc));
                    // cout << "  pierw. (" << m << ", " << n << "): " << ta << " " << tb << " " << tc << endl;
                }
            }
        }
    }
}

typedef unsigned long T;

int main(char** args) {

    const T maxN = 1000000;
    vector<TrojkaPitagorejska<T> > pierwotne;
    Pitagoras<T> pita;
    pita.generujPierwotneTrojki(maxN, pierwotne);

    //cout << "W sumie trojek: " << pierwotne.size() << endl;

//    for (unsigned int i = 0; i < pierwotne.size(); i++) {
//        TrojkaPitagorejska<T> &tp = pierwotne[i];
//        cout << tp.getA() << ", " << tp.getB() << ", " << tp.getC() << endl;
//    }

        // bitset<T> usedNumbers;
        T* usedNumbers = new T[maxN + 1];


        unsigned long n = 0;
        unsigned long n2 = 0;
        //time_t tStart = time(NULL);

        while (! cin.eof()) {
            cin >> n;
            if (cin.bad() || cin.fail())
                break;

            // cout << "---------------------- n = " << n << endl;
            n2 = 2 * n;

            memset(usedNumbers, 0, sizeof(T) * (n + 1));
            // T primitiveTripleCount = pita.liczbaPierwotnychTrojekPitagorejskichLimit(n, usedNumbers);

            T primitiveTripleCount = 0;

            for (unsigned int i = 0; i < pierwotne.size(); i++) {
                TrojkaPitagorejska<T> &tp = pierwotne[i];
                T ta = tp.getA();
                T tb = tp.getB();
                T tc = tp.getC();

                if (tc <= n) {
                    primitiveTripleCount++;
                    // cout << ta << ", " << tb << ", " << tc << endl;

                    for (T tta=ta, ttb=tb, ttc=tc; ttc <= n; ttc += tc) {
                        usedNumbers[tta] = 1;
                        usedNumbers[ttb] = 1;
                        usedNumbers[ttc] = 1;
                        tta += ta;
                        ttb += tb;
                    }
                } else if (n2 < tc)
                    break;
            }

            // used numbers
            long usedNC = 0;
            for (unsigned int i = 1; i <= n; i++) {
                if (usedNumbers[i])
                    usedNC++;
            }

            // cout << "WYNIK: " << primitiveTripleCount << ' ' << (n - usedNumbers.size()) << endl;
            // cout << primitiveTripleCount << ' ' << (n - usedNumbers.size()) << endl;
            cout << primitiveTripleCount << ' ' << (n - usedNC) << endl;

        }

            //time_t tEnd = time(NULL);
            //cout << "Liczba sekund: " << (tEnd - tStart) << endl;

        return 0;
}
