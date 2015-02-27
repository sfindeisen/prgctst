/**
 *   2007-11-19
 *   STF
 */

#include <cstring>
#include <iostream>

using namespace std;

const long MaxLength = 1000000;

void foldRight(char * fibo, long index) {
    long i = index;
    while ((1 == fibo[i]) && (1 == fibo[i+1])) {
        //cout << "FR(" << i << ") ";
        fibo[i]   =  0;
        fibo[i+1] =  0;
        fibo[i+2] += 1;
        i += 2;
    }
    //cout << "\n";
}

void inc(char * fibo, long index) {
    fibo[index] += 1;
    //cout << "inc[" << index << "] => " << (int) fibo[index] << "\n";
    if (1 == fibo[index]) {
        // cout << "-- 1\n";
        if (1 == fibo[index + 1])
            foldRight(fibo, index);
        else if ((1 <= index) && (1 == fibo[index - 1]))
            foldRight(fibo, index - 1);
    } else if (2 == fibo[index]) {
        // cout << "-- 2\n";
        if (0 == index) {
            // cout << "-- -- 0\n";
            fibo[0] = 0;
            fibo[1] = 1;
            foldRight(fibo, 1);
        } else if (1 == index) {
            // cout << "-- -- 1\n";
            fibo[0] = 1;
            fibo[1] = 0;
            fibo[2] = 1;
            foldRight(fibo, 2);
        } else {
            // cout << "-- -- daleko\n";
            fibo[index]     = 0;
            fibo[index + 1] = 1;
            foldRight(fibo, index + 1);

            inc(fibo, index - 2);
            //long k = index - 2;
            //fibo[k] += 1;
        }
    }
}

void wczytaj(char * fibo) {
    long i = 0, k = 0;
    cin >> i;
    // cout << i << " digits: ";

    for (int j = 0; j < i; j++) {
        cin >> k;
        fibo[j] = k;
        // cout << (int) fibo[j] << " ";
    }
    // cout << '\n';
}

void czytaj_i_dodawaj(char * fibo) {
    long i = 0, k = 0;
    cin >> i;
    // cout << "czytaj_i_dodawaj: digits: " << i << "\n";

    for (long j = 0; j < i; j++) {
        cin >> k;
        if (1 == k) {
            // cout << "czytaj_i_dodawaj: " << j << "\n";
            inc(fibo, j);
        }
    }
}

void wypisz_wynik(char * fibo, long len) {
    long end = len - 1;
    while ((0 <= end) && (0 == fibo[end]))
        end--;
    // cout << "Wynik: end=" << end << "\n";
    if (end < 0)
        cout << "1 0";
    else {
        cout << (end + 1) << " ";
        for (long i = 0; i <= end; i++)
            cout << (int) fibo[i] << " ";
    }
    cout << '\n';
}

int main() {
    const long len = MaxLength + 5;
    char fibo[len];
    memset(fibo, 0, len);

    wczytaj(fibo);
    czytaj_i_dodawaj(fibo);
    wypisz_wynik(fibo, len);

    return 0;
}
