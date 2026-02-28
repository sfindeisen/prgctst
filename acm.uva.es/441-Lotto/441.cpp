#include <array>
#include <iostream>

const int Ch   =  6;
const int MaxK = 13;

std::array<int,MaxK> dom;
std::array<int,Ch> sel;

using namespace std;

void print_out() {
    cout << sel[0];
    for (int i=1; i<Ch; ++i) {
        cout << " " << sel[i];
    }
    cout << endl;
}

// dind  = current dom array index (the one to use)
// count = how many numbers generated so far
// K     = dom array length
inline void gen(const int dind, const int count, const int K) {
    if (count == Ch) {
        print_out();
        return;
    }

    if ((dind < MaxK) && (Ch-count <= K-dind)) {
        sel[count] = dom[dind];
        gen(dind+1, count+1, K);
        gen(dind+1, count,   K);
    }
}

int main() {
    std::ios_base::sync_with_stdio(0);

    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int j,k;

    for (int testcase=0;; ++testcase) {
        cin >> k;

        if (Ch <= k) {
            for (int i=0; i<k; ++i) {
                cin >> j;
                dom[i] = j;
            }

            if (1 <= testcase)
                cout << endl;

            gen(0,0,k);
        } else {
            break;
        }
    }

    return 0;
}

