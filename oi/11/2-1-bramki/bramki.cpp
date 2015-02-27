#include <iostream>

const long MaxN =  10000;
      long    N =      0;
const long MaxE = 200000;

/**
 * Stany bramek (zadanie : program):
 *    0 : 0
 *  1/2 : 1
 *    1 : 2
 */

typedef struct {
    char state;
    long statePredCount[3];
    char phaseState[3];

    long predStart;
    long predCount;

    long succStart;
    long succCount;
} TBramka;

TBramka bramki[1 + MaxN];
long pred[1 + MaxE];
long succ[1 + MaxE];

using namespace std;

/*
void debug() {
    for (long i = 0; i < N; i++) {
        cout << "Bramka " << i << ": predCount=" << bramki[i].predCount << ", succCount=" << bramki[i].succCount << endl;

        cout << "  pred = ";
        for (long j = 0; j < bramki[i].predCount; j++) {
            cout << pred[bramki[i].predStart + j] << " ";
        }
        cout << endl;

        cout << "  succ = ";
        for (long j = 0; j < bramki[i].succCount; j++) {
            cout << succ[bramki[i].succStart + j] << " ";
        }
        cout << endl;
    }
}
*/

void init() {
    memset(bramki, 0, sizeof(bramki));
    memset(pred,   0, sizeof(pred));
    memset(succ,   0, sizeof(succ));
}

void czytaj() {
    {
        // wczytywanie predecessors

        long pTotal = 0;
        long p = 0;

        cin >> N;
        for (long i = 2; i < N; i++) {
            bramki[i].predStart = pTotal;

            cin >> bramki[i].predCount;
            for (long j = 0; j < bramki[i].predCount; j++) {
                cin >> p;
                pred[pTotal++] = p;
                bramki[p].succCount++;
            }
        }
    }

    {
        // obliczanie successors

        long sTotal = 0;
        for (long i = 0; i < N; i++) {
            bramki[i].succStart = sTotal;
            sTotal += bramki[i].succCount;
            bramki[i].succCount = 0;
        }

        for (long i = 0; i < N; i++) {
            for (long j = 0; j < bramki[i].predCount; j++) {
                long p = pred[bramki[i].predStart + j];
                succ[bramki[p].succStart + bramki[p].succCount++] = i;
            }
        }
    }
}

/** ustawia default state na wszystkich bramkach */
void ustaw_all(int v) {
    // cout << "--- ustaw all: " << v << endl;

    for (long i = 0; i < N; i++) {
        bramki[i].state = v;
        bramki[i].statePredCount[0] = 0;
        bramki[i].statePredCount[1] = 0;
        bramki[i].statePredCount[2] = 0;
        bramki[i].statePredCount[v] = bramki[i].predCount;
    }
}

void ustaw_predefined() {
    bramki[0].state = 0;
    bramki[1].state = 2;
}

/** defState - default state in this phase */
void savePhase(int defState) {
    for (long i = 0; i < N; i++) {
        bramki[i].phaseState[defState] = bramki[i].state;
    }
}

bool checkStateChange(long i) {
     int v = bramki[i].state;

    long state0 = bramki[i].statePredCount[0];
    long state2 = bramki[i].statePredCount[2];

    if ((state0 > state2) && (0 != v)) {
        bramki[i].state = 0;
        return true;
    } else if ((state0 < state2) && (2 != v)) {
        bramki[i].state = 2;
        return true;
    } else if ((state0 == state2) && (1 != v)) {
        bramki[i].state = 1;
        return true;
    }

    return false;
}

/** propaguje stan i-tej bramki wprzod; oldState - poprzedni stan i-tej bramki */
void propagate_fwd(int oldState, long i) {
    int curState = bramki[i].state;
    // cout << "propagate_fwd: bramka " << i << ": " << oldState << " => " << curState << endl;

    for (long j = 0; j < bramki[i].succCount; j++) {
        long s = succ[bramki[i].succStart + j];
        bramki[s].statePredCount[oldState]--;
        bramki[s].statePredCount[curState]++;
    }

    for (long j = 0; j < bramki[i].succCount; j++) {
        long s = succ[bramki[i].succStart + j];
         int sState = bramki[s].state;
        if (checkStateChange(s))
            propagate_fwd(sState, s);
    }
}

void oblicz() {
    ustaw_all(0);
    ustaw_predefined();
    propagate_fwd(0, 1);
    savePhase(0);

    ustaw_all(2);
    ustaw_predefined();
    propagate_fwd(2, 0);
    savePhase(2);

    ustaw_all(1);
    ustaw_predefined();
    propagate_fwd(1, 0);
    propagate_fwd(1, 1);
    savePhase(1);
}

void pisz() {
    for (long i = 0; i < N; i++) {
        char p0 = bramki[i].phaseState[0];
        char p1 = bramki[i].phaseState[1];
        char p2 = bramki[i].phaseState[2];

        if ((p0 == p1) && (p1 == p2)) {
            switch (p0) {
                case 0:
                    cout << '0' << endl;
                    break;
                case 1:
                    cout << "1/2" << endl;
                    break;
                case 2:
                    cout << '1' << endl;
                    break;
            }
        } else {
            cout << '?' << endl;
        }
    }
}

int main() {
    // cout << "sizeof(bramki)=" << sizeof(bramki) << endl;

    init();
    czytaj();
    // debug();
    oblicz();
    pisz();

    return 0;
}

