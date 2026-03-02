#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

enum State {
    Read_n,
    Read_ordering,
    Read_student,
};

State state = Read_student;
size_t n = 0;
std::vector<int> ordering;

using namespace std;

std::vector<int> parse_line(const std::string& line)
{
    std::istringstream iss(line);
    std::vector<int> values;
    int x;

    while (iss >> x) {
        if (0 == x)
            break;
        values.push_back(x);
    }

    return values;
}

//std::string print_vec(const std::vector<int>& vec) {
//    std::ostringstream oss;
//    for (auto x : vec) {
//        oss << " " << x;
//    }
//    return oss.str();
//}

// This assumes perm is a correct permutation, and as a result computes its reverse.
std::vector<int> perm_reverse(const std::vector<int>& perm) {
    vector<int> res(perm.size(), 0);
    for (size_t i=0; i < perm.size(); ++i)
        res[perm[i]-1] = i+1;
    return res;
}

int solve(const std::vector<int>& student) {
    // lseq[i] == length of the longest subsequence ending at i.
    vector<int> lseq(student.size(), 1);
    // cout << " student: " << print_vec(student) << endl;

    for (size_t i=0; i < student.size(); ++i) {
        for (size_t j=0; j < i; ++j) {
            if (ordering[student[j]-1] < ordering[student[i]-1])
                lseq[i] = max(lseq[i], 1+lseq[j]);
        }
    }

    return *max_element(lseq.begin(), lseq.end());
}

int main() {
    // Disable synchronization of all the iostream standard streams with their corresponding
    // standard C streams: http://www.cplusplus.com/reference/ios/ios_base/sync_with_stdio/ .
    std::ios_base::sync_with_stdio(0);

    // By default, cin is tied to the standard output stream cout (see ios::tie), which
    // indicates that cout's buffer is flushed (see ostream::flush) before each I/O
    // operation performed on cin: http://www.cplusplus.com/reference/iostream/cin/ .
    //
    // Here we disable this behavior.
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string line;

    while (std::getline(std::cin, line)) {
        std::vector<int> numbers(parse_line(line));

        switch(numbers.size()) {
            case 0:
                return 0;
            case 1:
                // most likely a new test case
                n = numbers[0];
                state = Read_n;
                break;
            default:
                switch (state) {
                    case Read_n:
                        if (numbers.size() == n) {
                            ordering = std::move(numbers);
                            // cout << "ordering: " << print_vec(ordering) << endl;
                        }
                        state = Read_ordering;
                        break;
                    case Read_ordering:
                    case Read_student:
                        if (numbers.size() == n)
                            cout << solve(perm_reverse(numbers)) << endl;
                        break;
                }
        }
    }

    return 0;
}

