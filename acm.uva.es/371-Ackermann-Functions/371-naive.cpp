#include <iostream>

int n;

using namespace std;

unsigned int gen_seq(unsigned int k) {
  unsigned int steps=0;

  do {
    if (k%2)
      k = 3*k + 1;
    else
      k >>= 1;
    ++steps;
  } while (1 != k);

  return steps;
}

int main() {
  cin >> n;
  cout << gen_seq(n) << endl;
  return 0;
}

