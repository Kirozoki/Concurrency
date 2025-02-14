#include <iostream>
#include <list>
#include <algorithm>
#include <future>
#include <chrono>
#include <random>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::milli;
using std::random_device;

const size_t testSize = 50'000;
const int iterationCount = 5;

template <typename T> 
std::list<T> parallel_quick_sort(std::list<T> input) // ~4sec
{
    if (input.size() < 2) {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T pivot = *result.begin();

    // arrange the input array
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t) { return t < pivot; });
    std::list<T> lower_list;
    lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

    auto new_lower(parallel_quick_sort(std::move(lower_list)));
    std::future<std::list<T>> new_upper_future(std::async(/*std::launch::async, - slower!!!*/ &parallel_quick_sort<T>, std::move(input)));
    // arranging the result list
    result.splice(result.begin(), new_lower);
    result.splice(result.end(), new_upper_future.get());
    return result;
}

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	//recursive condition
	if ( input.size() < 2 )
	{
		return input;
	}

	//move frist element in the list to result list and take it as pivot value
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T pivot = *result.begin();

	//partition the input array so that t< pivot in lower part and t> pivot in upper part of input list
	auto divide_point = std::partition(input.begin(), input.end(),
		[pivot](T const& t)
		{
			return t < pivot;
		});

	//move lower part of the list to separate list so that we can make recursive call
	std::list<T> lower_list;
	lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

	//call the sequenctial_quick_sort recursively
	auto new_lower(sequential_quick_sort(std::move(lower_list)));
	auto new_upper(sequential_quick_sort(std::move(input)));

	//transfer all elements in to result list
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_upper);

	return result;
}

int main() {
    
    random_device rd;

    // generate some random integers:
    printf("Testing with %zu integers...\n", testSize);
    std::list<int> integers(testSize);
    for (auto& d : integers) {
        d = static_cast<int>(rd());
    }

     // time how long it takes to sort them:
    for (int i = 0; i < iterationCount; ++i)
    {
        const auto startTime = high_resolution_clock::now();
        std::list<int> sorted = sequential_quick_sort(integers);
        const auto endTime = high_resolution_clock::now();
        std::cout << "Sequential STL: time " << duration_cast<duration<double, milli>>(endTime - startTime).count() << "ms" << std::endl;
    }

    for (int i = 0; i < 1; ++i)
    {
        const auto startTime = high_resolution_clock::now();
        std::list<int> sorted = parallel_quick_sort(integers);
        const auto endTime = high_resolution_clock::now();
        // in our output, note that these are the parallel results:
		std::cout << "Parallel STL: time " << duration_cast<duration<double, milli>>(endTime - startTime).count() << "ms" << std::endl;
    }
    
} 

/*
Testing with 50000 integers...
Sequential STL: time 24.0486ms
Sequential STL: time 23.9708ms
Sequential STL: time 23.7425ms
Sequential STL: time 24.0272ms
Sequential STL: time 23.5842ms
Parallel STL: time 3852.23ms
*/