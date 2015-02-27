/**
 *  Bug
 *
 *  \date   2008-05-15
 *  \author STF
 */

#include <iostream>
#include <map>
#include <vector>

// =================================
// STF library
// =================================

namespace stf {

// =================================
// STF library: some basics
// =================================

/**
 *  Returns an integer less than, equal to, or greater than zero
 *  if t1 is found, respectively, to be less than, to match, or be greater than t2.
 */
template<class T> inline int simpleCmp(const T& t1, const T& t2) {
    if (t1 < t2)
        return -1;
    else if (t2 < t1)
        return 1;
    else
        return 0;
}

// TODO zrobic to lepiej?
template<class T> inline void swap(T& t1, T& t2) {
    T tmp(t1);
    t1 = t2;
    t2 = tmp;
}

// =======================================
// STF library: AbstractComparingContainer
// =======================================

template <class T> class AbstractComparingContainer {
    public:
        typedef int (*TCmp)(const T&, const T&);

        AbstractComparingContainer(TCmp comparator = &simpleCmp);
        AbstractComparingContainer(const AbstractComparingContainer<T>& another);

        virtual AbstractComparingContainer<T>& operator=(const AbstractComparingContainer<T>& another);
        virtual                               ~AbstractComparingContainer();

        virtual long        size()    const = 0;
        virtual bool        isEmpty() const;

    protected:
        inline int compare(const T& t1, const T& t2) {
            return (*comparator)(t1, t2);
        }

    private:
        TCmp comparator;
};

template <class T> AbstractComparingContainer<T>::AbstractComparingContainer(TCmp cmp) : comparator(cmp) {
}

template <class T> AbstractComparingContainer<T>::AbstractComparingContainer(const AbstractComparingContainer<T>& another) : comparator(another.comparator) {
}

template <class T> AbstractComparingContainer<T>& AbstractComparingContainer<T>::operator=(const AbstractComparingContainer<T>& another) {
    if (this != &another) {
        this->comparator = another.comparator;
    }
    return *this;
}

template <class T> AbstractComparingContainer<T>::~AbstractComparingContainer() {
}

template <class T> bool AbstractComparingContainer<T>::isEmpty() const {
    return (0 == size());
}

// =======================================
// STF library: AbstractHeap
// =======================================

template <class T> class AbstractHeap : public AbstractComparingContainer<T> {
    public:
        typedef int (*TCmp)(const T&, const T&);        // TODO po co to trzeba tu powtarzac??....

        AbstractHeap(TCmp comparator = &simpleCmp);
        AbstractHeap(const AbstractHeap<T>& another);

        virtual AbstractHeap<T>& operator=(const AbstractHeap<T>& another);
        virtual                 ~AbstractHeap();

        virtual void        push(T elem)    = 0;
        virtual T           pop()           = 0;
        virtual const T&    top()     const = 0;
        virtual long        size()    const = 0;
};

template <class T> AbstractHeap<T>::AbstractHeap(TCmp cmp)
    : AbstractComparingContainer<T>(cmp)
{ }

template <class T> AbstractHeap<T>::AbstractHeap(const AbstractHeap<T>& another)
    : AbstractComparingContainer<T>(another)
{ }

template <class T> AbstractHeap<T>& AbstractHeap<T>::operator=(const AbstractHeap<T>& another) {
    AbstractComparingContainer<T>::operator=(another);
    return *this;
}

template <class T> AbstractHeap<T>::~AbstractHeap() {
}

// =======================================
// STF library: ArrayHeap
// =======================================

template <class T> class ArrayHeap : public AbstractHeap<T> {
    // W sygnaturach metod obowiazuje indeksacja od 1
    public:
        typedef int (*TCmp)(const T&, const T&);        // TODO po co to trzeba tu powtarzac??....

        ArrayHeap(TCmp comparator = &simpleCmp);
        ArrayHeap(const ArrayHeap& another);

        virtual ArrayHeap<T>& operator=(const ArrayHeap& another);
        virtual              ~ArrayHeap();

        virtual void       push(T elem);
        virtual T          pop();
        virtual const T&   top()                  const;
        virtual long       size()                 const;
        virtual const T&   operator[](long index) const;

        virtual void         upheap(long index);    // Yes, these 2 methods are public for the user to be able
        virtual void       downheap(long index);    // to reassign elements' priorities and trigger the change.

        virtual bool       getHeapIndexTracking();
        virtual void       setHeapIndexTracking(bool);
        virtual long       getElementIndex(T elem);
    protected:
        virtual void       swap(long i, long j);
    private:
        std::vector<T>   contents;
        std::map<T,long> heapIndexMap;              // heap index tracking; indeksowanie od 1
        bool             heapIndexTracking;
};

template <class T> ArrayHeap<T>::ArrayHeap(TCmp comparator)
    : AbstractHeap<T>(comparator),
      heapIndexTracking(false)
{ }

template <class T> ArrayHeap<T>::ArrayHeap(const ArrayHeap<T>& another)
    : AbstractHeap<T>(another),
      contents(another.contents),
      heapIndexTracking(another.heapIndexTracking),
      heapIndexMap(another.heapIndexMap)
{ }

template <class T> ArrayHeap<T>& ArrayHeap<T>::operator=(const ArrayHeap<T>& another) {
    AbstractHeap<T>::operator=(another);

    if (this != &another) {
        this->contents          = another.contents;
        this->heapIndexMap      = another.heapIndexMap;
        this->heapIndexTracking = another.heapIndexTracking;
    }
    return *this;
}

template <class T> ArrayHeap<T>::~ArrayHeap() {
}

template <class T> void ArrayHeap<T>::push(T elem) {
    contents.push_back(elem);
    if (heapIndexTracking) {
        heapIndexMap[elem] = size();
    }
    upheap(size());
}

template <class T> T ArrayHeap<T>::pop() {
    T result(contents.front());

    if (2 <= size()) {
        contents[0] = contents.back();
        contents.pop_back();

        if (heapIndexTracking) {
            heapIndexMap.erase(result);
            heapIndexMap[contents[0]] = 1;
        }

        downheap(1);
    } else {
        // This is the last element
        contents.clear();
        heapIndexMap.clear();
    }

    return result;
}

template <class T> const T& ArrayHeap<T>::top() const {
    return contents.front();    // TODO exception?
}

template <class T> void ArrayHeap<T>::swap(long i, long j) {
    T tmp(contents[i - 1]);
    contents[i - 1] = contents[j - 1];
    contents[j - 1] = tmp;

    if (heapIndexTracking) {
        heapIndexMap[contents[i - 1]] = i;
        heapIndexMap[contents[j - 1]] = j;
    }
}

template <class T> void ArrayHeap<T>::upheap(long index) {
    long ih = 0;
    while ((2 <= index) && (compare(contents[index - 1], contents[(ih = (index/2)) - 1]) < 0)) {
        swap(index, ih);
        index = ih;
    }
}

template <class T> void ArrayHeap<T>::downheap(long index) {
    const long    sz = contents.size();
          long inext = 0;
    while (index <= sz) {
        inext = 2 * index;
        if ((inext  < sz) && (0 < compare(contents[inext - 1], contents[inext])))
            inext++;
        if ((inext <= sz) && (0 < compare(contents[index - 1], contents[inext - 1]))) {
            swap(index, inext);
            index = inext;
        } else
            break;
    }
}

template <class T> long ArrayHeap<T>::size() const {
    return contents.size();
}

template <class T> const T& ArrayHeap<T>::operator[](long index) const {
    return contents[index - 1];     // TODO exception?
}

template <class T> bool ArrayHeap<T>::getHeapIndexTracking() {
    return heapIndexTracking;
}

template <class T> void ArrayHeap<T>::setHeapIndexTracking(bool trace) {
    this->heapIndexTracking = trace;
}

template <class T> long ArrayHeap<T>::getElementIndex(T elem) {
    return heapIndexMap.find(elem)->second;      // TODO exception?
}

// =================================
// STF library: sorting
// =================================

template<class T>
class Sorting {
    public:
        typedef int (*TCmp)(const T& t1, const T& t2);
        static void  heapSort(T t[], long left, long right, TCmp cmp = simpleCmp);
        static void quickSort(T t[], long left, long right, TCmp cmp = simpleCmp);
};

template<class T> void Sorting<T>::heapSort(T t[], long left, long right, TCmp cmp) {
    if (left < right) {
        ArrayHeap<T> heap(cmp);
        for (long i = left; i <= right; i++)
            heap.push(t[i]);
        for (long i = left; i <= right; i++)
            t[i] = heap.pop();
    }
}

template<class T> void Sorting<T>::quickSort(T t[], long left, long right, TCmp cmp) {
    if (left < right) {
        long middle = (left + right) / 2;
        swap(t[left], t[middle]);               // podzial wzgledem srodkowego
        long m = left;

        for (long i = left + 1; i <= right; i++) {
            if (((*cmp)(t[i], t[left])) < 0)    // t[i] < t[left]
                swap(t[i], t[++m]);
        }

        swap(t[left], t[m]);
        quickSort(t, left,  m - 1, cmp);
        quickSort(t, m + 1, right, cmp);
    }
}

// =================================
// STF library: Math
// =================================

class Math {
    public:
        static long power(int a, int b);         // a^b
        static long power(int a, int b, long m); // a^b mod m
};

long Math::power(int a, int b) {
    if (0 == b)
        return 1;

     int bh = b / 2;
    long  p = power(a, bh);
    return (0 == (b % 2)) ? (p * p) : (p * p * ((long) a));
}

long Math::power(int a, int b, long m) {
    if (0 == b)
        return 1;

     int bh = b / 2;
    long  p = power(a, bh, m);
    return (0 == (b % 2)) ? ((p * p) % m) : ((p * p * ((long) a)) % m);
}

};   // namespace stf

