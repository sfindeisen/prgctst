#include <iomanip>
#include <iostream>
#include <vector>

std::vector<long> vec;

long sum1to(long n) {
    return n*(n+1)/2;
}

bool check(int h, int last) {
    long s1 = sum1to(h-1);
    long s2 = sum1to(last)-s1-h;
    return (s1==s2);
}

using namespace std;

int main() {
    vec.reserve(100);
    vec.push_back(0);
    long s = 0;
    int  h = 1;

    for (int i=1; i < 500*1000*1000; ++i) {
        s += i;
        vec.push_back(s);

        if ((h < i) && (vec[h-1] <= s-vec[h])) {
            if (vec[h-1] == s-vec[h])
                cout << right << setfill(' ') << setw(10) << h
                     << right << setw(10) << i
                     << right << setw(10) << (check(h,i) ? "OK" : "Err") << endl;
            ++h;
        }
    }

    return 0;
}
