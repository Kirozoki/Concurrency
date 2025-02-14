#include <atomic>
#include <vector>
#include <iostream>
#include <functional>

#include "common_thread_safe_queue.h"
#include "common_objs.h"

class thread_pool {
    private:
    std::atomic_bool done;
    threadsafe_queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread() {
        while (!done)
        {
            std::function<void()> task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
        
    }

    public:

    thread_pool() : done(false), joiner(threads) {
        const int thread_count = std::thread::hardware_concurrency();
        std::cout << "" << std::endl;
        try {
            for (int i = 0; i < thread_count; ++i) {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread, this));
                
            }
        } catch(...) {
            done = true;
            throw;
        }
    }

    ~thread_pool() {
        done = true;
    }

    template<typename FunctionType>
    void submit(FunctionType f) {
        work_queue.push(std::function<void()>(f));
    }
};

void run_code1() {
    thread_pool pool;
    std::cout << "Testing thread pool" << std::endl;

    for (int i = 0; i < 100; ++i) {
        pool.submit([=] (){
            printf(" %d printed by thread - %d \n", i, std::this_thread::get_id());
        });
    }
    system("pause");
}

int main() {
    run_code1();
    //system("pause");
}