// =================================
// Rozwiazanie zadania
// =================================

using namespace std;
using namespace stf;

typedef struct {
    long  firstNode;
    long secondNode;
    int      length;
} TEdge;

typedef struct {
    long  e1start;          // index do e1 (krawedzie wychodzace)
    long  e2start;          // index do e2 (krawedzie  wchodzace)
                            // liczbe krawedzi bierze sie z nastepnego wierzcholka

    long shortestPathEven;
    long shortestPathOdd;

    long  nextEven;
    long  nextOdd;

    short state;
    static const short STATE_DONE_EVEN = 1;   // shortestPathEven jest juz ostatecznie policzone
    static const short STATE_DONE_ODD  = 2;   // shortestPathOdd  jest juz ostatecznie policzone
    static const short STATE_HEAP_EVEN = 4;   // ten node jest na stosie    parzystym
    static const short STATE_HEAP_ODD  = 8;   // ten node jest na stosie nieparzystym

    bool isOnEvenHeap() {
        return (STATE_HEAP_EVEN == (state & STATE_HEAP_EVEN));
    }
    bool isOnOddHeap() {
        return (STATE_HEAP_ODD == (state & STATE_HEAP_ODD));
    }
    bool isDoneEven() {
        return (STATE_DONE_EVEN == (state & STATE_DONE_EVEN));
    }
    bool isDoneOdd() {
        return (STATE_DONE_ODD == (state & STATE_DONE_ODD));
    }
    void setEvenHeap(bool b) {
        if (b)
            state |= STATE_HEAP_EVEN;
        else
            state &= (~STATE_HEAP_EVEN);
    }
    void setOddHeap(bool b) {
        if (b)
            state |= STATE_HEAP_ODD;
        else
            state &= (~STATE_HEAP_ODD);
    }
    void setEvenDone() {
        state |= STATE_DONE_EVEN;
    }
    void setOddDone() {
        state |= STATE_DONE_ODD;
    }
} TNode;

