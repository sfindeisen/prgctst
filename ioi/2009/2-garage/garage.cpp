#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

int N;     // number of parking places
int M;     // number of cars

const int MaxN = 100;
const int MaxM = 2000;

int p[1+MaxN];      // parking place rates (0-based)
int w[1+MaxM];      // car weights (0-based)
int m[1+MaxM];      // car (0-based) -> place (0-based)

// cars queue (0-based)
std::queue<int> cars_q;
// free parking places (0-based)
std::priority_queue<int, std::vector<int>, std::greater<int> > free_places;

long total=0;

void park(int k) {
    int j = free_places.top();
    free_places.pop();
    m[k] = j;
    total += (long) w[k] * (long) p[j];
}

int main() {
    std::cin >> N >> M;

    memset(p, 0, sizeof(p));
    memset(w, 0, sizeof(w));
    memset(m, 0, sizeof(m));

    for (int i=0; i<N; ++i)
        std::cin >> p[i];
    for (int j=0; j<M; ++j)
        std::cin >> w[j];
    for (int i=0; i<N; ++i)
        free_places.push(i);

    int k;
    for (int i=0; i<2*M; ++i) {
        std::cin >> k;

        if (0 < k) {
            --k;
            if (free_places.empty()) {
                cars_q.push(k);
            } else {
                park(k);
            }
        } else if (k < 0) {
            free_places.push(m[-k-1]);
            if (! cars_q.empty()) {
                park(cars_q.front());
                cars_q.pop();
            }
        }
    }

    std::cout << total << std::endl;
    return 0;
}
