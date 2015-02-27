#include <iostream>
#include <cstring>

const int MaxN = 1000;
const int MaxM = 1000;

typedef struct {
    int  axFromInc;
    int  axToInc;
    int  axMax;
    long axSum;

    int  ileft;
    int  iright;
} TNode;

int xx[MaxM + 1];
int yy[MaxN + 1];

TNode nodes[MaxM + MaxN + 5];
int nodec = 0;

int xxRoot = 0;
int yyRoot = 0;
int M = 0;
int N = 0;

using namespace std;

void init() {
    memset(xx,    0, sizeof(xx));
    memset(yy,    0, sizeof(yy));
    memset(nodes, 0, sizeof(nodes));
}

void czytaj() {
    cin >> M >> N;

    for (int i = 1; i < M; i++) {
        cin >> xx[i];
    }
    for (int j = 1; j < N; j++) {
        cin >> yy[j];
    }
}

int buildTree(int axes[], int axFromInc, int axToInc, bool force=false) {
    if ((axFromInc < axToInc) || force) {
        const int nIndex   = ++nodec;
        const int axMiddle = (axFromInc + axToInc) / 2;

        TNode *p = nodes + nIndex;
        p->axFromInc = axFromInc;
        p->axToInc   = axToInc;
        p->ileft     = buildTree(axes, axFromInc,    axMiddle);
        p->iright    = buildTree(axes, axMiddle + 1, axToInc);

        int  axMax = max(axes[axMiddle], axes[axToInc]);
        long axSum = 0;

        if (p->ileft) {
            axMax  = max(axMax, nodes[p->ileft].axMax);
            axSum += nodes[p->ileft].axSum;
        } else if (axFromInc)
            axSum += axes[axFromInc];

        if (p->iright) {
            axMax  = max(axMax, nodes[p->iright].axMax);
            axSum += nodes[p->iright].axSum;
        } else if (axToInc && (axFromInc != axToInc))
            axSum += axes[axToInc];

        p->axMax = axMax;
        p->axSum = axSum;

        // cout << " bt: " << nIndex << " :: [" << axFromInc << ".." << axToInc << "] :: max=" << axMax << " sum=" << axSum << endl;
        return nIndex;
    }

    return 0;
}

int findMaxAxis(int inode, int axes[], int axFromInc, int axToInc) {
    TNode *p = nodes + inode;

    if (axToInc < p->axFromInc)
        return 0;
    if (p->axToInc < axFromInc)
        return 0;

    int axisSel = 0;
    if ((axFromInc <= p->axFromInc) && (p->axFromInc <= axToInc))
        axisSel = p->axFromInc;
    if (((axFromInc <= p->axToInc) && (p->axToInc <= axToInc)) && ((!axisSel) || (axes[axisSel] < axes[p->axToInc])))
        axisSel = p->axToInc;

    if (p->ileft) {
        int k = findMaxAxis(p->ileft, axes, axFromInc, axToInc);
        if (k && ((!axisSel) || (axes[axisSel] < axes[k])))
            axisSel = k;
    }

    if (p->iright) {
        int k = findMaxAxis(p->iright, axes, axFromInc, axToInc);
        if (k && ((!axisSel) || (axes[axisSel] < axes[k])))
            axisSel = k;
    }

    //cout << "fmx: inode " << inode << " has: [" << (p->axFromInc) << ".." << (p->axToInc) << "] search: [" << axFromInc << ".." << axToInc << "] ==> " << axisSel << "->" << axes[axisSel] << endl;
    return axisSel;
}

long axesSum(int inode, int axes[], int axFromInc, int axToInc) {
    //cout << " as: " << inode << " :: [" << axFromInc << ".." << axToInc << "]" << endl;
/*
    long axSum = 0;
    for (int i = axFromInc; i <= axToInc; i++)
        axSum += axes[i];
    return axSum;
*/
    if (axToInc < axFromInc)
        return 0;

    TNode *p = nodes + inode;

    if (axToInc < p->axFromInc)
        return 0;
    if (p->axToInc < axFromInc)
        return 0;

    if ((axFromInc <= p->axFromInc) && (p->axToInc <= axToInc)) {
        //cout << " as: " << inode << " :: [" << axFromInc << ".." << axToInc << "] :: " << p->axSum << " (fast)" << endl;
        return p->axSum;
    }

    long axSum = 0;

    if (p->ileft)
        axSum += axesSum(p->ileft, axes, axFromInc, axToInc);
    else
    if ((axFromInc <= p->axFromInc) && (p->axFromInc <= axToInc))
        axSum += axes[p->axFromInc];

    if (p->iright)
        axSum += axesSum(p->iright, axes, axFromInc, axToInc);
    else
    if ((axFromInc <= p->axToInc) && (p->axToInc <= axToInc))
        axSum += axes[p->axToInc];

    //cout << " as: " << inode << " :: [" << axFromInc << ".." << axToInc << "] :: " << axSum << endl;
    return axSum;
}

long kosztDzielenia(int chxFromInc, int chxToInc, int chyFromInc, int chyToInc) {
    //cout << " kd: [" << chxFromInc << ".." << chxToInc << "][" << chyFromInc << ".." << chyToInc << "]" << endl;

    if (chxFromInc == chxToInc)
        return axesSum(yyRoot, yy, chyFromInc, chyToInc-1);
    if (chyFromInc == chyToInc)
        return axesSum(xxRoot, xx, chxFromInc, chxToInc-1);

    int maxAxisX = findMaxAxis(xxRoot, xx, chxFromInc, chxToInc-1);
    //cout << "mxx: " << maxAxisX << endl;
    int maxAxisY = findMaxAxis(yyRoot, yy, chyFromInc, chyToInc-1);
    //cout << "mxy: " << maxAxisY << endl;
    //sleep(1);

    long koszt = 0;
    if (xx[maxAxisX] <= yy[maxAxisY]) {
        koszt = yy[maxAxisY] +
                    kosztDzielenia(chxFromInc, chxToInc, chyFromInc, maxAxisY) +
                    kosztDzielenia(chxFromInc, chxToInc, maxAxisY+1, chyToInc);
    } else {
        koszt = xx[maxAxisX] +
                    kosztDzielenia(chxFromInc, maxAxisX, chyFromInc, chyToInc) +
                    kosztDzielenia(maxAxisX+1, chxToInc, chyFromInc, chyToInc);
    }

    //cout << " kd: [" << chxFromInc << ".." << chxToInc << "][" << chyFromInc << ".." << chyToInc << "] -> " << koszt << endl;
    return koszt;
}

void initTrees() {
    xxRoot = buildTree(xx, 1, M-1, true);
    //cout << "*** buildTree(xx) -> " << xxRoot << endl;
    yyRoot = buildTree(yy, 1, N-1, true);
    //cout << "*** buildTree(yy) -> " << yyRoot << endl;
}

int main() {
    init();
    czytaj();
    initTrees();

    long result = kosztDzielenia(1, M, 1, N);
    cout << result << endl;
}

