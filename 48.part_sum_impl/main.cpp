#include <iostream>
#include <vector>
#include <execution>

#include "partial_sum.h"
#include "utils.h"

const size_t testSize = 1'000'000;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::milli;

int main() {
    std::vector<int> ints(testSize);
    std::vector<int> outs(testSize);

    for (size_t i = 0; i < testSize; ++i) {
        ints[i] = 1;
    }

    //seuential implementation
    auto startTime = high_resolution_clock::now();
    std::inclusive_scan(ints.begin(), ints.end(), outs.begin());
    auto endTime = high_resolution_clock::now();
    print_results("sequential scan", startTime, endTime);

    // c++17 parallel implementation
    startTime = high_resolution_clock::now();
    std::inclusive_scan(std::execution::par, ints.begin(), ints.end(), outs.begin());
    endTime = high_resolution_clock::now();    
    print_results("parallel scan", startTime, endTime);

     // custom parallel implementation
    startTime = high_resolution_clock::now();
    parallel_partial_sum(ints.begin(), ints.end());
    endTime = high_resolution_clock::now();    
    print_results("custom parallel scan", startTime, endTime);
}