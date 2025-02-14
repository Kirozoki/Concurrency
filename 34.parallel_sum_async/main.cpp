#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <future>

int MIN_ELEMENT_COUNT = 1000; // const deleted

template<typename iterator, typename T>
T parallel_accumulate(iterator begin, iterator end)
{
	int length = std::distance(begin, end);

	//atleast runs 1000 element
	if (length <= MIN_ELEMENT_COUNT)
	{
		//std::cout << std::this_thread::get_id() << std::endl;
		return std::accumulate(begin, end, T{});
	}

	iterator mid = begin;
	std::advance(mid, (length + 1) / 2);

	//recursive all to parallel_accumulate
	std::future<T> f = std::async(std::launch::deferred | std::launch::async,
		parallel_accumulate<iterator, T>, mid, end);
	T sum = parallel_accumulate<iterator, T>(begin, mid);
	return sum + f.get();
}

int main() {
    
    std::vector<int> vec(20000, 2);
    int sum = parallel_accumulate<std::vector<int>::iterator, int>(vec.begin(), vec.end());

    std::cout << "Sum of vector elements equals to: " << sum << std::endl;

    std::vector<char> v(10000, 'A');
    std::string s = parallel_accumulate<std::vector<char>::iterator, std::string>(v.begin(), v.end());

    std::cout << "Sum of vector elements equals to: " << s << std::endl;
    std::cout << "Size of attained string equals to: " << sizeof(s) << std::endl;
    std::cout << "Size of attained int equals to: " << sizeof(sum) << std::endl;
}