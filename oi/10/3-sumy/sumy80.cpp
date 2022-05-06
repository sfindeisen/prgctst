#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>

const unsigned long MaxN   =  5000;   // maximal number of generators
const unsigned long MaxGen = 50000;   // maximal generator

unsigned long N = 0;
unsigned long K = 0;
unsigned long G = 0;
unsigned long MinGen =  0;            // smallest generator
unsigned long last0  = -1;            // max un-generable number
const unsigned long bufsz = 3 + MaxGen;
char buffer[bufsz];                   // working buffer
unsigned long current = 0;            // current integer

std::vector<unsigned long> gen0;
std::vector<unsigned long> gen1;

std::vector<unsigned long> query;
std::vector<unsigned long> qind;
std::vector<unsigned long> qres;

using namespace std;

void init() {
  memset(buffer, 0, bufsz);  
}

void czytaj() {
  unsigned long j = 0;

  // wczytaj zbior A (generatory)
  cin >> N;
  gen0.reserve(N);

  for (unsigned long i=0; i<N; ++i) {
    cin >> j;
    gen0.push_back(j);
    if (0 == i)
      MinGen = j;
  }

  // wczytaj zbior B (zapytania)
  cin >> K;

  query.reserve(K);
  qind.reserve(K);
  qres.reserve(K);

  for (unsigned long i=0; i<K; ++i) {
    cin >> j;
    query.push_back(j);
    qind.push_back(i);
    qres.push_back(0);
  }
}

unsigned long gcd(unsigned long a, unsigned long b) {
  if (a < b)
    return gcd(b,a);
  if (0 == b)
    return a;
  return gcd(b, a % b);
}


void oblicz_gcd() {
  G = gen0[0];
  for (unsigned long i=1; i<N; ++i)
    G = gcd(G, gen0[i]);
  if (2 <= G) {
    for (unsigned long i=0; i<N; ++i)
      gen0[i] /= G;
  }
}

void oblicz_gen1() {
  const unsigned long max0 = gen0[N-1];
  buffer[0]   =  1;
  last0       = -1;
  size_t ptr0 =  0;

  for (current=1; (current <= max0) && (ptr0 < N); ++current) {
    for (const unsigned long g1 : gen1) {
      if (buffer[current-g1]) {
        buffer[current] = 1;
        break;
      }
    }

    if (buffer[current]) {
      // cur wygenerowane z gen1
      // ponizszy generator jest redundantny
      if (gen0[ptr0] == current) {
        // cerr << "gen0 skip: " << gen0[ptr0] << endl;
        ++ptr0;
      }
    } else {
      if (gen0[ptr0] == current) {
        buffer[current] = 1;
        gen1.push_back(gen0[ptr0]);
        // cerr << "gen1  add: " << gen0[ptr0] << endl;
        ++ptr0;
      } else {
        last0 = current;
      }
    }
  }

  // gen0.clear();
  N = gen1.size();
  --current;
}

inline bool mycmp (const unsigned long i, const unsigned long j) {
    return (query[i] < query[j]);
}

void generuj() {
  unsigned long bcur = current;
  bool all = false;

  sort (qind.begin(), qind.end(), mycmp);

  for (unsigned long i=0; i<K; ++i) {
    unsigned long q = query[qind[i]];
    if (2 <= G) {
      if (q % G)
        continue;     // no way to generate q
      q /= G;
    }
    // cerr << "query: " << q << endl;

    if ((1 <= i) && (query[qind[i-1]] == query[qind[i]])) {
      qres[qind[i]] = qres[qind[i-1]];
      continue;
    }

    if (q <= current) {
      qres[qind[i]] = buffer[q];
      continue;
    }

    while ((!all) && (current < q)) {
      if (bufsz == ++bcur)
        bcur = 0;
      ++current;
      buffer[bcur] = 0;

      for (unsigned long j=0; (j<N) && (! buffer[bcur]); ++j) {
        unsigned long y = bufsz + bcur - gen1[j];
        if (bufsz <= y)
          y -= bufsz;
        if (buffer[y]) {
          buffer[bcur] = 1;
        }
      }

      if (buffer[bcur]) {
        // cerr << "current=" << current << " => ok" << endl;
        if (gen1[0] <= (current-last0)) {
          // cerr << "current=" << current << " last0=" << last0 << " ====>> ALL TRUE" << endl;
          all = true;     // everything can be generated from now on
        }
      } else {
        // cerr << "current=" << current << " => .." << endl;
        last0 = current;
      }
    }

    if (all || buffer[bcur]) {
      qres[qind[i]] = 1;
    }
  }
}

void wynik() {
  for (unsigned long i=0; i<K; ++i)
    cout << (qres[i] ? "TAK" : "NIE") << endl;
}

int main() {
  init();
  czytaj();

  if (1 == gen0[0]) {
    for (unsigned long i=0; i<K; ++i)
      qres[i] = 1;
  } else if (1 == N) {
    for (unsigned long i=0; i<K; ++i)
      qres[i] = ((query[i]) % (gen0[0])) ? 0 : 1;
  } else {
    oblicz_gcd();
    oblicz_gen1();
    generuj();
  }

  wynik();
  return 0;
}
