#include <array>
#include <iomanip>
#include <iostream>

const int FaceSz = 11;

// Max amount, in cents, as 5c multiply
const int MaxVal = 300 * 100 / 5;

// Faces, as 5c multiples
const std::array<int,FaceSz> Faces = {1,2,4,10,20,40,100,200,400,1000,2000};

// t[a][f] = number of ways to represent the amount (a) using only faces
//           no greater than (f).
std::array<std::array<long,FaceSz>,MaxVal+1> t{};

using namespace std;

void init_t() {
    for (int j=0; j < FaceSz; ++j) {
        t[0][j] = 1;
    }

    for (int i=1; i <= MaxVal; ++i) {
        for (int j=0; j < FaceSz; ++j) {
            long total = 0;

            for (int k=0; (k<=j) && (Faces[k] <= i); ++k) {
                total += t[i-Faces[k]][k];
            }

            t[i][j] = total;
        }
    }
}

void czytaj() {
    int dollar, cent;
    char dot;

    while (1) {
        cin >> dollar >> dot >> cent;

        if ((0==dollar) && (0==cent))
            break;

        const int a5 = (100*dollar + cent) / 5;
        cout << right << setw(3) << dollar << "."
             << setfill('0') << setw(2) << cent
             << right << setfill(' ') << setw(17) << t[a5][10] << endl;
    }
}

int main() {
    init_t();
    czytaj();
}

