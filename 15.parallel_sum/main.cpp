#include <iostream>
#include <functional>
#include <numeric>
#include <thread>
#include <vector>

const int MIN_BLOCK_SIZE = 1000;

template <typename Iterator, typename T>
void GetSummary(Iterator start, Iterator end, T& sum_ref) {
    sum_ref = std::accumulate(start, end, sum_ref);
}

template <typename Iterator, typename T>
void ParallelGetSummary(Iterator start, Iterator end, T* sum_ptr) {
    int qty_elements = std::distance(start, end);

    int avail_kernels = std::thread::hardware_concurrency();

    int threads_qty = std::min(qty_elements / MIN_BLOCK_SIZE, avail_kernels);

    std::vector<int> thread_sums(threads_qty);
    std::vector<std::thread> threads_v(threads_qty - 1);

    for(int i = 0; i < threads_qty - 1; ++i) {
        Iterator begin = start;
        std::advance(start, MIN_BLOCK_SIZE);
        threads_v[i] = std::thread(GetSummary<Iterator, T>, begin, start, std::ref(thread_sums[i]));
    }

    GetSummary(start, end, thread_sums[threads_qty - 1]);
    std::for_each(threads_v.begin(), threads_v.end(), std::mem_fn(&std::thread::join));
    GetSummary(thread_sums.begin(), thread_sums.end(), *sum_ptr);
}

int main() {
    std::vector<int> vec(20000, 2);
    int sum = 0;

    ParallelGetSummary(vec.begin(), vec.end(), &sum);

    std::cout << "Sum of vector elements equals to: " << sum << std::endl;
}