const long MaxN       =     200000;   // maksymalna liczba miast
const long MaxM       =     500000;   // maksymalna liczba drog
const long MaxC       =       1000;   // maksymalna dlugosc pojedynczej krawedzi
const long MaxPathLen = 2000000000;   // wartownik

TNode t[MaxN + 2];
TEdge u[MaxM + 2];
long e1[MaxM + 2];          // indeksy do tablicy z krawedziamy posortowane po pierwszym koncu
long e2[MaxM + 2];          // indeksy do tablicy z krawedziamy posortowane po drugim    koncu

long N = 0;
long M = 0;

int nodeCompareOdd(const long& i1, const long& i2) {
    if (t[i1].shortestPathOdd < t[i2].shortestPathOdd)
        return -1;
    else if (t[i2].shortestPathOdd < t[i1].shortestPathOdd)
        return 1;
    else
        return 0;
}

int nodeCompareEven(const long& i1, const long& i2) {
    if (t[i1].shortestPathEven < t[i2].shortestPathEven)
        return -1;
    else if (t[i2].shortestPathEven < t[i1].shortestPathEven)
        return 1;
    else
        return 0;
}

void adjustNeighboringNode(ArrayHeap<long>& evenHeap, ArrayHeap<long>& oddHeap, long bestNode, long nnIndex, long shortestPath, long edge) {
    const bool         odd = (shortestPath % 2);
    const  int  edgeLength = u[edge].length;
    const bool     oddEdge = (edgeLength % 2);
    const long shorterPath = edgeLength + shortestPath;
    TNode& neighboringNode = t[nnIndex];

/*
    {
        // TODO remove this debug
        if (odd == oddEdge) {
            if (neighboringNode.isDoneEven())
                return;
        } else {
            if (neighboringNode.isDoneOdd())
                return;
        }

        cout << "  adjust? (" << ((odd == oddEdge) ? "EVEN" : " ODD") << "): [" << nnIndex << "]--(" << edgeLength << ")--[" << bestNode << "]~~(" << shortestPath << ")~~ ";
    }
*/
    if (odd == oddEdge) {
        if ((! neighboringNode.isDoneEven()) && (shorterPath < neighboringNode.shortestPathEven)) {
//          cout << "YES (" << shorterPath << ")";

            neighboringNode.shortestPathEven = shorterPath;
            neighboringNode.nextEven         = bestNode;

            if (neighboringNode.isOnEvenHeap()) {
                long heapPos = evenHeap.getElementIndex(nnIndex);
                evenHeap.upheap(heapPos);
            } else {
                neighboringNode.setEvenHeap(true);
                evenHeap.push(nnIndex);
            }
        }
    } else {
        if ((! neighboringNode.isDoneOdd()) && (shorterPath < neighboringNode.shortestPathOdd)) {
//          cout << "YES (" << shorterPath << ")";

            neighboringNode.shortestPathOdd = shorterPath;
            neighboringNode.nextOdd         = bestNode;

            if (neighboringNode.isOnOddHeap()) {
                long heapPos = oddHeap.getElementIndex(nnIndex);
                oddHeap.upheap(heapPos);
            } else {
                neighboringNode.setOddHeap(true);
                oddHeap.push(nnIndex);
            }
        }
    }

//  cout << '\n';
}

