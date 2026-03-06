#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

const long MaxM = 32768;

std::vector<int> primes1;
std::vector<int> primes2;

using namespace std;

void generate_primes(const long m) {
    primes1.push_back(2);
    primes2.push_back(4);

    for (int i=3; i<=m; ++i) {
        bool is_prime = true;

        for (size_t j=0; is_prime && (j < primes1.size()) && (primes2[j] <= i); ++j) {
            if (0 == i % primes1[j])
                is_prime = false;
        }
        if (is_prime) {
            primes1.push_back(i);
            primes2.push_back(i*i);
        }
    }

    for (auto x : primes1)
        cout << "," << x;
    cout << endl;

    //for (auto x : primes2)
    //    cout << "," << x;
    //cout << endl;

    cout << "size=" << primes1.size() << endl;
}

int main() {
    // Disable synchronization of all the iostream standard streams with their corresponding
    // standard C streams: http://www.cplusplus.com/reference/ios/ios_base/sync_with_stdio/ .
    std::ios_base::sync_with_stdio(0);

    // By default, cin is tied to the standard output stream cout (see ios::tie), which
    // indicates that cout's buffer is flushed (see ostream::flush) before each I/O
    // operation performed on cin: http://www.cplusplus.com/reference/iostream/cin/ .
    //
    // Here we disable this behavior.
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    generate_primes(MaxM + 5);

    return 0;
}

