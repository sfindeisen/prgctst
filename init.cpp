#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

typedef int Int;
typedef unsigned int UInt;
typedef long long Long;
typedef unsigned long long ULong;

// typedef std::vector<Int> VInt;
// typedef std::vector<Long> VLong;

// #define DEBUG(msg) std::cerr << __LINE__ << ": " << msg << std::endl
// #define FOR(x,a,b) for (decltype(b) x=(a); x<(b); ++x)
// #define FORREV(x,a,b) for (decltype(a) x=(b); (a)<=(--x);)
// #define REP(x,b) FOR(x,0,(b))
// #define REP(b) REP(rep##__LINE__,(b))
// #define VAR(x,v) decltype(v) x=(v)
// #define VARITER(x,it,pos) std::pair<decltype(it),Int> x((it),pos)
// #define FOREACH(x,c) for (VARITER(x,(c).begin(),0); x.first!=(c).end(); ++x.first,++x.second)
// #define FOREACHREV(x,c) for (VARITER(x,(c).rbegin(),(c).size()-1); x.first!=(c).rend(); ++x.first,--x.second)
// #define POS(variter) variter.second
// #define ssize(c) ((Long)((c).size()))

using namespace std;

int main() {
    // By default, cin is tied to the standard output stream cout (see ios::tie), which
    // indicates that cout's buffer is flushed (see ostream::flush) before each I/O
    // operation performed on cin: http://www.cplusplus.com/reference/iostream/cin/ .
    //
    // Here we disable this behavior.
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // Disable synchronization of all the iostream standard streams with their corresponding
    // standard C streams: http://www.cplusplus.com/reference/ios/ios_base/sync_with_stdio/ .
    std::ios_base::sync_with_stdio(false);

    return 0;
}

