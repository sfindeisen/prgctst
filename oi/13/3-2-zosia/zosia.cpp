/** author: STF */

#include <iostream>
#include <cstring>
#include <queue>
#include <set>

const long MaxN       = 1000000;
const long MaxE       = 3000000;
const long MaxE2      = 2 * MaxE;
      long N          = 0;     // initial number of children (nodes)
      long E          = 0;     // initial number of edges
const long MaxNRLimit = 10;    // max max number of nodes to remove
      long    NRLimit = 0;     // max number of nodes to remove
      long    NR      = 0;     // number of removed nodes
const long MaxERLimit = MaxNRLimit * MaxNRLimit;    // max number of edges to remove in brute-force phase
const long MaxN_bf    = 2 * MaxERLimit;     // maximum number of nodes in brute-force phase
      long    ER      = 0;     // number of removed edges

typedef struct {
    long nn;            // neighbour node number
    long backPtr;       // address of this node entry in nn's predecessors/successors list
    long bfEdgeIndex;   // bfEdges index
    long prev;          // prev neighbour (or 0) (doubly linked list)
    long next;          // next neighbour (or 0) (doubly linked list)
} TNodeNeighbour;

typedef struct {
    long firstItem;     // first list item (or 0)
    long  lastItem;     //  last list item (or 0)
    long  itemCount;    //       list item count
} TDoubleLinkedList;

typedef struct {
    char  deleted;
    long  connCompo;       // connected component ID [1..connCompoCount]

    TDoubleLinkedList predList;
    TDoubleLinkedList succList;
} TNode;

typedef struct {
    long nodeCount;
    long edgeCount;
} TConnectedComponent;

// Graph edge (brute force phase)
typedef struct {
    long neighPtr1;
    long neighPtr2;
    long deletedLevel;     // recursion level at which this edge was deleted (0 = not deleted)
} TBruteForceEdge;

// TNodeNeighbour neighbours[MaxE2 + 3];       // node neighbours buffer [1..neighCnt]
TNodeNeighbour neighbours[MaxERLimit + 3];     // node neighbours buffer [1..neighCnt]
TNode          nodes     [MaxN  + 2];          // nodes [1..N]
long neighCnt       = 0;

TConnectedComponent connectedComponents[MaxN + 2];  // connected components [1..connCompoCount]
long connCompoCount = 0;                            // number of connected components

TBruteForceEdge bfEdges[MaxERLimit + 3];    // graph edges (brute-force phase) [0..bfEdgesCount-1]
long bfEdgesCount = 0;

long nodeHeap[MaxN + 2];    // binary heap [1..heapPtr]
long heapPtr = 0;           // number of heap elements

long stopnieWierzcholkow[MaxN+3];       // this is for preliminary graph reduction
long krawedzieRaw1[MaxE+3][2];          // this is for preliminary graph reduction (krawedzie sort po poczatkach)
long krawedzieRaw2[MaxE+3][2];          // this is for preliminary graph reduction (krawedzie sort po koncach)

using namespace std;

inline bool isPossible() {
    return ((NR <= NRLimit) && ((E - ER) <= ((NRLimit - NR) * (NRLimit - NR))));
}

/*
void printInfo() {
    cerr << "    NR=" << NR << ", NRLimit=" << NRLimit << ", E=" << E << ", ER=" << ER << std::endl;
}

void printNode(long n) {
    cerr << "    node " << n << ((nodes[n].deleted) ? " (deleted)" : "") << " has " << (nodes[n].predList.itemCount) << " preds:";
    for (long p = nodes[n].predList.firstItem; p; p = neighbours[p].next)
        cerr << " " << p << "->" << neighbours[p].nn << "";
    cerr << std::endl;
    cerr << "    node " << n << ((nodes[n].deleted) ? " (deleted)" : "") << " has " << (nodes[n].succList.itemCount) << " succs:";
    for (long p = nodes[n].succList.firstItem; p; p = neighbours[p].next)
        cerr << " " << p << "->" << neighbours[p].nn << "";
    cerr << std::endl;
}
*/

