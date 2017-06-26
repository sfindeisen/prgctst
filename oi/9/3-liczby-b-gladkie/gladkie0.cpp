#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

const long MaxN = 2000000000;
const long MaxM =  100000000;
const long MaxB =    1000000;

long N = 0;
long M = 0;
long B = 0;

using namespace std;

// /**
//  * upperBound: upper bound (exclusive)
//  */
// vector<long> gen_primes(const long upperBound) {
//   vector<long> v;

//   if (1 <= upperBound) {
//     if (2 <= upperBound) {
//       v.push_back(2);
//       long sqr = 3;

//       for (long i=sqr; (i<upperBound); i+=2) {
//         for (; (sqr*sqr < i); ++sqr);
//         bool ok = true;
        
//         for (size_t j=0; ok && (j < v.size()) && (v[j] <= sqr); ++j) {
//           if (0 == (i % v[j]))
//             ok = false;
//         }

//         if (ok) {
//           v.push_back(i);
//           // if (0 == (v.size() % 10000))
//           // cerr << setw(7) << v.size() << " prime: " << i << endl;
//         }
//       }
//     }
//   }

//   return v;
// }

/**
 * Prime number generation up to the given number using Eratosthenes' sieve.
 *
 * @param upperBound upper limit for the prime value (exclusive)
 * @return the list of all primes in the range [2..upperBound) in increasing order.
 */
vector<long> gen_primes_sieve_bound(const long upperBound) {
  vector<long> primes;

  if (3 <= upperBound) {
    const long sz = (3+upperBound) / 2;

    // We are only storing odd numbers in the sieve: 3,5,7,9,11,13,15,...
    // 
    // vector<bool> will hopefully cause template specialization to
    // fire: http://www.cplusplus.com/reference/vector/vector-bool/
    // so this should be space optimized (1 bit per value).
    vector<bool> u(sz, true);

    for (long i=0; i<sz; ++i) {
      if (u[i]) {
        const long v = 3+2*i;
        for (long j=i+v; j<sz; j+=v)
          u[j] = false;
      }
    }

    // compute the final result
    primes.push_back(2);
    for (long i=0; i<sz; ++i) {
      if (u[i]) {
        const long v = 3+2*i;
        if (upperBound <= v)
          break;
        primes.push_back(v);
      }
    }
  }
  
  return primes;
}

inline bool is_smooth(const long query, const vector<long>& primes, const size_t pi, vector<bool>& smooth, vector<bool>&nosmooth) {
  const long sz = min(smooth.size(), nosmooth.size());

  if (query <= B) {
    if (query < sz)
      smooth[query] = true;
    return true;
  }

  if (pi < primes.size()) {
    const long p = primes[pi];
    const bool r = (query % p) ?
      is_smooth(query, primes, 1+pi, smooth, nosmooth) :
      is_smooth(query/p, primes, pi, smooth, nosmooth);

    if (query < sz)
      (r ? smooth : nosmooth)[query] = true;
    return r;
  } else {
    if (query < sz)
      nosmooth[query] = true;
    return false;
  }
}

int main() {
  cin >> N >> M >> B;
  long sm_counter = 0;

  if (1 == N) {
    ++N;
    --M;
    ++sm_counter;   // 1 is always smooth, regardless of B
  }

  if (2 <= B) {
    if (3 <= B) {
      // long upperBound = max(B, 10000000L);
      // std::vector<long> primes(gen_primes(1 + upperBound));
      std::vector<long> primes(gen_primes_sieve_bound(1+B));

      //cerr << "Found " << primes.size() << " primes in the range 0 to " << B << " (inclusive)." << endl;
      //for (vector<long>::const_iterator it = primes.begin(); it != primes.end(); ++it)
      //  cerr << " " << (*it);
      //cerr << endl;

      const long sz = min(100000000L, 1+N+M);  // 2 cache vectors, some 12.5 MB each
      vector<bool>   smooth(sz, false);
      vector<bool> nosmooth(sz, false);

      for (long i=N; i<=N+M; ++i) {
        if (is_smooth(i, primes, 0, smooth, nosmooth)) {
          ++sm_counter;
          //cerr << "  " << i << " is smooth" << endl;
        } else {
          //cerr << "  " << i << " is NOT smooth" << endl;
        }
      }
    } else {
      long c = 0;
      long p = 1;
      
      for (; p <  N;   p*=2);
      for (; p <= N+M; p*=2,++c);

      sm_counter = N+M-c;
    }
  } else {
    // B <= 1: none of the numbers are smooth (nothing to do)
  }

  cout << sm_counter << endl;
  return 0;
}
