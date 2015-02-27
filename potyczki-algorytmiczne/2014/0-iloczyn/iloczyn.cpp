#include <iostream>
#include <set>
#include <vector>

using namespace std;
 
int main() {
    ios_base::sync_with_stdio(0);
    const long MaxN = 1000000000;

    // generate fibo
    long k1 = 0;
    long k2 = 1;
    long k3 = 0;
    vector<long> v;

    while (k2 <= MaxN) {
	// cout << k2 << endl;
	v.push_back(k2);

	k3 =  k2;
	k2 += k1;
	k1 =  k3;
    }

    // generate fibo products
    set<long> s;

    for (std::vector<long>::iterator it1 = v.begin(); it1 != v.end(); ++it1) {
	k1 = *it1;

	for (std::vector<long>::iterator it2 = it1; it2 != v.end(); ++it2) {
	    k2 = *it2;
	    k3 = k1 * k2;

	    if (k3 <= MaxN)
		s.insert(k3);
	    else
		break;
	}
    }

    s.insert(0);

    // answer the questions
    int t;
    cin >> t;

    for (int i = 0; i < t; ++i) {
	cin >> k1;
	cout << (((0 == k1) || (s.find(k1) != s.end())) ? "TAK" : "NIE") << endl;
    }

    return 0;
}
