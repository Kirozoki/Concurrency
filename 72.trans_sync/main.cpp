#include <iostream>
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false);

void thread1_func() {
    data[0].store(42, std::memory_order_relaxed);
    data[1].store(45, std::memory_order_relaxed);
    data[2].store(47, std::memory_order_relaxed);
    data[3].store(49, std::memory_order_relaxed);
    data[4].store(56, std::memory_order_relaxed);
    sync1.store(true, std::memory_order_release);
}

void thread2_func() {
    while (!sync1.load(std::memory_order_acquire));
    sync2.store(true, std::memory_order_release);
}

void thread3_func() {
    while(!sync2.load(std::memory_order_acquire));
    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 45);
    assert(data[2].load(std::memory_order_relaxed) == 47);
    assert(data[3].load(std::memory_order_relaxed) == 49);
    assert(data[4].load(std::memory_order_relaxed) == 56);
}

void run_code() {

    std::thread thread_1(thread1_func);
    std::thread thread_2(thread2_func);
    std::thread thread_3(thread3_func);

    thread_1.join();
    thread_2.join();
    thread_3.join();
}

int main() {
    run_code();
}