long oblicz() {
    stf::ArrayHeap<long>  oddHeap(&nodeCompareOdd);
    stf::ArrayHeap<long> evenHeap(&nodeCompareEven);

     oddHeap.setHeapIndexTracking(true);
    evenHeap.setHeapIndexTracking(true);

    // init Bitowice (even path of length 0):
    t[N - 1].shortestPathEven  = 0;
    t[N - 1].nextEven          = N - 1;
    t[N - 1].setEvenDone();
    t[N - 1].setEvenHeap(true);
    evenHeap.push(N - 1);

    while ((! oddHeap.isEmpty()) || (! evenHeap.isEmpty())) {
/*
        cout << "========  odd heap:";
        for (int i = 1; i <= oddHeap.size(); i++)
            cout << " [" << oddHeap[i] << "]";
        cout << '\n';

        cout << "======== even heap:";
        for (int i = 1; i <= evenHeap.size(); i++)
            cout << " [" << evenHeap[i] << "]";
        cout << '\n';
*/
        long shortestPathOdd  = MaxPathLen;
        long shortestPathEven = MaxPathLen;

        long bestNodeOdd  = -1;
        long bestNodeEven = -1;

        if (! oddHeap.isEmpty()) {
                bestNodeOdd = oddHeap.top();
            shortestPathOdd = t[bestNodeOdd].shortestPathOdd;
        }
        if (! evenHeap.isEmpty()) {
                bestNodeEven = evenHeap.top();
            shortestPathEven = t[bestNodeEven].shortestPathEven;
        }

        const bool          odd = (shortestPathOdd <= shortestPathEven);
        const long     bestNode = (odd ?     bestNodeOdd :     bestNodeEven);
        const long shortestPath = (odd ? shortestPathOdd : shortestPathEven);

/*
        {
            // TODO remove this debug
            if (! oddHeap.isEmpty()) {
                cout << " ODD best is: [" << bestNodeOdd << "]~~(" << shortestPathOdd << ")~~";
                if (odd)
                    cout << " [*]";
                cout << '\n';
            }
            if (! evenHeap.isEmpty()) {
                cout << "EVEN best is: [" << bestNodeEven << "]~~(" << shortestPathEven << ")~~";
                if (! odd)
                    cout << " [*]";
                cout << '\n';
            }
        }
*/

        if (odd) {
            oddHeap.pop();
            t[bestNode].setOddHeap(false);
            t[bestNode].setOddDone();

            if (0 == bestNode) {
                // cout << "######## FINAL RESULT = " << shortestPath << '\n';
                return shortestPath;
            }
        } else {
            evenHeap.pop();
            t[bestNode].setEvenHeap(false);
            t[bestNode].setEvenDone();
        }

        for (long e1i = t[bestNode].e1start; ((e1i < M) && (bestNode == u[e1[e1i]].firstNode)); e1i++) {
            long neighNIndex = u[e1[e1i]].secondNode;
            adjustNeighboringNode(evenHeap, oddHeap, bestNode, neighNIndex, shortestPath, e1[e1i]);
        }
        for (long e2i = t[bestNode].e2start; ((e2i < M) && (bestNode == u[e2[e2i]].secondNode)); e2i++) {
            long neighNIndex = u[e2[e2i]].firstNode;
            adjustNeighboringNode(evenHeap, oddHeap, bestNode, neighNIndex, shortestPath, e2[e2i]);
        }
    }

    return 0;
}

