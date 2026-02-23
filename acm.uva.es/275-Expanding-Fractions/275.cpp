#include <array>
#include <iostream>
#include <string>
#include <vector>

using uint = unsigned int;

const uint MaxD     = 1000;
const uint MaxWidth =   50;
std::array<int,MaxD+3> remainders;  // remainder -> 1-index of expansion digit
std::vector<int> expansion;         // the initial 0 + digits computed so far
const std::string s_terminates = "This expansion terminates.";

using namespace std;

void print_expansion() {
    cout << ".";
    for (size_t i=1; i < expansion.size(); ++i) {
        cout << expansion[i];
        if (0 == ((i+1) % MaxWidth))
            cout << endl;
    }
    if (expansion.size() % MaxWidth)
        cout << endl;
}

void print_repeat(uint o) {
    cout << "The last " << o << " digits repeat forever." << endl;
}

void print_terminate() {
    cout << s_terminates << endl;
}

void divide(const uint a, const uint b) {
    remainders.fill(0);
    expansion.clear();
    expansion.reserve(b+3);
    expansion.push_back(0);
    uint k = a;

    while (1) {
        k *= 10;
        const int q = k/b;
        k -= b*q;

        if (k) {
            if (remainders[k]) {
                // we've seen this remainder before!
                expansion.push_back(q);

                const int exp_len = expansion.size();
                const int period_len = exp_len - remainders[k];
                if (expansion[exp_len - 1] == expansion[exp_len - period_len - 1])
                    expansion.pop_back();

                print_expansion();
                print_repeat(period_len);
                return;
            }

            expansion.push_back(q);
            remainders[k] = expansion.size();
        } else {
            if (q)
                expansion.push_back(q);

            print_expansion();
            print_terminate();
            return;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    while (1) {
        uint a, b;
        cin >> a >> b;

        if ((0==a) && (0==b))
            break;

        if (0==a) {
            cout << 0 << endl;
            print_terminate();
        } else {
            // we assume a<b
            divide(a,b);
        }

        cout << endl;
    }

    return 0;
}

