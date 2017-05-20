#include <cstring>
#include <iostream>

const int MaxN = 20;
int  N = 0;
int  t[1+MaxN];  // nr kolejki (0+) => nr dziecka (1+)
char u[1+MaxN];  // nr dziecka (0+) => czy jeszcze gra

using namespace std;

/** absolute value */
template <class T> inline T abs(T x) {
  return ((0 <= x) ? x : (-x));
}

template <class T> T gcd(T a, T b) {
  if ((a < 0) || (b < 0))
    return gcd(abs(a), abs(b));
  if (a < b)
    return gcd(b,a);
  if (0 == b)
    return a;
  return gcd(b, a % b);
}

void init() {
  memset(t, 0, sizeof(int)*(1+MaxN));
  memset(u, 1, 1+MaxN);
}

void czytaj() {
  cin >> N;

  for (int i=0; i<N; ++i) {
    int j;
    cin >> j;
    t[j-1] = 1+i;
  }
}

long oblicz() {
  int kid = 0;  // nr dziecka ktore zaczyna (0+)
  long cm = 1;
  long kc = 0;

  for (int i=1; i<N; ++i) {
    const int k = t[i-1]-1;  // nr dziecka ktore odpada w tej kolejce (0+)
    const int n = N+1-i;     // liczba dzieci w tej kolejce (N..2)
    int d = 1;
    for (int j=kid; (k!=j); j=(j+1)%N) {
      if (u[j])
	++d;
    }
    d = d % n;

    // cerr << "Tura " << i << ": n=" << n << " kid=" << kid << " out=" << k << " d=" << d << endl;
    const long g = gcd(cm, (long) n);
    // cerr << "   g=" << g << endl;

    if (1==i)
      kc = (d ? d : n);
    else {
      const long h = gcd(n+kc-d, (long) n);
      // cerr << "   h=" << h << endl;
      if (h % g)
        return -1;
      else
        while (d != (kc % n))
          kc += cm;
    }
    cm = cm*n/g;

    // cerr << "  kc=" << kc << " cm=" << cm << endl;
    u[k] = 0;
    kid = (k+1) % N;
    if (i < N-1)
      for (; (! u[kid]); kid=(1+kid)%N);
  }

  // cerr << "Final result: " << kc << endl;
  return kc;
}

int main() {
  init();
  czytaj();

  const long result = oblicz();
  if (0 <= result)
    cout << result;
  else
    cout << "NIE";
  cout << endl;

  return 0;
}
