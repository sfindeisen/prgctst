#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>
#include <stack>
#include <vector>

struct trec {
  size_t pos1;     // prime 1-index
  long fromInc;
  long   toInc;

  trec(size_t pos1, long fromInc, long toInc) : pos1(pos1), fromInc(fromInc), toInc(toInc) { }
};

const long MaxN = 2000000000;
const long MaxM =  100000000;
const long MaxB =    1000000;

long N = 0;
long M = 0;
long B = 0;

std::vector<long> primes;
std::set<long> pcache;    // numbers >B, not divisible by [2..B].
std::stack<trec> rst;

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

/** (2 <= p) */
bool is_smooth(const long p, const long query) {
  if (query <= p)
    return true;
  if (0 == (query % 2))
    return is_smooth(p, query >> 1);

  const long maxp = *(primes.rbegin());

  if ((query <= maxp) && binary_search(primes.begin(), primes.end(), query)) {
    return false;
  } else if (pcache.count(query)) {
    return false;
  } else {
    for (size_t i = 0; ((i < primes.size()) && ((primes[i]*primes[i]) <= query)); ++i) {
      if (0 == (query % primes[i])) {
        const long d = query / primes[i];
        return is_smooth(p, d);
      }
    }
    pcache.insert(query);
    return false;
  }
}

/**
 * Returns the number of primes[pos1-1] -smooth numbers
 * in the range [fromInc..toInc] (both inclusive).
 */
long count_smooth(const size_t _pos1, const long _fromInc, const long _toInc) {
  rst.push(trec(_pos1, _fromInc, _toInc));
  long res = 0;

  while (! rst.empty()) {
    const trec r(rst.top());
    rst.pop();

    // cerr << "count_smooth: primes[" << pos1 << "]=" << ((1<=pos1) ? primes[pos1-1] : 1) << ", [ " << fromInc << ".." << toInc << "]" << endl;
    if (r.toInc < r.fromInc)
      continue;

    if (2 == r.toInc) {
      res += ((r.fromInc <= 1) ? 1 : 0) + ((1 <= r.pos1) ? 1 : 0);
      continue;
    } else if (1 == r.toInc) {
      res += 1;
      continue;
    } else if (0 == r.toInc)
      continue;

    if (1 <= r.pos1) {
      const long p = primes[r.pos1-1];

      if (r.toInc <= p) {
        res += (r.toInc - r.fromInc + 1);
        continue;
      } else if (r.fromInc <= p) {
        res += (p - r.fromInc + 1);
        rst.push(trec(r.pos1, 1+p, r.toInc));
        continue;
      }
    
      if (r.toInc <= r.fromInc + 4) {
        for (long j=r.fromInc; j<=r.toInc; ++j) {
          if (is_smooth(p,j))
            ++res;
        }
        continue;
      }

      const long r1 = (r.fromInc % p);
      const long d1 = (r.fromInc / p) + ((0==r1) ? 0 : 1);
      const long d2 =    r.toInc / p;

      if ((d2 < d1) && (3 <= r.pos1)) {
        const size_t ub = upper_bound(primes.begin(), primes.begin() + (r.pos1-1), r.toInc/(1+d2)) - primes.begin();
        rst.push(trec(ub, r.fromInc, r.toInc));
      } else {
        rst.push(trec(r.pos1, d1, d2));
        rst.push(trec(r.pos1-1, r.fromInc, r.toInc));
      }
    } else {
      res += (((r.fromInc <= 1) && (1 <= r.toInc)) ? 1 : 0);
      continue;
    }
  }

  return res;
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
