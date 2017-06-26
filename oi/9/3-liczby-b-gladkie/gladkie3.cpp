#include <algorithm>
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

std::vector<long> primes;

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
 * in the range [fromInc..toInc] (both inclusive).
 */
long count_smooth(size_t pos1, long fromInc, long toInc) {
  cerr << "count_smooth: primes[" << pos1 << "]=" << ((1<=pos1) ? primes[pos1-1] : 1) << ", [ " << fromInc << ".." << toInc << "]" << endl;
  if (toInc < fromInc)
    return 0;

  if (2 == toInc)
    return ((fromInc <= 1) ? 1 : 0) + ((1 <= pos1) ? 1 : 0);
  else if (1 == toInc)
    return 1;
  else if (0 == toInc)
    return 0;

  if (1 <= pos1) {
    const long p  = primes[pos1-1];

    if (toInc <= p)
      return (toInc - fromInc + 1);
    else if (fromInc <= p)
      return (p - fromInc + 1) + count_smooth(pos1, 1+p, toInc);

    const long r1 = (fromInc % p);
    const long d1 = (fromInc / p) + ((0==r1) ? 0 : 1);
    const long d2 =    toInc / p;

    if ((d2 < d1) && (4 <= pos1)) {
      const size_t ub = upper_bound(primes.begin(), primes.begin() + (pos1-1), toInc/(1+d2)) - primes.begin();
      return count_smooth(ub, fromInc, toInc);
    } else {
      return count_smooth(pos1, d1, d2) + count_smooth(pos1-1, fromInc, toInc);
    }
  } else {
    return (((fromInc <= 1) && (1 <= toInc)) ? 1 : 0);
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
      primes = gen_primes_sieve_bound(1+B);

      // cerr << "Found " << primes.size() << " primes in the range 0 to " << B << " (inclusive)." << endl;
      // for (vector<long>::const_iterator it = primes.begin(); it != primes.end(); ++it)
      //   cerr << " " << (*it);
      // cerr << endl;

      sm_counter = count_smooth(primes.size(), N, N+M);
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
