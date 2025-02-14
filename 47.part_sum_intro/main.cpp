#include <iostream>
#include <future>
#include <numeric>
#include <vector>
#include <chrono>

#include <execution>

#include "common_objs.h"
#include "utils.h"

const size_t testSize = 10000000;

template<typename Iterator, typename OutIterator>
void sequential_partial_sum(Iterator first, Iterator last, OutIterator y) {

    unsigned long const length = std::distance(first, last);

    y[0] = first[0];

    for(size_t i = 1; i < length; ++i) {
        y[i] = y[i - 1] + first[i]; // yi = y(i-1) + xi

    }
}

int main() { // inclusive_scan, exclusive_scan, partial_sum(the one)

    std::vector<int> ints(testSize);
    std::vector<int> outs(testSize);

    for (size_t i = 0; i < testSize; ++i) {
        ints[i] = i;
    }

    //seuential implementation
    std::chrono::high_resolution_clock::time_point startTime = high_resolution_clock::now();
    std::inclusive_scan(ints.begin(), ints.end(), outs.begin());
    std::chrono::high_resolution_clock::time_point endTime = high_resolution_clock::now();
    print_results("sequential scan", startTime, endTime);

    // c++17 parallel implementation
    startTime = high_resolution_clock::now();
    std::inclusive_scan(std::execution::par, ints.begin(), ints.end(), outs.begin());
    endTime = high_resolution_clock::now();    
    print_results("parallel scan", startTime, endTime);

}

//exclusive doesn't calculate i element 1,2,3 => 0,1,3
