#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>

using namespace std;

int main() {
    int n, j;
    vector<int> u;
    cin >> n;

    for (int i=0; i<n; ++i) {
        cin >> j;
        u.push_back(j);
    }

    sort(u.begin(), u.end());
    long s = accumulate(u.begin(), u.end(), 0L);

    for (int i=0,j=n-1; (i<=j); ++i,--j) {
        s += u[j] - u[i];
    }

    cout << s << endl;
    return 0;
}