/** number of adjacent nodes */
inline long heap_value(long index) {
    return (nodes[nodeHeap[index]].predList.itemCount + nodes[nodeHeap[index]].succList.itemCount);
}

inline void heap_swap(long i, long j) {
    const long tmp = nodeHeap[i];
    nodeHeap[i] = nodeHeap[j];
    nodeHeap[j] = tmp;
}

/** bigger first */
void heap_up(long current) {
    long parent = 0;
    while ((2 <= current) && (heap_value(current) > heap_value(parent=(current/2)))) {
        heap_swap(current, parent);
        current = parent;
    }
}

/** bigger first */
void heap_down(long current) {
    const long sz    = heapPtr;
          long child = 0;

    while (current <= sz) {
        child = 2 * current;

        if ((child <  sz) && (heap_value(child+1) > heap_value(child)))
            child++;
        if ((child <= sz) && (heap_value(child) > heap_value(current))) {
            heap_swap(current, child);
            current = child;
        } else
            break;
    }
}

inline long heap_top() {
    return nodeHeap[1];
}

long heap_pop() {
    const long topv = heap_top();

    if (2 <= heapPtr) {
        nodeHeap[1] = nodeHeap[heapPtr--];
        heap_down(1);
    } else
        --heapPtr;

    return topv;
}

void heap_push(long nn) {
    nodeHeap[++heapPtr] = nn;
    heap_up(heapPtr);
}

void initNodeHeap() {
    for (long i = 1; i <= N; ++i) {
        if (! (nodes[i].deleted))
            heap_push(i);
        //cerr << "initNodeHeap: Node " << i << ": pred/succ: " << (nodes[i].predList.itemCount) << "/" << (nodes[i].succList.itemCount) << std::endl;
    }
}

/** doubly linked list insert (at the end) */
void double_list_insert_last(TDoubleLinkedList& dl, const long item) {
    neighbours[item].prev = dl.lastItem;
    neighbours[item].next = 0;

    if (dl.lastItem)
        neighbours[dl.lastItem].next = item;
    else
        dl.firstItem = item;

    dl.lastItem = item;
    dl.itemCount++;
}

void double_list_delete(TDoubleLinkedList& dl, const long item) {
    const long itemPrev = neighbours[item].prev;
    const long itemNext = neighbours[item].next;

    if (itemPrev)
        neighbours[itemPrev].next = itemNext;
    else
        dl.firstItem = itemNext;

    if (itemNext)
        neighbours[itemNext].prev = itemPrev;
    else
        dl.lastItem = itemPrev;

    neighbours[item].next = 0;
    neighbours[item].prev = 0;
    dl.itemCount--;
}

void double_list_clear(TDoubleLinkedList& dl) {
    long p2 = dl.firstItem;

    while (p2) {
        const long p1 = neighbours[p2].next;
        neighbours[p2].prev = 0;
        neighbours[p2].next = 0;
        p2 = p1;
    }

    dl.firstItem = 0;
    dl. lastItem = 0;
    dl.itemCount = 0;
}

/**
 * Dodaje wierzcholek n2 na koniec listy poprzednikow wierzcholka n1.
 * This leaves graph in inconsistent state!
 */
void addNodePredecessor_basic(long n1, long n2, long item, long backPtr) {
    neighbours[item].nn      = n2;
    neighbours[item].backPtr = backPtr;
    double_list_insert_last(nodes[n1].predList, item);
}

/**
 * Dodaje wierzcholek n2 na koniec listy nastepnikow wierzcholka n1.
 * This leaves graph in inconsistent state!
 */
void addNodeSuccessor_basic(long n1, long n2, long item, long backPtr) {
    neighbours[item].nn      = n2;
    neighbours[item].backPtr = backPtr;
    double_list_insert_last(nodes[n1].succList, item);
}

/** Directed edge from n1 to n2. */
void addEdge(long n1, long n2) {
    const long ncnt = neighCnt;
    addNodeSuccessor_basic  (n1, n2, ++neighCnt, ncnt + 2);
    addNodePredecessor_basic(n2, n1, ++neighCnt, ncnt + 1);
}

