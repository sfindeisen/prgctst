#include <iostream>
#include <algorithm>
#include <vector>

const long MaxN = 1000000000;

struct tmr {
    long w1;
    long w2;
    long h1;
    long h2;
};

using namespace std;

vector<tmr> v;
 
int main() {
    ios_base::sync_with_stdio(0);

    long t = 0;
    long n, w1, w2, h1, h2, minW, maxW, minH, maxH;

    cin >> t;

    for (long i = 0; i < t; ++i) {
	maxH = maxW = -1;
	minH = minW = MaxN + 1;

	short res = 0;
	v.clear();
	cin >> n;

	// read
	for (long j = 0; j < n; ++j) {
	    cin >> w1;
	    cin >> w2;
	    cin >> h1;
	    cin >> h2;

	    tmr mr;
	    mr.w1 = w1;
	    mr.w2 = w2;
	    mr.h1 = h1;
	    mr.h2 = h2;

	    v.push_back(mr);

	    minW = min(minW, w1);
	    maxW = max(maxW, w2);
	    minH = min(minH, h1);
	    maxH = max(maxH, h2);
	}

	// check
	for (vector<tmr>::iterator it = v.begin(); it != v.end(); ++it) {
	    if ((minW == (it->w1)) && (maxW == (it->w2)) && (minH == (it->h1)) && (maxH == (it->h2))) {
		res = 1;
		break;
	    }
	}

	cout << (res ? "TAK" : "NIE") << endl;
    }

    return 0;
}
