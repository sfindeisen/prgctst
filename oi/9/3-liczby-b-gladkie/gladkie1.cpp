#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

#include <cmath>

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
template <class T> vector<T> gen_primes_sieve_bound(const T upperBound) {
  vector<T> primes;

  if (3 <= upperBound) {
    const T sz = (3+upperBound) / 2;

    // We are only storing odd numbers in the sieve: 3,5,7,9,11,13,15,...
    // 
    // vector<bool> will hopefully cause template specialization to
    // fire: http://www.cplusplus.com/reference/vector/vector-bool/
    // so this should be space optimized (1 bit per value).
    vector<bool> u(sz, true);

    for (T i=0; i<sz; ++i) {
      if (u[i]) {
        const T v = 3 + 2*i;
        for (T j=i+v; j<sz; j+=v)
          u[j] = false;
      }
    }

    // compute the final result
    primes.push_back(2);
    for (T i=0; i<sz; ++i) {
      if (u[i]) {
        const T v = 3 + 2*i;
        if (upperBound <= v)
          break;
        primes.push_back(v);
      }
    }
  }
  
  return primes;
}

long calc_smooth(const vector<long>& primes, size_t idx, long a, long b) {
  const long p = primes[idx];

  if (b < a) {
    return 0;
  } else if (a == b) {
    return 1;
  } else if (b <= p) {
    return (b-a+1);
  } else if (a <= p) {
    return (p-a+1) + calc_smooth(primes, idx, 1+p, b);
  } else {
    // Here (p < a <= b)
    const long k  = a / p;
    const long r  = a % p;             // remainder should be for free
    const long lo = k + (r ? 1 : 0);
    const long hi = b / p;

    long res = calc_smooth(primes, idx, lo, hi);

    for (size_t j = 1+idx; j < primes.size(); ++j) {
      res += calc_smooth(primes, j, a, b);
    }

    cerr << "calc_smooth: prime=" << p << " range: [" << a << ".." << b << "] => " res << endl;
    return res;
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

      cerr << "Found " << primes.size() << " primes in the range 0 to " << B << " (inclusive)." << endl;
      cerr << "iterator diff: " << (primes.end() - primes.begin()) << endl;
      for (vector<long>::const_iterator it = primes.begin(); it != primes.end(); ++it)
        cerr << " " << (*it);
      cerr << endl;

      sm_counter = calc_smooth(primes, 0, N, N+M);
    } else {
      // B == 2
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