/**
 * Removes neighIndex from its list. It must be a predecessor of its node.
 * This leaves graph in inconsistent state!
 */
void removeNodePredecessor_basic(const long nn, const long neighIndex) {
    double_list_delete(nodes[nn].predList, neighIndex);
    neighbours[neighIndex].nn      = 0;
    neighbours[neighIndex].backPtr = 0;
}

/**
 * Removes neighIndex from its list. It must be a successor of its node.
 * This leaves graph in inconsistent state!
 */
void removeNodeSuccessor_basic(const long nn, const long neighIndex) {
    double_list_delete(nodes[nn].succList, neighIndex);
    neighbours[neighIndex].nn      = 0;
    neighbours[neighIndex].backPtr = 0;
}

inline bool checkRemoveBfEdge(long neighPtr, long bfLevel) {
    const long bfEdgeIndex = neighbours[neighPtr].bfEdgeIndex;
    if (! (bfEdges[bfEdgeIndex].deletedLevel)) {
           bfEdges[bfEdgeIndex].deletedLevel = bfLevel;
           return true;
    }
    return false;
}

/** Removes all incoming edges of node n1. */
void removePredEdges(long n1, long bfLevel=0) {
    for (long p = nodes[n1].predList.firstItem; p; p = neighbours[p].next) {
        if (bfLevel) {
            if (checkRemoveBfEdge(p, bfLevel))
                nodes[neighbours[p].nn].succList.itemCount--;
        } else
            removeNodeSuccessor_basic(neighbours[p].nn, neighbours[p].backPtr);
    }

    ER += nodes[n1].predList.itemCount;
    //cerr << "remove node: " << n1 << ": removed " << (nodes[n1].predList.itemCount) << " pred edges (level: " << bfLevel << ")" << std::endl;

    if (bfLevel)
        nodes[n1].predList.itemCount = 0;
    else
        double_list_clear(nodes[n1].predList);
}

/** Removes all outgoing edges of node n1. */
void removeSuccEdges(long n1, long bfLevel=0) {
    for (long p = nodes[n1].succList.firstItem; p; p = neighbours[p].next) {
        if (bfLevel) {
            if (checkRemoveBfEdge(p, bfLevel))
                nodes[neighbours[p].nn].predList.itemCount--;
        } else
            removeNodePredecessor_basic(neighbours[p].nn, neighbours[p].backPtr);
    }

    ER += nodes[n1].succList.itemCount;
    //cerr << "remove node: " << n1 << ": removed " << (nodes[n1].succList.itemCount) << " succ edges (level: " << bfLevel << ")" << std::endl;

    if (bfLevel)
        nodes[n1].succList.itemCount = 0;
    else
        double_list_clear(nodes[n1].succList);
}

/** Removes given node from the graph. */
void removeNode(long n1, long bfLevel=0) {
/*  if (bfLevel) {
        // cerr << "Remove node: " << n1 << ", bfLevel=" << bfLevel << std::endl;
    } else {
        cerr << "Remove node: " << n1 << std::endl;
        printNode(n1);
        printInfo();
    }
*/
    removePredEdges(n1, bfLevel);
    removeSuccEdges(n1, bfLevel);
    nodes[n1].deleted = 1;
    ++NR;
    // printInfo();
}

void removeNodes(const set<long>& ns) {
    for (set<long>::const_iterator it = ns.begin(); ns.end() != it; ++it) {
        removeNode(*it);
    }
}

inline bool checkRestoreBfEdge(long neighPtr, long bfLevel) {
    const long bfEdgeIndex = neighbours[neighPtr].bfEdgeIndex;
    if (bfLevel <= (bfEdges[bfEdgeIndex].deletedLevel)) {
        bfEdges[bfEdgeIndex].deletedLevel = 0;      // undelete edge
        return true;
    }
    return false;
}

/** Restores incoming edges of node n1 (brute-force phase only). */
void restorePredEdges(long n1, long bfLevel) {
    long restoreCnt = 0;

    for (long p = nodes[n1].predList.firstItem; p; p = neighbours[p].next) {
        if (checkRestoreBfEdge(p, bfLevel)) {
            nodes[neighbours[p].nn].succList.itemCount++;
            ++restoreCnt;
        }
    }

    //cerr << "restore node " << n1 << ": restored " << restoreCnt << " pred edges (level: " << bfLevel << ")" << std::endl;
    nodes[n1].predList.itemCount = restoreCnt;
    ER -= restoreCnt;
}