void init() {
    memset(t,  0, sizeof(t));
    memset(u,  0, sizeof(u));
    memset(e1, 0, sizeof(e1));
    memset(e2, 0, sizeof(e2));
}

void czytaj() {
    long a = 0;
    long b = 0;
    long c = 0;

    cin >> N >> M;
    // cout << "Liczba miast: " << N << "; liczba drog: " << M << '\n';

    for (long i = 0; i < M; i++) {
        cin >> a >> b >> c;

        u[i].firstNode  = (a - 1);
        u[i].secondNode = (b - 1);
        u[i].length     = c;

        e1[i] = i;
        e2[i] = i;
    }
}

inline int edgeCmpFirstNode(const long& i, const long& j) {
    long in = u[i].firstNode;
    long jn = u[j].firstNode;

    if (in < jn)
        return -1;
    else if (jn < in)
        return 1;
    else
        return 0;
}

inline int edgeCmpSecondNode(const long& i, const long& j) {
    long in = u[i].secondNode;
    long jn = u[j].secondNode;

    if (in < jn)
        return -1;
    else if (jn < in)
        return 1;
    else
        return 0;
}

void inicjalizuj_graf() {
    // cout << "Sortowanie krawedzi....\n";

    Sorting<long>::heapSort(e1, 0, M - 1, &edgeCmpFirstNode);
    Sorting<long>::heapSort(e2, 0, M - 1, &edgeCmpSecondNode);

    // wpisywanie indeksow:
    long e1i = 0, e2i = 0;
    for (long i = 0; i < N; i++) {
        t[i].shortestPathEven = MaxPathLen;
        t[i].shortestPathOdd  = MaxPathLen;
        t[i].e1start          = e1i;    // aczkolwiek moze byc 0 krawedzi
        t[i].e2start          = e2i;    // j.w.

        while ((e1i < M) && (i == u[e1[e1i]].firstNode))
            e1i++;
        while ((e2i < M) && (i == u[e2[e2i]].secondNode))
            e2i++;
    }
}

