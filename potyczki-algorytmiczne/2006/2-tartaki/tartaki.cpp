//#include <iostream>
#include <cstdio>
#include <string>
#include <stack>

using namespace std;

int main() {
    long n = 0;
    long g = 0;
    long t = 0;
    string s;
    stack<long> gs;
    stack<long> ts;

    // cin >> n;
    // cin >> s;
    scanf("%ld", &n);
    long n2 = 2 * n;

    for (long j = 0; j < n2; ++j) {
        char c = 0;
        scanf("%c", &c);
        if (('g' == c) || ('t' == c))
            s += c;
        else
            j -= 1;
    }

    // cerr << "#" << s << "$" << s.length() << endl;

    for (long j = 0; j < n2; ++j) {
        const char c = s[j];

        switch (c) {
            case 'g':
                ++g;
                if (ts.empty()) {
                    gs.push(g);
                } else {
                    // cout << g << " " << ts.top() << endl;
                    printf("%ld %ld\n", g, ts.top());
                    ts.pop();
                }
                break;
            case 't':
                ++t;
                if (gs.empty()) {
                    ts.push(t);
                } else {
                    // cout << gs.top() << " " << t << endl;
                    printf("%ld %ld\n", gs.top(), t);
                    gs.pop();
                }
                break;
        }
    }

    return 0;
}