/** Restores outgoing edges of node n1 (brute-force phase only). */
void restoreSuccEdges(long n1, long bfLevel) {
    long restoreCnt = 0;

    for (long p = nodes[n1].succList.firstItem; p; p = neighbours[p].next) {
        if (checkRestoreBfEdge(p, bfLevel)) {
            nodes[neighbours[p].nn].predList.itemCount++;
            ++restoreCnt;
        }
    }

    //cerr << "restore node " << n1 << ": restored " << restoreCnt << " succ edges (level: " << bfLevel << ")" << std::endl;
    nodes[n1].succList.itemCount = restoreCnt;
    ER -= restoreCnt;
}

/** Restores previously deleted graph node (brute-force phase only). */
void restoreNode(long n1, long bfLevel) {
    // cerr << "Restore node: " << n1 << ", bfLevel=" << bfLevel << std::endl;
    restorePredEdges(n1, bfLevel);
    restoreSuccEdges(n1, bfLevel);
    nodes[n1].deleted = 0;
    --NR;
    //printInfo();
    //printNode(n1);
}

/** removes nodes with too many adjacent edges */
bool removePopularNodes() {
    while ((heapPtr) && (NR <= NRLimit)) {
        const long nntop = heap_top();
        const long adjc  = nodes[nntop].predList.itemCount + nodes[nntop].succList.itemCount;

        if (adjc > (NRLimit - NR)) {
            if (NR < NRLimit) {
                heap_pop();
                removeNode(nntop);
            } else
                return false;
        } else
            break;
    }

    return true;
}

bool removeLeafParents() {
    for (long i = 1; i <= N; ++i) {
        if (! (nodes[i].deleted)) {
            //cerr << "removeLeafParents: Node " << i << ": pred/succ: " << (nodes[i].predList.itemCount) << "/" << (nodes[i].succList.itemCount) << std::endl;

            if (1 == ((nodes[i].predList.itemCount) + (nodes[i].succList.itemCount))) {
                // Just 1 neighbour
                if (NR < NRLimit) {
                    if (nodes[i].predList.itemCount)
                        removeNode(neighbours[nodes[i].predList.firstItem].nn);
                    else
                    if (nodes[i].succList.itemCount)
                        removeNode(neighbours[nodes[i].succList.firstItem].nn);
                } else
                    return false;
            }
        }
    }

    return true;
}

inline void initConnectedComponent_BFS_push(std::queue<long>& q, const long neigh, const long ccid) {
    if ((! (nodes[neigh].deleted)) && (ccid != (nodes[neigh].connCompo)))
        q.push(neigh);
}

void initConnectedComponent_BFS(const long startNode, const long ccid) {
    long nodeCnt = 0;
    long edgeCnt = 0;
    queue<long> bfs;
    bfs.push(startNode);

    for (; (! (bfs.empty())); ++nodeCnt) {
        const long n = bfs.front();
        nodes[n].connCompo = ccid;

        for (long p = nodes[n].predList.firstItem; p; p = neighbours[p].next) {
            initConnectedComponent_BFS_push(bfs, neighbours[p].nn, ccid);
            ++edgeCnt;
        }
        for (long p = nodes[n].succList.firstItem; p; p = neighbours[p].next) {
            initConnectedComponent_BFS_push(bfs, neighbours[p].nn, ccid);
            ++edgeCnt;
        }

        bfs.pop();
    }

    connectedComponents[ccid].nodeCount = nodeCnt;
    connectedComponents[ccid].edgeCount = edgeCnt / 2;
}