/*
void wypisz_graf() {
    cout << "Liczba miast: " << N << "; liczba drog: " << M << '\n';

    cout << "t:\n";
    for (long i = 0; i < N; i++) {
        cout << "  i=" << i << ", e1ind=" << t[i].e1start << ", e2ind=" << t[i].e2start << '\n';
    }

    cout << "u:\n";
    for (long i = 0; i < M; i++) {
        cout << "  i=" << i << ":  [" << u[i].firstNode << "]--(" << u[i].length << ")--[" << u[i].secondNode << "]\n";
    }

    cout << "e1:\n";
    for (long i = 0; i < M; i++) {
        cout << "  i=" << i << ",  ind=" << e1[i] << "  firstNode=" << u[e1[i]].firstNode << '\n';
    }

    cout << "e2:\n";
    for (long i = 0; i < M; i++) {
        cout << "  i=" << i << ",  ind=" << e2[i] << "  secondNode=" << u[e2[i]].secondNode << '\n';
    }
}
*/

// ======================
// kilka testow....
// ======================

/*
void init_random(int t[], int count) {
    for (int i = 0; i < count; i++) {
        t[i] = random() % 20;
    }
}

void wypisz(int t[], int count) {
    const int RANGE = 20;
    int occ[RANGE];
    memset(occ, 0, sizeof(occ));

    printf("   ");
    for (int i = 0; i < count; i++) {
        printf(" [%2d]", t[i]);
        if ((0 <= t[i]) && (t[i] < RANGE))
            occ[t[i]]++;
    }
    cout << '\n';

    int occt = 0;
    for (int i = 0; i < count; i++) {
        printf(" occ[%2d] => %2d\n", i, occ[i]);
        occt += occ[i];
    }
    printf(" (%2d occurences total)\n", occt);
}

void testuj_qsort() {
    const int SIZE = 20;
    int t[SIZE];
    init_random(t, SIZE);
    wypisz(t, SIZE);
    Sorting<int>::quickSort(t, 0, SIZE - 1);
    wypisz(t, SIZE);
}

void testuj_hsort() {
    const int SIZE = 20;
    int t[SIZE];
    init_random(t, SIZE);
    wypisz(t, SIZE);
    Sorting<int>::heapSort(t, 0, SIZE - 1);
    wypisz(t, SIZE);
}

void testuj() {
    srandom(time(NULL));
    // testuj_qsort();
    testuj_hsort();
}
*/

int main() {
    init();
    // testuj();
    czytaj();
    // wypisz_graf();
    inicjalizuj_graf();
    // wypisz_graf();

    long result = oblicz();
    cout << result << '\n';

    return 0;
}
