#include <iostream>

using namespace std;

int main(char** args) {
    const char symbols[3] = {'B', 'C', 'G'};
    int t[9];
    int r[3];

    // cout << "size of t is: " << sizeof(t) << "\n";
    // cout << "size of r is: " << sizeof(r) << "\n";

    while (! (cin.eof() || cin.bad())) {
        int sum = 0;

        for (int i = 0; i < 3; i++) {
            cin >> t[3*i];
            cin >> t[3*i + 2];
            cin >> t[3*i + 1];

            if (cin.eof() || cin.bad())
                return 0;

            sum += t[3*i] + t[3*i + 2] + t[3*i + 1];
            // cout << "Bin " << i << ": B=" << t[3*i] << " C=" << t[3*i + 1] << " G=" << t[3*i + 2] << endl;
        }
        // cout << "  sum is: " << sum << endl;

        //for (int j = 0; j < 9; j++)
        //    cout << t[j] << " ";
        //cout << endl;

        int rsum = -1;

        for (int a = 0; a < 3; a++) {
            for (int b = 0; (b < 3); b++) {
                if (a != b) {
                    for (int c = 0; (c < 3); c++) {
                        if ((c != a) && (c != b)) {
                            int s = t[a] + t[3 + b] + t[6 + c];
                            if (s > rsum) {
                                r[0] = a;
                                r[1] = b;
                                r[2] = c;
                                rsum = s;
                                // cout << "a=" << a << ", b=" << b << ", c=" << c << " -> s=" << s << endl;
                            }
                        }
                    }
                }
            }
        }

        // cout << r[0] << " " << r[1] << " " << r[2] << ' ' << (sum - rsum) << endl;
        cout << symbols[r[0]] << symbols[r[1]] << symbols[r[2]] << ' ' << (sum - rsum) << endl;
    }
    
    return 0;
}