void initConnectedComponents() {
    for (long i = 1; i <= N; ++i) {
        if ((! (nodes[i].deleted)) && (! (nodes[i].connCompo))) {
            //cerr << "Starting connected component with node " << i << " (connCompoCount=" << connCompoCount << ")" << std::endl;
            initConnectedComponent_BFS(i, ++connCompoCount);
        }
    }

/*  cerr << "Found " << connCompoCount << " connected components:" << std::endl;
    for (long i = 1; i <= N; ++i) {
        if (! (nodes[i].deleted))
            cerr << "  node " << i << ": cc=" << nodes[i].connCompo << std::endl;
    }
*/
}

void bfEdges_init() {
    memset(bfEdges, 0, sizeof(bfEdges));
    bfEdgesCount = 0;
}

inline void bfEdges_push(long neighPtr1, long neighPtr2) {
    neighbours[neighPtr1].bfEdgeIndex = bfEdgesCount;
    neighbours[neighPtr2].bfEdgeIndex = bfEdgesCount;
    //cerr << "bfEdges_push: " << bfEdgesCount << " => " << neighPtr1 << "," << neighPtr2 << " => " << neighbours[neighPtr2].nn << "," << neighbours[neighPtr1].nn << std::endl;
    bfEdges[bfEdgesCount  ].neighPtr1 = neighPtr1;
    bfEdges[bfEdgesCount++].neighPtr2 = neighPtr2;
}

/**
 * Start with edge startEdge; all edges before it are already removed.
 * removedNodes is an OUT parameter.
 */
long bruteForce_CC(const long level, const long startEdge, set<long>& removedNodes) {
    //cerr << "    [" << level << "]" << std::endl;

    set<long> best_nodes;
        long  best_res = MaxNRLimit + 1;        // number of removed nodes
    set<long> nodes2;
        long  l2 = 0;
        long  edgeCnt = 0;

    if (isPossible()) {
        for (long i = startEdge; (i < bfEdgesCount) && (0 == edgeCnt); ++i) {
            if (! (bfEdges[i].deletedLevel)) {
                const long node1 = neighbours[bfEdges[i].neighPtr2].nn;
                const long node2 = neighbours[bfEdges[i].neighPtr1].nn;
                //cerr << "[A] [" << level << "]: edge=" << i << ", node1=" << node1 << ", node2=" << node2 << std::endl;

                // Two nodes, try to remove 1, then restore 1 and remove 2.
                removeNode(node1, level);
                l2 = bruteForce_CC(1 + level, 1 + i, nodes2);
                if ((0 <= l2) && ((l2 + 1) < best_res)) {
                    best_nodes = nodes2;
                    best_nodes.insert(node1);
                    best_res   = best_nodes.size();
                    //cerr << "[A* [" << level << "]: best_res=" << best_res << std::endl;
                }
                restoreNode(node1, level);

                //cerr << "[B] [" << level << "]: edge=" << i << ", node1=" << node1 << ", node2=" << node2 << std::endl;

                removeNode(node2, level);
                l2 = bruteForce_CC(1 + level, 1 + i, nodes2);
                if ((0 <= l2) && ((l2 + 1) < best_res)) {
                    best_nodes = nodes2;
                    best_nodes.insert(node2);
                    best_res   = best_nodes.size();
                    //cerr << "[B* [" << level << "]: best_res=" << best_res << std::endl;
                }
                restoreNode(node2, level);

                ++edgeCnt;
            }
        }

        if (! edgeCnt) {    // success!
            best_res = 0;
            best_nodes.clear();
        }
    }

    if (best_res <= NRLimit) {
/*      if (best_res)
            cerr << "    [" << level << "]: SUCCESS! best_res=" << best_res << std::endl;
        else
            cerr << "    [" << level << "]: SUCCESS!" << std::endl;
*/
        removedNodes = best_nodes;
        return best_res;
    }

    return -1;          // error
}

