#include <array>
#include <iostream>
#include <string>

const int FaceSz = 11;

// Max amount, in cents, as 5c multiply
const int MaxVal = 300 * 100 / 5;

// Faces, as 5c multiples
const std::array<int,FaceSz> Faces = {1,2,4,10,20,40,100,200,400,1000,2000};

// t[a][f] = number of ways to represent the amount (a) using only faces
//           no greater than (f).
std::array<std::array<long,FaceSz>,MaxVal+1> t{};

using namespace std;

std::string formatA(int val) {
    int d = val / 100;
    int c = val % 100;
    return (to_string(d) + "." + ((c<=9) ? "0" : "") + to_string(c));
}

int main() {
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

            cout << formatA(5*i) << " using " << formatA(5*Faces[j]) << " (or less) : " << total << " ways" << endl; 
        }
    }

    cout << "{";
    for (int i=0; i <= MaxVal; ++i) {
        cout << "{";
        for (int j=0; j < FaceSz; ++j) {
            cout << t[i][j] << ((FaceSz==j+1) ? "" : ",");
        }
        cout << "},";
    }
    cout << "};";

    return 0;
}

