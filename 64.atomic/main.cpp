#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<bool> data_ready = false;
std::vector<int> data_vector;

//Happened before relationship

void reader_func() {
    while (!data_ready)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    //synchronization 
    //inter-thread happen before btw two threads
    std::cout << data_vector[0] << std::endl;
}

void writer_func() {
    data_vector.push_back(3);
    data_ready.store(true);
}

void run_code() {
    std::thread reader_thread(reader_func);
    std::thread writer_thread(writer_func);

    reader_thread.join();
    writer_thread.join();
}