void bruteForce_init(const long startNode) {
    const long ccid = nodes[startNode].connCompo;
    bfEdges_init();
    set<long> visited;

    for (long i = startNode; i <= N; ++i) {
        if ((! (nodes[i].deleted)) && (ccid == (nodes[i].connCompo))) {
            //cerr << "bruteForce_init: i=" << i << std::endl;

            for (long p = nodes[i].predList.firstItem; p; p = neighbours[p].next) {
                if (! (visited.count(p))) {
                    bfEdges_push(neighbours[p].backPtr,p);
                    visited.insert(p);
                    visited.insert(neighbours[p].backPtr);
                }
            }

            for (long p = nodes[i].succList.firstItem; p; p = neighbours[p].next) {
                if (! (visited.count(p))) {
                    bfEdges_push(p,neighbours[p].backPtr);
                    visited.insert(p);
                    visited.insert(neighbours[p].backPtr);
                }
            }
        }
    }
}

long bruteForce_connectedComponents(set<long>& nodesToRemove) {
    set<long> visitedCC;        // visited connected components
    set<long> nodesTR;

    for (long i = 1; i <= N; ++i) {
        if ((! (nodes[i].deleted)) && (! (visitedCC.count(nodes[i].connCompo)))) {
            const long ccid = nodes[i].connCompo;
            visitedCC.insert(ccid);
            //cerr << "==== bruteForce_connectedComponents: cc=" << (ccid) << std::endl;
            //printInfo();

            if (connectedComponents[ccid].edgeCount) {
                if (isPossible()) {
                    set<long> ccNodes;
                    bruteForce_init(i);
                    long ccRes = bruteForce_CC(1, 0, ccNodes);
                    if  (ccRes < 0)
                        return -1;
                    nodesTR.insert(ccNodes.begin(), ccNodes.end());
                    removeNodes(ccNodes); // this is the best solution for this connected component, so we can delete the nodes

                    // cerr << "==== [bf] start with node " << i << " (cc=" << (ccid) << ": " << connectedComponents[ccid].nodeCount << "/" << connectedComponents[ccid].edgeCount << "): ccRes=" << ccRes << std::endl;
                    //printInfo();
                } else
                    return -1;
            }
        }
    }

    //cerr << "==== bruteForce_connectedComponents: res=" << nodesTR.size() << std::endl;
    //printInfo();
    nodesToRemove = nodesTR;
    return nodesTR.size();
}

void init() {
    memset(neighbours, 0, sizeof(neighbours));
    memset(nodes,      0, sizeof(nodes));
    memset(bfEdges,    0, sizeof(bfEdges));
    memset(connectedComponents, 0, sizeof(connectedComponents));
    memset(nodeHeap,   0, sizeof(nodeHeap));

    memset(stopnieWierzcholkow, 0, sizeof(stopnieWierzcholkow));
    memset(krawedzieRaw1,       0, sizeof(krawedzieRaw1));
    memset(krawedzieRaw2,       0, sizeof(krawedzieRaw2));

/*  cerr << "size neighbours: " << sizeof(neighbours) << std::endl;
    cerr << "size nodes: " << sizeof(nodes) << std::endl;
    cerr << "size bfEdges: " << sizeof(bfEdges) << std::endl;
    cerr << "size connectedComponents: " << sizeof(connectedComponents) << std::endl;
    cerr << "size nodeHeap: " << sizeof(nodeHeap) << std::endl;
    cerr << "size stopnieWierzcholkow: " << sizeof(stopnieWierzcholkow) << std::endl;
    cerr << "size krawedzieRaw: " << sizeof(krawedzieRaw) << std::endl;
    cerr << "size total: " << sizeof(neighbours) + sizeof(nodes) + sizeof(bfEdges) + sizeof(connectedComponents) + sizeof(nodeHeap) + sizeof(stopnieWierzcholkow) + sizeof(krawedzieRaw) << std::endl; */
}

/* void readData() {
    long n1, n2, k;
    cin >> N >> k >> E;
    NRLimit = N - k;

    for (long i = 0; (i < E); ++i) {
        cin >> n1 >> n2;
        addEdge(n1, n2);
    }
} */

inline void swap(long t[][2], long i1, long i2) {
    long n1  = t[i1][0];
    long n2  = t[i1][1];
    t[i1][0] = t[i2][0];
    t[i1][1] = t[i2][1];
    t[i2][0] = n1;
    t[i2][1] = n2;
}

