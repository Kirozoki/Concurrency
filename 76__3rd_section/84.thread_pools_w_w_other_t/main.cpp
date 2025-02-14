#include <atomic>
#include <numeric>
#include <vector>
#include <iostream>
#include <functional>
#include <future>
#include <list>
#include <thread>

#include "common_thread_safe_queue.h"
#include "common_objs.h"

#include "utils.h"

class thread_pool_waiting_other_tasks {
    private:
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue; // copy constructable
    // therefore we need to use function_wrapper class 
    //instead of std::function<void()>
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread() {
        while (!done)
        {
            function_wrapper task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
        
    }

    public:

    thread_pool_waiting_other_tasks() : done(false), joiner(threads) {
        const int thread_count = std::thread::hardware_concurrency();
        try {
            for (int i = 0; i < thread_count; ++i) {
                threads.push_back(
                    std::thread(&thread_pool_waiting_other_tasks::worker_thread, this));
                
            }
        } catch(...) {
            done = true;
            throw;
        }
    }

    ~thread_pool_waiting_other_tasks() {
        done = true;
    }

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task)); // it's not copy constructable () => error while compiling
        return res;
    }

    void run_pending_task() {
        function_wrapper task;
        if (work_queue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield;
        }
    }
};

template<typename T>
struct sorter
{

    thread_pool_waiting_other_tasks pool;

    std::list<T> do_sort(std::list<T>& chunk_data) {
        if (chunk_data.size() < 2) {
            return chunk_data;
        }
        std::list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin());
        const T& partition_val = *result.begin();

        typename std::list<T>::iterator divide_point = std::partition(chunk_data.begin()
            , chunk_data.end(), [&partition_val] (const T& val) {
                return val < partition_val;});
        std::list<T> new_lower_chunk;
        new_lower_chunk.splice(new_lower_chunk.end()
            , chunk_data, chunk_data.begin(), divide_point);

        std::future<std::list<T>> new_lower = 
            pool.submit(std::bind(&sorter::do_sort, this, std::move(new_lower_chunk)));
        
        std::list<T> new_higher(do_sort(chunk_data));

        result.splice(result.end(), new_higher);

        while(new_lower.wait_for(std::chrono::seconds(0))== std::future_status::timeout)
        //while (!new_lower._Is_ready())
        {
            pool.run_pending_task();
        }
        
        result.splice(result.begin(), new_lower.get());

        return result;
    }
	
};

template<typename T>

std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) {
    return input;}

    sorter<T> s;
    return s.do_sort(input);
}

void run_code1() {
    const int size = 800;
    std::list<int> my_array;

    srand(0);

    for (size_t i = 0; i < size; ++i) {
        my_array.push_back(rand());
    }

    my_array = parallel_quick_sort(my_array);

    for (size_t i = 0; i < size; ++i) {
        std::cout << my_array.front() << std::endl;
        my_array.pop_front();
    }
}

int main() {
    run_code1();
    //system("pause");
}
