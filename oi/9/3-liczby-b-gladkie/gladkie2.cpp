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

/**
 * Returns the number of primes[pos1-1] -smooth numbers
 * in the range [1..upinc] (both inclusive).
 */
inline long count_smooth(const std::vector<long>& primes, size_t pos1, long upinc) {
  // cerr << "count_smooth up to " << upinc << " pos1=" << pos1 << endl;

  if (upinc <= 2) {
    if (upinc <= 1)
      return upinc;
    return 1 + ((1 <= pos1) ? 1 : 0);
  }

  if (1 <= pos1) {
    const long p = primes[pos1-1];
    const long d = upinc / p;
    const long k = count_smooth(primes, pos1, d) + count_smooth(primes, pos1-1, upinc);
    // cerr << "count_smooth up to " << upinc << " using fac " << pos1 << "/" << primes[pos1-1] << " => " << k << endl;
    return k;
  } else {
    return 1;
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
      std::vector<long> primes(gen_primes_sieve_bound(1+B));

      // cerr << "Found " << primes.size() << " primes in the range 0 to " << B << " (inclusive)." << endl;
      // for (vector<long>::const_iterator it = primes.begin(); it != primes.end(); ++it)
      //   cerr << " " << (*it);
      // cerr << endl;

      sm_counter = count_smooth(primes, primes.size(), N+M) - count_smooth(primes, primes.size(), N-1);
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