/** [left..right]: obustronnie domkniety przedzial indeksow */
void qsort(long t[][2], long left, long right) {
    if (left < right) {
        swap(t, left, (left + right) / 2);

        long m0 = t[left][0];
        long m1 = t[left][1];
        long k  = left;

        for (long i = left + 1; i <= right; ++i) {
            if ((t[i][0] < m0) || ((t[i][0] == m0) && (t[i][1] <= m1)))
                swap(t, i, ++k);
        }

        swap (t, left, k);
        qsort(t, left, k - 1);
        qsort(t, k + 1, right);
    }
}

void sortujKrawedzie() {
    qsort(krawedzieRaw1, 0, E-1);
    qsort(krawedzieRaw2, 0, E-1);
}

void removeNode_preliminary_decreaseAdjRanks(const long t[][2], const long nn) {
    long p0 = 0;
    long p1 = E - 1;

    while (p0 < p1) {
        long m = (p0 + p1) / 2;
        if (t[m][0] < nn)
            p0 = m + 1;
        else
        if (t[m][0] > nn)
            p1 = m - 1;
        else
            p1 = m;
    }

    if ((p0 == p1) && (nn == t[p0][0])) {
        for (long i = p0; nn == t[i][0]; ++i) {
            //cerr << "decr-rank: " << t[i][1] << std::endl;
            stopnieWierzcholkow[t[i][1]]--;
        }
    }
}

void removeNode_preliminary(const long nn) {
    // cerr << "[a] Remove node (preliminary phase): " << nn << std::endl;
    removeNode_preliminary_decreaseAdjRanks(krawedzieRaw1, nn);
    //cerr << "[b] Remove node (preliminary phase): " << nn << std::endl;
    removeNode_preliminary_decreaseAdjRanks(krawedzieRaw2, nn);
    stopnieWierzcholkow[nn] = -1;
    nodes[nn].deleted = 1;
    ++NR;
}

bool readData2() {
    long n1, n2, k;
    cin >> N >> k >> E;
    NRLimit = N - k;

    for (long i = 0; (i < E); ++i) {
        cin >> n1 >> n2;
        stopnieWierzcholkow[n1]++;
        stopnieWierzcholkow[n2]++;
        krawedzieRaw1[i][0] = n1;
        krawedzieRaw1[i][1] = n2;
        krawedzieRaw2[i][0] = n2;
        krawedzieRaw2[i][1] = n1;
    }

    sortujKrawedzie();

    for (long i = 0; (i < N); ++i) {
        if ((stopnieWierzcholkow[i]) > NRLimit) {
            removeNode_preliminary(i);
            if (NR > NRLimit)
                return false;
        }
    }

    for (long i = 0; (i < E); ++i) {
        n1 = krawedzieRaw1[i][0];
        n2 = krawedzieRaw1[i][1];

        if ((0 <= stopnieWierzcholkow[n1]) && (0 <= stopnieWierzcholkow[n2]))
            addEdge(n1, n2);
        else
            ++ER;
    }

    return true;
}

bool printResult() {
    if (E == ER) {
        cout << (N - NR) << std::endl;
        bool space = false;

        for (long i = 1; i <= N; ++i) {
            if (! (nodes[i].deleted)) {
                cout << (space ? " " : "") << i;
                space = true;
            }
        }

        if (space)
            cout << std::endl;
        return true;
    }

    return false;
}

int main() {
    init();

    if (readData2()) {
        initNodeHeap();

        if (removePopularNodes()) {
            if (printResult())
                return 0;
            if (removeLeafParents()) {
                if (printResult())
                    return 0;

                if (isPossible()) {
                    //cerr << "brute force phase (1)" << std::endl;
                    initConnectedComponents();
                    set<long> nodesToRemove;
                    //cerr << "brute force phase (2)" << std::endl;
                    long bfRes = bruteForce_connectedComponents(nodesToRemove);
                    if (0 <= bfRes) {
                        //cerr << "[1] NR=" << NR << ", NRLimit=" << NRLimit << ", E=" << E << ", ER=" << ER << ", bfRes=" << bfRes << std::endl;
                        if (printResult())
                            return 0;
                    }
                }
            }
        }
    }

    cout << "NIE" << std::endl;
    return 0;
}

