#include <iostream>
#include <thread>
#include <mutex>

class spinlock_mutex
{
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT; // by default is false
public:
    spinlock_mutex(){}
    ~spinlock_mutex(){}

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)); // for 2nd thread it sees true value, and await to th 1st thread unlock() fucntion.
    }

    void unlock() {
        flag.clear(std::memory_order_release); // set flag value to false
    }
};

spinlock_mutex mutex;

void func() {
    std::lock_guard<spinlock_mutex> lg(mutex);
    std::cout << std::this_thread::get_id() << " hello " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void run_code() {
    std::thread thread_1(func);
    std::thread thread_2(func);

    thread_1.join();
    thread_2.join();
}

int main() {
    run_code();
}