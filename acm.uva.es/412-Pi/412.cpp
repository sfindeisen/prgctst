#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

typedef unsigned long ulong;

using namespace std;

std::vector<ulong> read_input(const size_t n) {
    vector<ulong> result;
    result.reserve(1+n);
    ulong k;

    for (size_t i=0; i<n; ++i) {
        cin >> k;
        result.push_back(k);
    }

    return result;
}

ulong gcd(const ulong a, const ulong b) {
    if (a < b)
        return gcd(b,a);
    if (0 == b)
        return a;
    return gcd(b, a%b);
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

    while (1) {
        int n = 0;
        cin >> n;

        if (1 <= n) {
            std::vector<ulong> datavec = read_input(n);
            const size_t N = datavec.size();
            ulong cp = 0;

            for (size_t i=0; i < N; ++i) {
                for (size_t j=0; j < i; ++j) {
                    if (1 == gcd(datavec[j], datavec[i]))
                        ++cp;
                }
            }

            if (1 <= cp) {
                const double pi_est = sqrt(static_cast<double>(3 * N * (N-1)) / cp);
                cout << fixed << setprecision(6) << pi_est << endl;
            } else {
                cout << "No estimate for this data set." << endl;
            }
        } else {
            break;
        }
    }

    return 0;
}

